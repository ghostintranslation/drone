#ifndef Voice_h
#define Voice_h

#include <Audio.h>
#include "AudioSmoothAmplifier.h"

/*
// GUItool: begin automatically generated code
AudioSynthWaveformSine   sineModulator;  //xy=92,102
AudioSynthWaveformModulated sineFM;         //xy=243,108
AudioAmplifier           amp;           //xy=396,116
AudioAmplifier           ampLeft;           //xy=590,110
AudioAmplifier           ampRight;           //xy=590,151
AudioOutputI2S           is2; //xy=727,140
AudioConnection          patchCord1(sineModulator, 0, sineFM, 0);
AudioConnection          patchCord2(sineFM, amp);
AudioConnection          patchCord3(amp, ampLeft);
AudioConnection          patchCord4(amp, ampRight);
AudioConnection          patchCord5(ampLeft, 0, is2, 0);
AudioConnection          patchCord6(ampRight, 0, is2, 1);
// GUItool: end automatically generated code
*/


/*
 * Voice
 */
class Voice : public AudioStream {
  private:
    audio_block_t *inputQueueArray[2];

    // FM
    AudioSynthWaveformModulated *sineFM;
    AudioSynthWaveformSine   *sineModulator;
    AudioSmoothAmplifier * amp; // This custom amplifier allows for gradual noiseless gain change
    // Common
    AudioConnection* patchCords[5];
    // Output
    AudioAmplifier *outputLeft;
    AudioAmplifier *outputRight;

    uint16_t frequency = 0;
    byte pan = 127;
    uint16_t modulatorFrequency = 0;
    uint16_t modulatorAmplitude = 0;
    uint16_t gain = 0;
    unsigned int intervalGain = 200;

  public:
    Voice();
    void update(void);
    AudioAmplifier * getOutputLeft();
    AudioAmplifier * getOutputRight();
    // Setters
    void setFrequency(uint16_t freq);
    void setPan(byte pan);
    void setGain(uint16_t gain);
    void setModulatorFrequency(int freq);
    void setModulatorAmplitude(float amp);
};

/**
 * Constructor
 */
inline Voice::Voice() : AudioStream(2, inputQueueArray){
  // FM
  this->sineFM = new AudioSynthWaveformModulated();
  this->sineFM->begin(WAVEFORM_SINE);
  this->sineFM->amplitude(1);
  this->sineModulator = new AudioSynthWaveformSine();
  this->outputLeft = new AudioAmplifier();
  this->outputLeft->gain(1);
  this->outputRight = new AudioAmplifier();
  this->outputRight->gain(1);
  this->amp = new AudioSmoothAmplifier();

  this->patchCords[0] = new AudioConnection(*this->sineModulator, 0, *this->sineFM, 0);
  this->patchCords[1] = new AudioConnection(*this->sineFM, 0, *this->amp, 0);
  this->patchCords[2] = new AudioConnection(*this->amp, 0, *this->outputLeft, 0);
  this->patchCords[3] = new AudioConnection(*this->amp, 0, *this->outputRight, 0);
}

inline void Voice::update(void) {
  IO::update();

  // Receive input data
  audio_block_t *freqBlock;
  freqBlock = receiveReadOnly(0);
  audio_block_t *ampBlock;
  ampBlock = receiveReadOnly(1);
  
  if (freqBlock){
    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
      int16_t x = freqBlock->data[i];
      
      // Cutting off negative values
      if(x < 0){
        x = 0;
      }
  
      // Aproximated moving average
      this->frequency -= this->frequency / AUDIO_BLOCK_SAMPLES;
      this->frequency += x / AUDIO_BLOCK_SAMPLES;
    }

    this->setFrequency(this->frequency);
  
    release(freqBlock);
  }


  if (ampBlock){
    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
      int16_t x = ampBlock->data[i];
      
      // Cutting off negative values
      if(x < 0){
        x = 0;
      }
  
      // Aproximated moving average
      this->gain -= this->gain / AUDIO_BLOCK_SAMPLES;
      this->gain += x / AUDIO_BLOCK_SAMPLES;
    }

    this->setGain(this->gain);
  
    release(ampBlock);
  }
}

/**
 * Set the modulator frequency
 */
inline void Voice::setModulatorFrequency(int frequency){
  this->modulatorFrequency = frequency;
  this->sineModulator->frequency(this->modulatorFrequency);
}

/**
 * Set the modulator amplitude
 */
inline void Voice::setModulatorAmplitude(float amplitude){
  this->modulatorAmplitude = amplitude;
  this->sineModulator->amplitude(this->modulatorAmplitude);
}

/**
 * Return the audio left output
 */
inline AudioAmplifier * Voice::getOutputLeft(){
  return this->outputLeft;
}

/**
 * Return the audio right output
 */
inline AudioAmplifier * Voice::getOutputRight(){
  return this->outputRight;
}

/**
 * Set the frequency
 */
inline void Voice::setFrequency(uint16_t frequency){
  this->frequency = frequency;
  this->sineFM->frequency(map(this->frequency, 0, 32767, 10, 1000));
}

/**
 * Set the pan
 */
inline void Voice::setPan(byte pan){
  this->pan = pan;
  this->outputLeft->gain((float)map((float)this->pan, 0, 255, (float)1, (float)0));
  this->outputRight->gain((float)map((float)this->pan, 0, 255, (float)0, (float)1));
}

inline void Voice::setGain(uint16_t gain){
  this->gain = gain;
  this->amp->gain((float)map((float)this->gain, 0, 32767, 0, 1));
}
#endif
