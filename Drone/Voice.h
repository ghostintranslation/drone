#ifndef Voice_h
#define Voice_h

#include <Audio.h>
#include "AudioSmoothAmplifier.h"

/*
 * Voice
 */
class Voice : public AudioStream {
  private:
    audio_block_t *inputQueueArray[3];

    AudioSynthWaveformModulated *sineFM;
    AudioSmoothAmplifier *amplifier;
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
    
  public:
    Voice();
    void update(void);
};

/**
 * Constructor
 */
inline Voice::Voice() : AudioStream(3, inputQueueArray){
  this->sineFM = new AudioSynthWaveformModulated();
  this->sineFM->begin(WAVEFORM_SINE);
  this->sineFM->frequencyModulation(12);
  this->sineFM->frequency(0.3);
  this->sineFM->amplitude(1);

  this->amplifier = new AudioSmoothAmplifier();
  
  this->sineModulator = new AudioSynthWaveformModulated();
  this->sineModulator->begin(WAVEFORM_SINE);
  this->sineModulator->frequencyModulation(12);
  this->sineModulator->frequency(100);
  this->sineModulator->amplitude(1);

  this->sineModulatorMultiplier = new AudioEffectMultiply();

  this->audioOutQueue = new AudioRecordQueue();
  this->audioOutQueue->begin();
  
  this->frequencyQueue = new AudioPlayQueue();
  
  this->gainQueue = new AudioPlayQueue();
  
  this->sineModulatorFrequencyQueue = new AudioPlayQueue();

  this->sineModulatorAmplitudeQueue = new AudioPlayQueue();

  this->fmMixer = new AudioMixer4();
  this->fmMixer->gain(0, 1);
  this->fmMixer->gain(1, 1);

  new AudioConnection(*this->sineModulatorFrequencyQueue, 0, *this->sineModulator, 0);
  new AudioConnection(*this->sineModulator, 0, *this->sineModulatorMultiplier, 0);
  new AudioConnection(*this->sineModulatorAmplitudeQueue, 0, *this->sineModulatorMultiplier, 1);
  new AudioConnection(*this->sineModulatorMultiplier, 0, *this->fmMixer, 0);
  new AudioConnection(*this->frequencyQueue, 0, *this->fmMixer, 1);
  new AudioConnection(*this->fmMixer, *this->sineFM);
  new AudioConnection(*this->sineFM, *this->amplifier);
  new AudioConnection(*this->gainQueue, 0, *this->amplifier, 1);
  new AudioConnection(*this->amplifier, *this->audioOutQueue);
}

inline void Voice::update(void) {
  // Receive input data
  audio_block_t *freqBlock;
  audio_block_t *gainBlock;
  audio_block_t *fmBlock;

  // Frequency block
  // Receiving data from input of the block, and passing it to the queue which is connected to the other objects
  freqBlock = receiveReadOnly(0);
  
  if (freqBlock){
    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
      int32_t val = freqBlock->data[i] + 32668; // between 0 and 65335
      if(val<1) val = 1; // between 1 and 65335
      val = log(val)*65335/11.5 - 32668;
      if(val>32667) val = 32667;
       this->frequencyData[i] = val;
      }
//      Serial.println(this->frequencyData[0]);

    release(freqBlock);
  }
  
  this->frequencyQueue->play(this->frequencyData, AUDIO_BLOCK_SAMPLES);

  // Gain block
  // Receiving data from input of the block, and passing it to the queue which is connected to the other objects
  gainBlock = receiveReadOnly(1);
  
  if (gainBlock){
    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
     this->gainData[i] = (gainBlock->data[i] + 32768) / 2; // Negative values reverse the signal, we don't want that.
    }

    release(gainBlock);
  }
  
  this->gainQueue->play(this->gainData, AUDIO_BLOCK_SAMPLES);

  // FM block
  // Receiving data from input of the block, and passing it to the queue which is connected to the other objects
  fmBlock = receiveReadOnly(2);
  
  if (fmBlock){
    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
     this->sineModulatorFrequencyData[i] = fmBlock->data[i];
    }
    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
      this->sineModulatorAmplitudeData[i] = (fmBlock->data[i] + 32768) / 2 - 1000; // Negative values reverse the signal, we don't want that.
      this->sineModulatorAmplitudeData[i] = this->sineModulatorAmplitudeData[i] > 0 ? this->sineModulatorAmplitudeData[i] : 0;
    }
    release(fmBlock);
  }
  
  this->sineModulatorFrequencyQueue->play(this->sineModulatorFrequencyData, AUDIO_BLOCK_SAMPLES);
  this->sineModulatorAmplitudeQueue->play(this->sineModulatorAmplitudeData, AUDIO_BLOCK_SAMPLES);

  // Transmitting the audio queue
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
#endif
