#ifndef Voice_h
#define Voice_h

#include <Audio.h>

/*
// GUItool: begin automatically generated code
AudioSynthWaveformSine   sineModulator;  //xy=108,70
AudioSynthWaveformModulated sineFM;         //xy=265,77
AudioMixer4              output;         //xy=414,97
AudioOutputI2S           i2s2;           //xy=555,96
AudioConnection          patchCord1(sineModulator, 0, sineFM, 0);
AudioConnection          patchCord2(sineFM, 0, output, 0);
AudioConnection          patchCord3(output, 0, i2s2, 0);
AudioConnection          patchCord4(output, 0, i2s2, 1);
// GUItool: end automatically generated code
*/


/*
 * Voice
 */
class Voice{
  private:
    // FM
    AudioSynthWaveformModulated *sineFM;
    AudioSynthWaveformSine   *sineModulator;
    // Common
    AudioConnection* patchCords[2];
    // Output
    AudioMixer4 *output;

    unsigned int freq = 0;
    unsigned int frequencyTarget = 0;
    byte intervalGlide = 200;
    float gain = 0;
    float gainTarget = 0;
    unsigned int intervalGain = 200;
    byte updateMillis = 0;

  public:
    Voice();
    void update();
    AudioMixer4 * getOutput();
    // Setters
    void setFrequency(int targetFreq);
    void setGain(float gain);
    void setModulatorFrequency(int freq);
    void setModulatorAmplitude(float amp);
    void setUpdateMillis(byte updateMillis);
};

/**
 * Constructor
 */
inline Voice::Voice(){
  // FM
  this->sineFM = new AudioSynthWaveformModulated();
  this->sineFM->begin(WAVEFORM_SINE);
  this->sineFM->amplitude(1);
  this->sineModulator = new AudioSynthWaveformSine();
  this->output = new AudioMixer4();
  this->output->gain(0, 0);

  this->patchCords[0] = new AudioConnection(*this->sineModulator, 0, *this->sineFM, 0);
  this->patchCords[1] = new AudioConnection(*this->sineFM, 0, *this->output, 0);
}

/**
 * Set the modulaor frequency
 */
inline void Voice::setModulatorFrequency(int freq){
  this->sineModulator->frequency(freq);
}
inline void Voice::setModulatorAmplitude(float amp){
  this->sineModulator->amplitude(amp);
}

/**
 * Return the audio output
 */
inline AudioMixer4 * Voice::getOutput(){
  return this->output;
}

/**
 * Set the frequency
 */
inline void Voice::setFrequency(int frequencyTarget){
  this->frequencyTarget = frequencyTarget;
}

inline void Voice::update(){
  if(this->frequencyTarget > this->freq){
    this->freq += (this->frequencyTarget - this->freq) / ((float)this->intervalGlide / (float)updateMillis);
  }else{
    this->freq -= (this->freq - this->frequencyTarget) / ((float)this->intervalGlide / (float)updateMillis);
  }

  if(this->gainTarget > this->gain){
    this->gain += (this->gainTarget - this->gain) / ((float)this->intervalGain / (float)updateMillis);
  }else{
    this->gain -= (this->gain - this->gainTarget) / ((float)this->intervalGain / (float)updateMillis);
  }
  
  
  this->sineFM->frequency(this->freq);
  this->output->gain(0, this->gain);
}

inline void Voice::setUpdateMillis(byte updateMillis){
  this->updateMillis = updateMillis;
}

inline void Voice::setGain(float gainTarget){
  this->gainTarget = gainTarget;
}
#endif
