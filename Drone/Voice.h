#ifndef Voice_h
#define Voice_h

#include <Audio.h>

/*
// GUItool: begin automatically generated code
AudioSynthWaveformSine   sineModulator;  //xy=141,95
AudioSynthWaveformModulated sineFM;         //xy=298,102
AudioMixer4              outputLeft;         //xy=447,122
AudioMixer4              outputRight;         //xy=453,191
AudioOutputI2S           is2; //xy=613,148
AudioConnection          patchCord1(sineModulator, 0, sineFM, 0);
AudioConnection          patchCord2(sineFM, 0, outputLeft, 0);
AudioConnection          patchCord3(sineFM, 0, outputRight, 0);
AudioConnection          patchCord4(outputLeft, 0, is2, 0);
AudioConnection          patchCord5(outputRight, 0, is2, 1);
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
    AudioConnection* patchCords[3];
    // Output
    AudioMixer4 *outputLeft;
    AudioMixer4 *outputRight;

    unsigned int frequency = 0;
    byte pan = 127;
    unsigned int modulatorFrequency = 0;
    float modulatorAmplitude = 0;
    byte intervalGlide = 200;
    float gain = 0;
    unsigned int intervalGain = 200;
    byte updateMillis = 0;

  public:
    Voice();
    void update();
    AudioMixer4 * getOutputLeft();
    AudioMixer4 * getOutputRight();
    // Setters
    void setFrequency(int freq);
    void setPan(byte pan);
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
  this->outputLeft = new AudioMixer4();
  this->outputLeft->gain(0, 0);
  this->outputRight= new AudioMixer4();
  this->outputRight->gain(0, 0);

  this->patchCords[0] = new AudioConnection(*this->sineModulator, 0, *this->sineFM, 0);
  this->patchCords[1] = new AudioConnection(*this->sineFM, 0, *this->outputLeft, 0);
  this->patchCords[2] = new AudioConnection(*this->sineFM, 0, *this->outputRight, 0);
}

/**
 * Set the modulator frequency
 */
inline void Voice::setModulatorFrequency(int frequency){
  this->modulatorFrequency = frequency;
}

/**
 * Set the modulator amplitude
 */
inline void Voice::setModulatorAmplitude(float amplitude){
  this->modulatorAmplitude = amplitude;
}

/**
 * Return the audio left output
 */
inline AudioMixer4 * Voice::getOutputLeft(){
  return this->outputLeft;
}

/**
 * Return the audio right output
 */
inline AudioMixer4 * Voice::getOutputRight(){
  return this->outputRight;
}

/**
 * Set the frequency
 */
inline void Voice::setFrequency(int frequency){
  this->frequency = frequency;
}

/**
 * Set the pan
 */
inline void Voice::setPan(byte pan){
  this->pan = pan;
}

inline void Voice::update(){
  this->sineModulator->frequency(this->modulatorFrequency);
  this->sineModulator->amplitude(this->modulatorAmplitude);
  this->sineFM->frequency(this->frequency);
  this->outputLeft->gain(0, (float)map((float)this->pan, 0, 255, (float)1, (float)0) * this->gain);
  this->outputRight->gain(0, (float)map((float)this->pan, 0, 255, (float)0, (float)1) * this->gain);
}

inline void Voice::setUpdateMillis(byte updateMillis){
  this->updateMillis = updateMillis;
}

inline void Voice::setGain(float gain){
  this->gain = gain;
}
#endif
