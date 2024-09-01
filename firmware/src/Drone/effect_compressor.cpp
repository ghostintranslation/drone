//
// Created by Nicholas Newdigate on 17/06/2018.
//

#include "effect_compressor.h"

bool AudioEffectCompressor::_initialized  = false; 
double AudioEffectCompressor::logtable[NumlogTablePoints];

void AudioEffectCompressor::buildLogTable() {
  
  if (! _initialized) {
    for (int i=0; i<NumlogTablePoints;i++) {
      logtable[i] = 0x7FFF * log10((9.0 * ((float)i)/NumlogTablePoints) + 1);
    }
    _initialized = true;
  }
  
}

int16_t AudioEffectCompressor::interpolate(int16_t input) {

    bool isNegative = input < 0;
    
    double fPoint = (NumlogTablePoints-1) * abs(input) / double(0x7FFF);
    int point0 = floor(fPoint);
    int point1 = point0 + 1;
    double amountPoint1 = point1 - fPoint;
    double amountPoint2 = 1 - amountPoint1;
    double p0 = logtable[point0];
    double p1 = logtable[point1];

    int16_t result1 = (amountPoint1 * p0) + (amountPoint2 * p1);
    return isNegative? -result1 : result1;
}

void AudioEffectCompressor::update(void)
{
    audio_block_t *blocka;
    int16_t *pa;
    int16_t a1;

    blocka = receiveWritable(0);
    if (!blocka) {
        return;
    }
    pa = (int16_t *)(blocka->data);

    if (_enabled) {
      for (uint32_t n = 0; n < AUDIO_BLOCK_SAMPLES; n++) {
          a1         = pa[n];
          double d = a1 * _inputGain;
          a1 = round(d);
          pa[n] = interpolate(a1); 
      }
    }
    transmit(blocka);
    release(blocka);
}

void AudioEffectCompressor::setInputGain(float inputGain) {
    _inputGain = inputGain;
}

float AudioEffectCompressor::getInputGain() {
    return _inputGain;
}

bool AudioEffectCompressor::getEnabled() {
    return _enabled;
}

void AudioEffectCompressor::setEnabled(bool enabled) {
    _enabled = enabled;
}

void AudioEffectCompressor::toggleEnabled() {
    _enabled = !_enabled;
}