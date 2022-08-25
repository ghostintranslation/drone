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
    audio_block_t *inputQueueArray[3];

    AudioSynthWaveformModulated *sineFM;
    AudioEffectMultiply *multiply;
    AudioSynthWaveformModulated *sineModulator;
    AudioRecordQueue *audioOutQueue;
    AudioPlayQueue *frequencyQueue;
    AudioPlayQueue *gainQueue;
    AudioPlayQueue *sineModulatorFrequencyQueue;
    AudioPlayQueue *sineModulatorAmplitudeQueue;
    AudioMixer4 *fmMixer;
    AudioEffectMultiply *sineModulatorMultiplier;


    int16_t frequencyData[AUDIO_BLOCK_SAMPLES];
    int16_t gainData[AUDIO_BLOCK_SAMPLES];
    int16_t sineModulatorFrequencyData[AUDIO_BLOCK_SAMPLES];
    int16_t sineModulatorAmplitudeData[AUDIO_BLOCK_SAMPLES];

//    int16_t frequency = 32767;
//    byte pan = 127;
//    int16_t modulatorFrequency = 0;
//    int16_t modulatorAmplitude = 0;
//    int16_t gain = 0;
//    unsigned int intervalGain = 200;

  public:
    Voice();
    void update(void);
//    AudioAmplifier * getOutputLeft();
//    AudioAmplifier * getOutputRight();
    // Setters
//    void setFrequency(int16_t freq);
//    void setPan(byte pan);
//    void setGain(int16_t gain);
//    void setModulatorFrequency(int16_t freq);
//    void setModulatorAmplitude(int16_t amp);
};

/**
 * Constructor
 */
inline Voice::Voice() : AudioStream(3, inputQueueArray){
  this->sineFM = new AudioSynthWaveformModulated();
  this->sineFM->begin(WAVEFORM_SINE);
  this->sineFM->frequencyModulation(2);
  this->sineFM->frequency(200);
  this->sineFM->amplitude(0.5);

  this->multiply = new AudioEffectMultiply();
  
  this->sineModulator = new AudioSynthWaveformModulated();
  this->sineModulator->begin(WAVEFORM_SINE);
  this->sineModulator->frequencyModulation(3);
  this->sineModulator->frequency(100);
  this->sineModulator->amplitude(0.5);

  this->sineModulatorMultiplier = new AudioEffectMultiply();
  
//  this->outputLeft = new AudioAmplifier();
//  this->outputLeft->gain(0.25);
//  this->outputRight = new AudioAmplifier();
//  this->outputRight->gain(0.25);
//  this->amp = new AudioSmoothAmplifier();

  
  this->audioOutQueue = new AudioRecordQueue();
  this->audioOutQueue->begin();
  
  this->frequencyQueue = new AudioPlayQueue();
  
  this->gainQueue = new AudioPlayQueue();
  
  this->sineModulatorFrequencyQueue = new AudioPlayQueue();

  this->sineModulatorAmplitudeQueue = new AudioPlayQueue();

  this->fmMixer = new AudioMixer4();
  this->fmMixer->gain(0, 1);
  this->fmMixer->gain(1, 1);

//new AudioConnection(*this->sineModulator, 0, *this->sineFM, 0);
//  this->patchCords[1] = new AudioConnection(*this->sineFM, 0, *this->amp, 0);
//  this->patchCords[2] = new AudioConnection(*this->amp, 0, *this->outputLeft, 0);
//  this->patchCords[3] = new AudioConnection(*this->amp, 0, *this->outputRight, 0);
//new AudioConnection(*this->sineFM, 0, *this->outputLeft, 0);


new AudioConnection(*this->sineModulatorFrequencyQueue, 0, *this->sineModulator, 0);
new AudioConnection(*this->sineModulator, 0, *this->sineModulatorMultiplier, 0);
new AudioConnection(*this->sineModulatorAmplitudeQueue, 0, *this->sineModulatorMultiplier, 1);
//new AudioConnection(*this->sineModulatorMultiplier, *this->sineModulator);
new AudioConnection(*this->sineModulatorMultiplier, 0, *this->fmMixer, 0);
new AudioConnection(*this->frequencyQueue, 0, *this->fmMixer, 1);
new AudioConnection(*this->fmMixer, *this->sineFM);
new AudioConnection(*this->sineFM, *this->multiply);
new AudioConnection(*this->gainQueue, 0, *this->multiply, 1);
new AudioConnection(*this->multiply, *this->audioOutQueue);
  
//  this->setFrequency(this->frequency);
//  this->setGain(-30000);
}

inline void Voice::update(void) {
  // Receive input data
  audio_block_t *freqBlock;
  freqBlock = receiveReadOnly(0);
  audio_block_t *gainBlock;
  gainBlock = receiveReadOnly(1);
  audio_block_t *fmBlock;
  fmBlock = receiveReadOnly(2);


  if (freqBlock){
//    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
//      // Aproximated moving average
//      this->frequency -= this->frequency / AUDIO_BLOCK_SAMPLES;
//      this->frequency += freqBlock->data[i] / AUDIO_BLOCK_SAMPLES;
//    }
//
//    this->setFrequency(this->frequency);

//    audio_block_t *newFreqBlock = allocate();
//    if(newFreqBlock){
//      for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
//       newFreqBlock->data[i] = 2000;//(freqBlock->data[i] + 32768) / 2;
//      }
//      transmit(newFreqBlock, 1);
//      release(newFreqBlock);
//    }

    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
     this->frequencyData[i] = freqBlock->data[i];
    }

    release(freqBlock);
  }
  this->frequencyQueue->play(this->frequencyData, AUDIO_BLOCK_SAMPLES);


  if (gainBlock){
//    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
//      // Aproximated moving average
//      this->gain -= this->gain / AUDIO_BLOCK_SAMPLES;
//      this->gain += gainBlock->data[i] / AUDIO_BLOCK_SAMPLES;
//    }
//
//    this->setGain(this->gain);
////    this->setGain(1);
//  

    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
     this->gainData[i] = (gainBlock->data[i] + 32768) / 2; // Negative values reverse the signal, we don't want that.
    }

    release(gainBlock);
  }
  this->gainQueue->play(this->gainData, AUDIO_BLOCK_SAMPLES);

  if (fmBlock){
//    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
//      // Aproximated moving average
//      this->modulatorFrequency -= this->modulatorFrequency / AUDIO_BLOCK_SAMPLES;
//      this->modulatorFrequency += fmBlock->data[i] / AUDIO_BLOCK_SAMPLES;
//    }
//
//    this->setModulatorFrequency(this->modulatorFrequency);
//    this->setModulatorAmplitude(this->modulatorFrequency);
//  
    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
     this->sineModulatorFrequencyData[i] = fmBlock->data[i];
    }
    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
      this->sineModulatorAmplitudeData[i] = (fmBlock->data[i] + 32768) / 2 - 1000; // Negative values reverse the signal, we don't want that.
      this->sineModulatorAmplitudeData[i] = this->sineModulatorAmplitudeData[i] > 0 ? this->sineModulatorAmplitudeData[i] : 0;
    }
    Serial.println(this->sineModulatorAmplitudeData[0]);
    release(fmBlock);
  }
  this->sineModulatorFrequencyQueue->play(this->sineModulatorFrequencyData, AUDIO_BLOCK_SAMPLES);
  this->sineModulatorAmplitudeQueue->play(this->sineModulatorAmplitudeData, AUDIO_BLOCK_SAMPLES);


  if(this->audioOutQueue->available()){
    while(this->audioOutQueue->available()){ 
      audio_block_t *audioBlock = allocate();
      if(audioBlock){
        int16_t *queueData = this->audioOutQueue->readBuffer();
        for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
         audioBlock->data[i] = queueData[i];
        }
        transmit(audioBlock, 0);
        release(audioBlock);
        this->audioOutQueue->freeBuffer();
      }
    }
  }
}

/**
 * Set the modulator frequency
 */
//inline void Voice::setModulatorFrequency(int16_t frequency){
//  this->modulatorFrequency = frequency;
////  this->sineModulator->frequency(map(this->modulatorFrequency, -32768, 32767, 10, 1000));
//}

/**
 * Set the modulator amplitude
 */
//inline void Voice::setModulatorAmplitude(int16_t amplitude){
//  this->modulatorAmplitude = amplitude;
////  this->sineModulator->amplitude(map((float)this->modulatorAmplitude, -32768, 32767, 0, .5));
//}

/**
 * Return the audio left output
 */
//inline AudioAmplifier * Voice::getOutputLeft(){
//  return this->outputLeft;
//}

/**
 * Return the audio right output
 */
//inline AudioAmplifier * Voice::getOutputRight(){
//  return this->outputRight;
//}

/**
 * Set the frequency
 */
//inline void Voice::setFrequency(int16_t frequency){
//  this->frequency = frequency;
//  this->sineFM->frequency(map(this->frequency, -32768, 32767, 10, 1000));
//}

/**
 * Set the pan
 */
//inline void Voice::setPan(byte pan){
//  this->pan = pan;
//  this->outputLeft->gain((float)map((float)this->pan, 0, 255, (float)1, (float)0));
//  this->outputRight->gain((float)map((float)this->pan, 0, 255, (float)0, (float)1));
//}
//
//inline void Voice::setGain(int16_t gain){
//  this->gain = gain;
////  this->amp->gain((float)map((float)this->gain, -32768, 32767, 0, 1), 0.2);
//}
#endif
