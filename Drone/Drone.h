#ifndef Drone_h
#define Drone_h

#include <Audio.h>

#include "Motherboard.h"
#include "Voice.h"

/*
 * Drone
 */
class Drone{
  
  private:
    // Singleton
    static Drone *instance;
    Drone();

    // Motherboard
    Motherboard *device;

    // Inputs
    Input* tune1;
    Input* tune2;
    Input* tune3;
    Input* tune4;
    Input* mix;
    Input* fm;

    // Leds
    Led* led1;
    Led* led2;

    static const byte voiceCount = 4;
    Voice *voices[voiceCount];

    byte updateMillis = 10;
    elapsedMillis clockUpdate;

    AudioConnection* patchCords[4]; 
    AudioMixer4 *outputLeft;
    AudioMixer4 *outputRight;
    
  public:
    static Drone *getInstance();
    void init();
    void update();
    void setPan(byte voiceIndex, byte pan);

    // Inputs callbacks
    // static void onTuneChange(Input* input);
    static void onMixChange(Input* input); // TODO: CREATE A SPIRAL CONVERTER TO ELIMINATE THIS CALLBACK
    static void onFmChange(Input* input);

    // Audio output
    AudioMixer4 * getOutputLeft();
    AudioMixer4 * getOutputRight();
};

// Singleton pre init
Drone * Drone::instance = nullptr;

/**
 * Constructor
 */
inline Drone::Drone(){  

  this->tune1 = new Input(0);
  this->tune1->setRange(10, 1000);
  // this->tune1->setOnChange(onTuneChange);

  this->tune2 = new Input(1);
  this->tune2->setRange(10, 1000);
  // this->tune2->setOnChange(onTuneChange);

  this->tune3 = new Input(2);
  this->tune3->setRange(10, 1000);
  // this->tune3->setOnChange(onTuneChange);

  this->tune4 = new Input(3);
  this->tune4->setRange(10, 1000);
  // this->tune4->setOnChange(onTuneChange);

  this->mix = new Input(4);
  this->mix->setRange(0, 4*PI);
  
  this->fm = new Input(5);

  this->led1 = new Led(0);
  this->led2 = new Led(1);

  this->outputLeft = new AudioMixer4();
  this->outputLeft->gain(0, 1);
  this->outputLeft->gain(1, 1);
  this->outputLeft->gain(2, 1);
  this->outputLeft->gain(3, 1);
  
  this->outputRight = new AudioMixer4();
  this->outputRight->gain(0, 1);
  this->outputRight->gain(1, 1);
  this->outputRight->gain(2, 1);
  this->outputRight->gain(3, 1);
  
  for (int i = 0; i < this->voiceCount; i++) {
    this->voices[i] = new Voice();
    this->patchCords[i] = new AudioConnection(*this->voices[i]->getOutputLeft(), 0, *this->outputLeft, i);
    this->patchCords[i] = new AudioConnection(*this->voices[i]->getOutputRight(), 0, *this->outputRight, i);
  }

  new AudioConnection(*this->tune1, 0, *this->voices[0], 0);
  new AudioConnection(*this->tune2, 0, *this->voices[1], 0);
  new AudioConnection(*this->tune3, 0, *this->voices[2], 0);
  new AudioConnection(*this->tune4, 0, *this->voices[3], 0);

  // Set the pan programmatically
  this->setPan(0, 50);
  this->setPan(1, 100);
  this->setPan(2, 150);
  this->setPan(3, 200);
}

/**
 * Singleton instance
 */
inline Drone *Drone::getInstance()    {
  if (!instance)
     instance = new Drone;
  return instance;
}

/**
 * Init
 */
inline void Drone::init(){
}


/**
 * Return the audio left output
 */
inline AudioMixer4 * Drone::getOutputLeft(){
  return this->outputLeft;
}

/**
 * Return the audio right output
 */
inline AudioMixer4 * Drone::getOutputRight(){
  return this->outputRight;
}

/**
 * Update
 */
inline void Drone::update(){
  this->device->update();
}

/**
 * Set one voice's pan
 */
inline void Drone::setPan(byte voiceIndex, byte pan){
  voiceIndex = voiceIndex % 4;
  this->voices[voiceIndex]->setPan(pan);
}

/**
 * On Tune Change
 */
// inline void Drone::onTuneChange(Input* input){
//   getInstance()->voices[input->getIndex()]->setFrequency(input->getValue());
// }

/**
 * On Mix Change
 */
inline void Drone::onMixChange(Input* input){
  float voice1Gain = constrain(100*cos(input->getValue()) + pow(input->getValue(),2), 0, 100)/400;
  float voice2Gain = constrain(100*cos(input->getValue()+1.5*PI) + pow(input->getValue(),2), 0, 100)/400;
  float voice3Gain = constrain(100*cos(input->getValue()+0.5*PI) + pow(input->getValue(),2), 0, 100)/400;
  float voice4Gain = constrain(100*cos(input->getValue()+PI) + pow(input->getValue(),2), 0, 100)/400;

  getInstance()->voices[0]->setGain(voice1Gain);
  getInstance()->voices[1]->setGain(voice2Gain);
  getInstance()->voices[2]->setGain(voice3Gain);
  getInstance()->voices[3]->setGain(voice4Gain);

  getInstance()->device->setLED(0, 1, 32768 * voice1Gain * 4);
  getInstance()->device->setLED(1, 1, 32768 * voice2Gain * 4);
  getInstance()->device->setLED(2, 1, 32768 * voice3Gain * 4);
  getInstance()->device->setLED(3, 1, 32768 * voice4Gain * 4);
}

/**
 * On Fm Change
 */
inline void Drone::onFmChange(Input* input){
  int modulatorFrequency = 0;
  float modulatorAmplitude = 0;
    
  if(input->getValue() < getInstance()->device->getAnalogMaxValue() / 3){
    modulatorFrequency = map(input->getValue(), 0, getInstance()->device->getAnalogMaxValue() / 2, 0, 10);
    modulatorAmplitude = (float)map((float)input->getValue(), 0, getInstance()->device->getAnalogMaxValue() / 2, 0.001, .01);
  }
  else if(input->getValue() >= getInstance()->device->getAnalogMaxValue() / 3 && input->getValue() < getInstance()->device->getAnalogMaxValue() / 2){
    modulatorFrequency = map(input->getValue(), 0, getInstance()->device->getAnalogMaxValue() / 2, 0, 40);
    modulatorAmplitude = (float)map((float)input->getValue(), 0, getInstance()->device->getAnalogMaxValue() / 2, 0.001, .01);
  }else{
    modulatorFrequency = map(input->getValue(), getInstance()->device->getAnalogMaxValue() / 2, getInstance()->device->getAnalogMaxValue(), 0, 1000);
    modulatorAmplitude = (float)map((float)input->getValue(), getInstance()->device->getAnalogMaxValue() / 2, getInstance()->device->getAnalogMaxValue(), 0, .5);
  }
    
  for (int i = 0; i < getInstance()->voiceCount ; i++) {
    getInstance()->voices[i]->setModulatorFrequency(modulatorFrequency);
  }
  
  if(value > 50){
    for (int i = 0; i < getInstance()->voiceCount ; i++) {
      getInstance()->voices[i]->setModulatorAmplitude(modulatorAmplitude);
    }
  }else{
    for (int i = 0; i < getInstance()->voiceCount ; i++) {
      getInstance()->voices[i]->setModulatorAmplitude(0);
    }
  }
}
#endif
