// TODO: Couldn't get good results with DMA yet but should be investigated to increase sampling frequency
// TODO: Couldn't get good results with arm_fir_interpolate_q15 but should be investigated to benefits from hardware and more realistic interpolation
// TODO: maybe move the low pass filter from input to here, would remove it if using arm_fir_interpolate_q15

#ifndef InputsManager_h
#define InputsManager_h

// #include "arm_math.h"
#include <ADC.h>

/**
 * The manager for the inputs.
 * Handles the multiplexer in order to read one value and then switch the multiplexer
 * at 44100 * 8 Hz. Fills all 8 inputs buffer that are read by the Input objects in 2.9ms as
 * required by the audio library. Due to digitalWriteFast in the isr the ADC is not timed at the requested frequency,
 * so to reach 128 samples the buffers are interpolated.
 */
class InputsManager
{
public:
  static InputsManager *getInstance();
  void init();
  int16_t *readInput(byte index);

private:
  // Singleton
  static InputsManager *instance;
  InputsManager();

  void iterate();
  void interpolate(uint8_t index);
  void processAnalogData();
  static void adcIsr();

  static const byte inputsCount = 8;
  int muxIndex = 0;
  int inputIndex = 0;
  static const uint32_t buffSize = AUDIO_BLOCK_SAMPLES;
  static const uint32_t maxBuffers = 8;
  // static const uint32_t maxSamples = buffSize * maxBuffers; // 2.9ms * 8 = 23ms of data
  int16_t queue[inputsCount][maxBuffers][buffSize] = {{{0}}};
  // int16_t temp[inputsCount][buffSize] = {{0}};
  // int16_t tempHead[inputsCount] = {{0}};
  // int16_t interpolatedBuffer[inputsCount][buffSize] = {{0}};
  uint16_t head[inputsCount] = {0};
  uint16_t headSamplesCount[inputsCount] = {0};
  uint16_t tail[inputsCount] = {0};
  uint8_t pins[4] = {A0, A1, A2, A3};
  uint8_t readPin_adc_0 = A0;

  ADC *adc; // adc object

  AudioRecordQueue *audioRecordQueue[inputsCount];

  // Hardware accelerated interpolation and low pass filter
  // Taking 32 points and turning it to 128
  // Coeffs from https://forum.pjrc.com/threads/35951-FFT256-with-signal-decimation
  // arm_fir_interpolate_instance_q15 upSample_inst[inputsCount];
  // static const int16_t numTaps = 80;
  // static const int16_t upsample_factor = 4;
  // static const int16_t blockSize = AUDIO_BLOCK_SAMPLES / 4;
  // int16_t state[inputsCount][(numTaps / upsample_factor) + blockSize - 1] = {{0}};
  // int16_t coeffs4[80] = {0, -1, -2, -3, 0, 7, 16, 16, 0, -31,
  //                        -60, -57, 0, 96, 174, 155, 0, -239, -415, -357,
  //                        0, 516, 871, 731, 0, -1016, -1685, -1395, 0, 1907,
  //                        3159, 2625, 0, -3703, -6340, -5547, 0, 9649, 20719, 29479,
  //                        32743, 29300, 20468, 9474, 0, -5380, -6110, -3547, 0, 2482,
  //                        2967, 1779, 0, -1283, -1539, -921, 0, 652, 771, 453,
  //                        0, -307, -354, -202, 0, 128, 141, 77, 0, -44,
  //                        -46, -23, 0, 11, 11, 5, 0, -2, -1, 0};
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

  // for (int i = 0; i < inputsCount; i++)
  // {
  //   arm_fir_interpolate_init_q15(&(this->upSample_inst[i]), this->upsample_factor, this->numTaps, this->coeffs4, this->state[i], this->blockSize);
  // }

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  this->adc = new ADC();              // adc object
  this->adc->adc0->setAveraging(1);   // set number of averages
  this->adc->adc0->setResolution(12); // set bits of resolution
  this->adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED);
  this->adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED);
  // this->adc->adc0->enableInterrupts(adcIsr);

  this->adc->adc0->stopTimer();
  this->adc->adc0->startSingleRead(readPin_adc_0); // call this to setup everything before the Timer starts, differential is also possible

  delay(1);

  this->adc->adc0->readSingle();

  // now start the ADC collection timer
  this->adc->adc0->startTimer(378000); // AUDIO_SAMPLE_RATE_EXACT * inputsCount); // frequency in Hz
  this->adc->adc0->enableInterrupts(adcIsr);

  digitalWriteFast(2, bitRead(this->muxIndex, 0));
  digitalWriteFast(3, bitRead(this->muxIndex, 1));
  digitalWriteFast(4, bitRead(this->muxIndex, 2));

  // wait enough time for the first timer conversion to finish (depends on resolution and averaging),
  // with 16 averages, 12 bits, and ADC_MED_SPEED in both sampling and conversion speeds it takes about 36 us.
  delayMicroseconds(25);
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
  uint32_t t = this->tail[index];
  int16_t *result = this->queue[this->inputIndex][this->tail[index]];

  if (t == this->head[index])
  {
    return NULL;
  }

  if (++t >= maxBuffers)
  {
    t = 0;
  }

  if (t == this->head[index])
  {
    return NULL;
  }

  this->tail[index] = t;

  // if (getInstance()->inputIndex == 0)
  // {
  //   Serial.println(this->tail[index]);
  // }

  return result;

  return NULL;
  // if (this->bufferCountt[index] >= 2 && this->bufferCountt[index] < this->buffSize)
  // {
  //   // Serial.println(this->bufferCountt[index]);
  //   this->interpolate(index);
  //   // arm_fir_interpolate_q15(&(this->upSample_inst[index]), this->bufferr[index], this->interpolatedBuffer[index], this->blockSize);

  //   this->bufferCountt[index] = 0;
  //   return this->interpolatedBuffer[index];
  // }

  // // No interpolation required
  // if (this->bufferCountt[index] >= this->buffSize)
  // {
  //   this->bufferCountt[index] = 0;
  //   return this->interpolatedBuffer[index];
  // }

  // return NULL;
}

elapsedMicros debugClock;
uint32_t debugCounter = 0;
// when the measurement finishes, this will be called
// first: see which pin finished and then save the measurement into the correct buffer
inline void InputsManager::adcIsr()
{
  // if (debugCounter == 0)
  // {
  //   Serial.print("debugClock: ");
  //   Serial.println(debugClock);
  //   debugCounter++;
  // }

  debugCounter++;
  if (debugCounter >= 8 * 128)
  {
    // Serial.println(debugClock); // weird AUDIO_SAMPLE_RATE_EXACT * inputsCount gives 3023us instead of 2902, so 42kHz instea of 44.1kHz
    debugClock = 0;
    debugCounter = 0;
  }

  // if (getInstance()->inputIndex == 0)
  // {
  //   Serial.println(getInstance()->head[getInstance()->inputIndex]);
  // }

  uint32_t h;
  uint16_t adc_val = getInstance()->adc->adc0->readSingle();

  if (getInstance()->headSamplesCount[getInstance()->inputIndex] < buffSize)
  {
    getInstance()->queue[getInstance()->inputIndex][getInstance()->head[getInstance()->inputIndex]][getInstance()->headSamplesCount[getInstance()->inputIndex]] = adc_val * 16 - 32768;
    getInstance()->headSamplesCount[getInstance()->inputIndex]++;
  }

  if (getInstance()->headSamplesCount[getInstance()->inputIndex] >= buffSize)
  {
    h = getInstance()->head[getInstance()->inputIndex] + 1;

    if (h >= maxBuffers)
    {
      h = 0;
    }

    if (h == getInstance()->tail[getInstance()->inputIndex])
    {
      return;
    }

    getInstance()->head[getInstance()->inputIndex] = h;
  }
  // getInstance()->processAnalogData();
  getInstance()->iterate();
}

inline void InputsManager::processAnalogData()
{
  // uint32_t h;

  // // call readSingle() to clear the interrupt and store the ADC value
  // uint16_t adc_val = adc->adc0->readSingle();

  // this->queue[this->inputIndex][this->head[this->inputIndex]] = adc_val * 16 - 32768;

  // this->headSamplesCount[this->inputIndex] = this->headSamplesCount[this->inputIndex] + 1;

  // if (this->headSamplesCount[this->inputIndex] >= buffSize)
  // {
  //   h = this->head[this->inputIndex] + 1;

  //   if (h >= maxBuffers)
  //   {
  //     h = 0;
  //   }

  //   if (h == this->tail[this->inputIndex])
  //   {
  //     return;
  //   }

  //   this->head[this->inputIndex] = h;
  // }

  // if (this->head[this->inputIndex] < maxSamples)
  // {
  // // Save the incoming sample in the right buffer at the right index
  // this->queue[this->inputIndex][this->head[this->inputIndex]] = adc_val * 16 - 32768;

  // // Move the buffer's head for the next sample
  // this->head[this->inputIndex]++;

  // // If the buffer reached the max number of samples and it's not the last buffer,
  // // then move the head to the next buffer
  // if (this->head[this->inputIndex] >= maxSamples)
  // {
  //   this->sampleHead[this->inputIndex] = 0;
  //   this->bufferHead[this->inputIndex]++;
  // }
  // }
}

inline void InputsManager::interpolate(uint8_t index)
{
  // int16_t result[AUDIO_BLOCK_SAMPLES] = {0};

  // for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
  // {
  //   // How far along are we?
  //   double srcpos = i * (float)(this->bufferCountt[index] - 1) / (float)AUDIO_BLOCK_SAMPLES;

  //   int srcel = (int)srcpos;

  //   // Alpha/beta fraction.
  //   srcpos -= srcel;

  //   result[i] = this->bufferr[index][srcel] * (1 - srcpos) + this->bufferr[index][srcel + 1] * srcpos;
  // }

  // memcpy(this->interpolatedBuffer[index], result, sizeof(result));
}
#endif