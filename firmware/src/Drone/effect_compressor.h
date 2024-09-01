#ifndef TEENSYAUDIO_EFFECT_COMPRESSOR_H
#define TEENSYAUDIO_EFFECT_COMPRESSOR_H

#include <Arduino.h>
#include "AudioStream.h"

const long NumlogTablePoints = 100;

class AudioEffectCompressor : public AudioStream {
public:
    AudioEffectCompressor() : AudioStream(1, inputQueueArray) { 
      buildLogTable();
    }
    
    virtual void update(void);
    
    float getInputGain();
    void setInputGain(float inputGain);
    
    bool getEnabled();
    void setEnabled(bool enabled);
    void toggleEnabled();
    
    int16_t interpolate(int16_t input);
    
private:    
    bool _enabled = true;
    float _inputGain = 1.0;
    void buildLogTable();
   
    audio_block_t *inputQueueArray[1];

    static double logtable[NumlogTablePoints];
    static bool _initialized;
};

#endif //TEENSYAUDIO_EFFECT_COMPRESSOR_H