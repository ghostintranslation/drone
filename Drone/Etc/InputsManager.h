// NOTE: With this technique the inputs indexes are not always correct,
// for example input 2 will sometime be sampling in index 0
// That might be because of asynchronous ADC interrupt and mux iteration on timer tick

#ifndef InputsManager_h
#define InputsManager_h

#include <ADC.h>
#include <DMAChannel.h>
#include <AnalogBufferDMA.h>

// TODO: Couldn't get it to work with abdma1 as a member of the class...
DMAMEM static volatile uint16_t __attribute__((aligned(32))) inputsManagerDmaAdcBuff1[2];
DMAMEM static volatile uint16_t __attribute__((aligned(32))) inputsManagerDmaAdcBuff2[2];
AnalogBufferDMA abdma1(inputsManagerDmaAdcBuff1, 1, inputsManagerDmaAdcBuff2, 1);

elapsedMicros elaspedTime;

/**
 * The manager for the inputs.
 * Handles the multiplexer and DMA in order to read one value and then switch the multiplexer
 * at 44100 * 8 Hz. Fills all 8 inputs buffer that are read by the Input objects in 2.9ms as
 * required by the audio library.
 */
class InputsManager
{
public:
  static InputsManager *getInstance();
  void init();
  void update();
  int16_t *readInput(byte index);
  void interpolate(uint8_t index);
  void ProcessAnalogData();

private:
  // Singleton
  static InputsManager *instance;
  InputsManager();

  void iterate();

  static void adcIsr();
  //  static void timerCallback();

  static const byte inputsCount = 8;
  int muxIndex = 0;
  int inputIndex = 0;
  static const uint32_t buffSize = AUDIO_BLOCK_SAMPLES;

  ADC *adc;                   // adc object
                              //  AnalogBufferDMA *abdma1;
                              //  volatile uint16_t __attribute__((aligned(32))) dma_adc_buff1[2];
                              //  volatile uint16_t __attribute__((aligned(32))) dma_adc_buff2[2];
  const int timerPeriod = 10; // us
  static const uint32_t buffer_size = 1;
  int16_t bufferr[inputsCount][buffSize * 2] = {{0}};
  uint16_t bufferCountt[inputsCount] = {0};

  uint8_t pins[4] = {A0, A1, A2, A3};
  uint8_t readPin_adc_0 = A0;
};

// Singleton pre init
InputsManager *InputsManager::instance = nullptr;

inline InputsManager::InputsManager()
{
}

/**
 * Singleton instance
 */
inline InputsManager *InputsManager::getInstance()
{
  if (!instance)
    instance = new InputsManager;
  return instance;
}

inline void InputsManager::init()
{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  this->adc = new ADC();              // adc object
  this->adc->adc0->setAveraging(8);   // set number of averages
  this->adc->adc0->setResolution(12); // set bits of resolution
  this->adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
  this->adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);
  this->adc->adc0->enableInterrupts(adcIsr);

  //  abdma1 = new AnalogBufferDMA(this->dma_adc_buff1, this->buffer_size, this->dma_adc_buff2, this->buffer_size);
  abdma1.init(this->adc, ADC_0 /*, DMAMUX_SOURCE_ADC_ETC*/);

  // Start the dma operation..
  this->adc->adc0->startSingleRead(A0);                               // call this to setup everything before the Timer starts, differential is also possible
  this->adc->adc0->startTimer(AUDIO_SAMPLE_RATE_EXACT * inputsCount); // frequency in Hz

  digitalWriteFast(2, bitRead(this->muxIndex, 0));
  digitalWriteFast(3, bitRead(this->muxIndex, 1));
  digitalWriteFast(4, bitRead(this->muxIndex, 2));

  // wait enough time for the first timer conversion to finish (depends on resolution and averaging),
  // with 16 averages, 12 bits, and ADC_MED_SPEED in both sampling and conversion speeds it takes about 36 us.
  delayMicroseconds(25);
}

inline void InputsManager::update()
{
}

inline void InputsManager::iterate()
{
  this->inputIndex++;
  this->inputIndex = this->inputIndex % this->inputsCount;
  this->muxIndex = this->inputIndex % 8;

  digitalWriteFast(2, bitRead(this->muxIndex, 0));
  digitalWriteFast(3, bitRead(this->muxIndex, 1));
  digitalWriteFast(4, bitRead(this->muxIndex, 2));

  // if (this->readPin_adc_0 != pins[this->inputIndex / 8])
  // {

  //   this->readPin_adc_0 = this->pins[this->inputIndex / 8];
  //   this->adc->adc0->startSingleRead(this->readPin_adc_0);
  // }
}

inline int16_t *InputsManager::readInput(byte index)
{
  if (this->bufferCountt[index] >= 2)
  {
    if (this->bufferCountt[index] < this->buffSize)
    {
      this->interpolate(index);
    }

    this->bufferCountt[index] = 0;

    return this->bufferr[index];
  }

  return NULL;
}

// when the measurement finishes, this will be called
// first: see which pin finished and then save the measurement into the correct buffer
inline void InputsManager::adcIsr()
{
  getInstance()->ProcessAnalogData();
}

inline void InputsManager::ProcessAnalogData()
{
  volatile uint16_t *pbuffer = abdma1.bufferLastISRFilled();

  if ((uint32_t)pbuffer >= 0x20200000u)
    arm_dcache_delete((void *)pbuffer, sizeof(inputsManagerDmaAdcBuff1));

  if (this->bufferCountt[this->inputIndex] < AUDIO_BLOCK_SAMPLES)
  {
    this->bufferr[this->inputIndex][this->bufferCountt[this->inputIndex]] = *pbuffer * 16 - 32768; // converting from uint (0 to 4095) to int (-32668 to 32767) as the audio lib expects
    // if (bufferCountt[inputIndex] - 1 > 0)
    // {
    //   this->bufferr[this->inputIndex][this->bufferCountt[this->inputIndex]] = (*pbuffer * 16 - 32768) * 0.5 + this->bufferr[this->inputIndex][this->bufferCountt[this->inputIndex] - 1] * 0.5;
    // }
    // else
    // {
    //   this->bufferr[this->inputIndex][this->bufferCountt[this->inputIndex]] = *pbuffer * 16 - 32768;
    // }
    this->bufferCountt[this->inputIndex]++;
  }

  getInstance()->iterate();

  abdma1.clearInterrupt();
}

inline void InputsManager::interpolate(uint8_t index)
{
  int16_t result[AUDIO_BLOCK_SAMPLES] = {0};

  //      Serial.println(size);
  for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
  {
    // How far along are we?
    double srcpos = i * (float)(this->bufferCountt[index] - 1) / (float)AUDIO_BLOCK_SAMPLES;

    int srcel = (int)srcpos;

    // Alpha/beta fraction.
    srcpos -= srcel;

    result[i] = this->bufferr[index][srcel] * (1 - srcpos) + this->bufferr[index][srcel + 1] * srcpos;
  }

  memcpy(this->bufferr[index], result, sizeof(result));
}
#endif