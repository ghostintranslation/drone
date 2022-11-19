// TODO: Couldn't get good results with DMA yet but should be investigated to increase sampling frequency

#ifndef InputsManager_h
#define InputsManager_h

#include <ADC.h>
#include <ADC_util.h>
#include "arm_math.h"

/**
 * The manager for the inputs.
 * Handles the multiplexer in order to read one value and then iterate the multiplexer
 * at 44100 / 4 * 8 Hz. Fills all 8 inputs buffer that are read by the Input objects in 2.9ms as
 * required by the audio library.
 * I am getting too much crosstalks when trying a frequency of 44100 * 8 Hz which is why I reduce it.
 * By reducing it I only get 32 samples after 2.9ms, so the buffers are then interpolated to make 128 samples using the arm function.
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

  void interpolate(uint8_t index);
  static void TIM0();

  static const byte inputsCount = 8;
  static int muxIndex;
  static int inputIndex;
  static const uint32_t decimationFactor = 4;
  static const uint32_t buffSize = AUDIO_BLOCK_SAMPLES;
  static const uint32_t maxSamples = buffSize / decimationFactor;
  static const uint32_t maxBuffers = 8;
  static int16_t queue[inputsCount][maxBuffers][buffSize * 2];
  static uint16_t head[inputsCount];
  static uint16_t headQueueTempCount[inputsCount];
  static int16_t headQueueTemp[inputsCount][buffSize * 2];
  static uint16_t tail[inputsCount];
  //  uint8_t pins[4] = {A0, A1, A2, A3};
  //  uint8_t readPin_adc_0 = A0;

  ADC *adc; // adc object

  AudioRecordQueue *audioRecordQueue[inputsCount];

  //   Hardware accelerated interpolation and low pass filter
  //   Taking 32 points and turning it to 128
  //   Coeffs from https://forum.pjrc.com/threads/35951-FFT256-with-signal-decimation
  arm_fir_interpolate_instance_q15 upSample_inst[inputsCount];
  static const int16_t numTaps = 80;
  static const int16_t upsample_factor = decimationFactor;
  static const int16_t blockSize = AUDIO_BLOCK_SAMPLES / decimationFactor;
  int16_t state[inputsCount][(numTaps / upsample_factor) + blockSize - 1] = {{0}};
  int16_t coeffs4[80] = {0, -1, -2, -3, 0, 7, 16, 16, 0, -31,
                         -60, -57, 0, 96, 174, 155, 0, -239, -415, -357,
                         0, 516, 871, 731, 0, -1016, -1685, -1395, 0, 1907,
                         3159, 2625, 0, -3703, -6340, -5547, 0, 9649, 20719, 29479,
                         32743, 29300, 20468, 9474, 0, -5380, -6110, -3547, 0, 2482,
                         2967, 1779, 0, -1283, -1539, -921, 0, 652, 771, 453,
                         0, -307, -354, -202, 0, 128, 141, 77, 0, -44,
                         -46, -23, 0, 11, 11, 5, 0, -2, -1, 0};
};

// Singleton pre init
InputsManager *InputsManager::instance = nullptr;

int InputsManager::muxIndex = 0;
int InputsManager::inputIndex = 0;
int16_t InputsManager::queue[inputsCount][maxBuffers][buffSize * 2] = {{{0}}};
uint16_t InputsManager::head[inputsCount] = {0};
uint16_t InputsManager::headQueueTempCount[inputsCount] = {0};
int16_t InputsManager::headQueueTemp[inputsCount][buffSize * 2] = {{0}};
uint16_t InputsManager::tail[inputsCount] = {0};

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

  for (int i = 0; i < inputsCount; i++)
  {
    arm_fir_interpolate_init_q15(&(this->upSample_inst[i]), this->upsample_factor, this->numTaps, this->coeffs4, this->state[i], this->blockSize);
  }

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

  // PIT
  PIT_MCR = 0;                                      // Enable PIT
  PIT_LDVAL0 = 135 / ((float)1 / decimationFactor); // 135*4 = 88200Hz, 135 = 352800Hz, 1087 = 44100Hz
  PIT_TCTRL0 = PIT_TCTRL_TIE;                       // PIT Interrupt enable for Timer0
  PIT_TCTRL0 |= PIT_TCTRL_TEN;                      // start Timer0
  attachInterruptVector(IRQ_PIT, TIM0);             // attach TIM0(isr) to Timer0 PIT interrupt
  NVIC_ENABLE_IRQ(IRQ_PIT);

  digitalWriteFast(2, bitRead(this->muxIndex, 0));
  digitalWriteFast(3, bitRead(this->muxIndex, 1));
  digitalWriteFast(4, bitRead(this->muxIndex, 2));

  // wait enough time for the first timer conversion to finish (depends on resolution and averaging),
  // with 16 averages, 12 bits, and ADC_MED_SPEED in both sampling and conversion speeds it takes about 36 us.
  delayMicroseconds(25);
}

inline int16_t *InputsManager::readInput(byte index)
{
  uint32_t t = this->tail[index];
  int16_t *result = this->queue[index][this->tail[index]];

  if (++t >= maxBuffers)
  {
    t = 0;
  }

  this->tail[index] = t;

  return result;
}

inline void InputsManager::TIM0()
{

  if (headQueueTempCount[muxIndex] < maxSamples)
  {
    // Filling temp buffer with 32 samples
    uint32_t val = getInstance()->adc->adc0->analogRead(A0) * 16 - 32768; // read a new value, will return ADC_ERROR_VALUE if the comparison is false.

    headQueueTemp[muxIndex][headQueueTempCount[muxIndex]] = val;

    headQueueTempCount[muxIndex]++;
  }
  else
  {

    // If we are decimating and interpolation was not yet performed
    if (headQueueTempCount[muxIndex] < buffSize && decimationFactor > 1)
    {
      // The size could be just buffSize but just in case...
      int16_t interpolatedBuffer[buffSize * 2] = {0};

      // Interpolating temp buffer's to 128 samples
      arm_fir_interpolate_q15(&(getInstance()->upSample_inst[muxIndex]), headQueueTemp[muxIndex], interpolatedBuffer, blockSize);

      // TODO: This
      for (int i = 0; i < buffSize; i++)
      {
        headQueueTemp[muxIndex][i] = interpolatedBuffer[i];
      }
      // Or That?
      //    memcpy(queue[muxIndex][head[muxIndex]], interpolatedBuffer, sizeof(interpolatedBuffer));

      headQueueTempCount[muxIndex] = buffSize;
    }

    // Try to move head, we'll see if that works with the tail just bellow before actually doing it
    uint32_t h = head[muxIndex] + 1;

    // Ciruclar buffer, the head goes back to index 0 after reaching the max
    if (h >= maxBuffers)
    {
      h = 0;
    }

    if (h != tail[muxIndex])
    {
      // The temp buffer is full and head is in a good position to write
      // let's write the temp buffer in the queue
      memcpy(queue[muxIndex][head[muxIndex]], headQueueTemp[muxIndex], sizeof(headQueueTemp[muxIndex]));

      // Reset the temp buffer
      for (int i = 0; i < buffSize; i++)
      {
        headQueueTemp[muxIndex][i] = 0;
      }
      // Reset also the samples count of the temp buffer
      headQueueTempCount[muxIndex] = 0;

      // Moving the head
      head[muxIndex] = h;
    }
  }

  // Iterate to next input
  muxIndex++;
  muxIndex = muxIndex % 8;
  digitalWriteFast(2, bitRead(muxIndex, 0));
  digitalWriteFast(3, bitRead(muxIndex, 1));
  digitalWriteFast(4, bitRead(muxIndex, 2));

  PIT_TFLG0 |= PIT_TFLG_TIF; // to enable interrupt again
}
#endif