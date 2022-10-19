#ifndef audiosmoothamplifer_h_
#define audiosmoothamplifer_h_

#include "Arduino.h"
#include "AudioStream.h"
#include "utility/dspinst.h"

class AudioSmoothAmplifier : public AudioStream
{
  public:
    AudioSmoothAmplifier(void) : AudioStream(2, inputQueueArray), multiplierTarget(65536) {
    }
    virtual void update(void);
    void gain(float n, float s) {
      if (n > 32767.0f) n = 32767.0f;
      else if (n < -32767.0f) n = -32767.0f;
      multiplierTarget = n * 65536.0f;
      smoothness = map((float)constrain(s, 0, 1), 0, 1, 1, 0.01);
    }
  private:
    int32_t multiplierTarget;
    float multiplier = 0;
    audio_block_t *inputQueueArray[2];
    float smoothness = 1;
    void applyGain(int16_t *data);
};



#if defined(__ARM_ARCH_7EM__)
#define MULTI_UNITYGAIN 65536

inline void AudioSmoothAmplifier::applyGain(int16_t *data)
{
  uint32_t *p = (uint32_t *)data;
  const uint32_t *end = (uint32_t *)(data + AUDIO_BLOCK_SAMPLES);

  do {
    multiplier += (smoothness * (multiplierTarget - multiplier)) / 1000;
    uint32_t tmp32 = *p; // read 2 samples from *data
    int32_t val1 = signed_multiply_32x16b(multiplier, tmp32);
    int32_t val2 = signed_multiply_32x16t(multiplier, tmp32);
    val1 = signed_saturate_rshift(val1, 16, 0);
    val2 = signed_saturate_rshift(val2, 16, 0);
    *p++ = pack_16b_16b(val2, val1);
  } while (p < end);
}

#elif defined(KINETISL)
#define MULTI_UNITYGAIN 256

inline void AudioSmoothAmplifier::applyGain(int16_t *data)
{
  const int16_t *end = data + AUDIO_BLOCK_SAMPLES;

  do {
    multiplier += (smoothness * (multiplierTarget - multiplier)) / 1000;
    
    int32_t val = *data * multiplier;
    *data++ = signed_saturate_rshift(val, 16, 0);
  } while (data < end);
}

#endif

inline void AudioSmoothAmplifier::update(void)
{
  audio_block_t *block, *gainBlock;

  gainBlock = receiveReadOnly(1);
  
  if (gainBlock){
    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
      // Aproximated moving average
      this->multiplierTarget -= this->multiplierTarget / AUDIO_BLOCK_SAMPLES;
      this->multiplierTarget += gainBlock->data[i] / AUDIO_BLOCK_SAMPLES;
    }
//    Serial.println(this->multiplierTarget);
    release(gainBlock);
  }

  if (multiplier == 0 && multiplierTarget == 0) {
    // zero gain, discard any input and transmit nothing
    block = receiveReadOnly(0);
    if (block) release(block);
  } else if (multiplier == MULTI_UNITYGAIN && multiplierTarget == MULTI_UNITYGAIN) {
    // unity gain, pass input to output without any change
    block = receiveReadOnly(0);
    if (block) {
      transmit(block);
      release(block);
    }
  } else {
    // apply gain to signal
    block = receiveWritable(0);
    if (block) {
      applyGain(block->data);
      transmit(block);
      release(block);
    }
  }
}

#endif
