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

    unsigned int freq = 0;
    unsigned int frequencyTarget = 0;
    byte pan = 0;
    byte panTarget = 0;
    unsigned int modulatorFreq = 0;
    unsigned int modulatorFrequencyTarget = 0;
    float modulatorAmp = 0;
    float modulatorAmplitudeTarget = 0;
    byte intervalGlide = 200;
    float gain = 0;
    float gainTarget = 0;
    unsigned int intervalGain = 200;
    byte updateMillis = 0;

  public:
    Voice();
    void update();
    AudioMixer4 * getOutputLeft();
    AudioMixer4 * getOutputRight();
    // Setters
    void setFrequency(int targetFreq);
    void setPan(byte targetPan);
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
 * Set the modulaor frequency
 */
inline void Voice::setModulatorFrequency(int frequencyTarget){
  this->modulatorFrequencyTarget = frequencyTarget;
}
inline void Voice::setModulatorAmplitude(float amplitudeTarget){
  this->modulatorAmplitudeTarget = amplitudeTarget;
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
inline void Voice::setFrequency(int frequencyTarget){
  this->frequencyTarget = frequencyTarget;
}

/**
 * Set the pan
 */
inline void Voice::setPan(byte panTarget){
  this->panTarget = panTarget;
}

inline void Voice::update(){
  if(this->frequencyTarget > this->freq){
    this->freq += (this->frequencyTarget - this->freq) / ((float)this->intervalGlide / (float)updateMillis);
  }else{
    this->freq -= (this->freq - this->frequencyTarget) / ((float)this->intervalGlide / (float)updateMillis);
  }

  if(this->panTarget > this->pan){
    this->pan += (this->panTarget - this->pan) / ((float)this->intervalGlide / (float)updateMillis);
  }else{
    this->pan -= (this->pan - this->panTarget) / ((float)this->intervalGlide / (float)updateMillis);
  }

  if(this->gainTarget > this->gain){
    this->gain += (this->gainTarget - this->gain) / ((float)this->intervalGain / (float)updateMillis);
  }else{
    this->gain -= (this->gain - this->gainTarget) / ((float)this->intervalGain / (float)updateMillis);
  }
  
  if(this->modulatorFrequencyTarget > this->modulatorFreq){
    this->modulatorFreq += (this->modulatorFrequencyTarget - this->modulatorFreq) / ((float)this->intervalGlide / (float)updateMillis);
  }else{
    this->modulatorFreq -= (this->modulatorFreq - this->modulatorFrequencyTarget) / ((float)this->intervalGlide / (float)updateMillis);
  }

  if(this->modulatorAmplitudeTarget > this->modulatorAmp){
    this->modulatorAmp += (this->modulatorAmplitudeTarget - this->modulatorAmp) / ((float)this->intervalGlide / (float)updateMillis);
  }else{
    this->modulatorAmp -= (this->modulatorAmp - this->modulatorAmplitudeTarget) / ((float)this->intervalGlide / (float)updateMillis);
  }
  
  this->sineModulator->frequency(this->modulatorFreq);
  this->sineModulator->amplitude(this->modulatorAmp);
  this->sineFM->frequency(this->freq);
  this->outputLeft->gain(0, (float)map((float)this->pan, 0, 255, (float)1, (float)0) * this->gain);
  this->outputRight->gain(0, (float)map((float)this->pan, 0, 255, (float)0, (float)1) * this->gain);
}

inline void Voice::setUpdateMillis(byte updateMillis){
  this->updateMillis = updateMillis;
}

inline void Voice::setGain(float gainTarget){
  this->gainTarget = gainTarget;
}
#endif
