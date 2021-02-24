#ifndef Drone_h
#define Drone_h

#include <Audio.h>

#include "Motherboard6.h"
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
    Motherboard6 *device;
    
    static const byte voiceCount = 4;
    Voice *voices[voiceCount];

    byte updateMillis = 10;
    elapsedMillis clockUpdate;

    AudioConnection* patchCords[4]; 
    AudioMixer4 *output;
    
  public:
    static Drone *getInstance();
    void init();
    void update();
    static void onTuneChange(byte inputIndex, unsigned int value, int diffToPrevious);
    static void onMixChange(byte inputIndex, unsigned int value, int diffToPrevious);
    static void onFmChange(byte inputIndex, unsigned int value, int diffToPrevious);
    static void onMidiMixChange(byte value);
    AudioMixer4 * getOutput();

};

// Singleton pre init
Drone * Drone::instance = nullptr;

/**
 * Constructor
 */
inline Drone::Drone(){

  this->device = Motherboard6::getInstance();
  
  this->output = new AudioMixer4();
  this->output->gain(0, 1 );
  this->output->gain(1, 1 );
  this->output->gain(2, 1 );
  this->output->gain(3, 1 );
  
  for (int i = 0; i < this->voiceCount; i++) {
    this->voices[i] = new Voice();
    this->patchCords[i] = new AudioConnection(*this->voices[i]->getOutput(), 0, *this->output, i);
  }
  
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
  // 0 = empty, 1 = button, 2 = potentiometer, 3 = encoder
  device->init(
    "Drone",
    {
      Potentiometer, Potentiometer,
      Potentiometer, Potentiometer,
      Potentiometer, Potentiometer
    }
  );

  // Device callbacks
  this->device->setHandlePotentiometerChange(0, onTuneChange);
  this->device->setHandlePotentiometerChange(1, onTuneChange);
  this->device->setHandlePotentiometerChange(2, onTuneChange);
  this->device->setHandlePotentiometerChange(3, onTuneChange);
  this->device->setHandlePotentiometerChange(4, onMixChange);
  this->device->setHandlePotentiometerChange(5, onFmChange);

  // MIDI callbacks
  device->setHandleMidiControlChange(0, 0, "Mix", onMidiMixChange);
}


/**
 * Return the audio output
 */
inline AudioMixer4 * Drone::getOutput(){
  return this->output;
}


/**
 * Update
 */
inline void Drone::update(){
  this->device->update();
}

/**
 * On Tune Change
 */
inline void Drone::onTuneChange(byte inputIndex, unsigned int value, int diffToPrevious){
  unsigned int freq = (float)map(
    (float)value, 
    getInstance()->device->getAnalogMinValue(), 
    getInstance()->device->getAnalogMaxValue(),
    10,
    1000
  );
  
  getInstance()->voices[inputIndex]->setFrequency(freq);
}

/**
 * On Mix Change
 */
inline void Drone::onMixChange(byte inputIndex, unsigned int value, int diffToPrevious){
  float mix = (float)map(
    (float)value, 
    getInstance()->device->getAnalogMinValue(), 
    getInstance()->device->getAnalogMaxValue(),
    0,
    4*PI
  );
  
  float voice1Gain = constrain(100*cos(mix) + pow(mix,2), 0, 100);
  float voice2Gain = constrain(100*cos(mix+1.5*PI) + pow(mix,2), 0, 100);
  float voice3Gain = constrain(100*cos(mix+0.5*PI) + pow(mix,2), 0, 100);
  float voice4Gain = constrain(100*cos(mix+PI) + pow(mix,2), 0, 100);

  getInstance()->output->gain(0, voice1Gain/400);
  getInstance()->output->gain(1, voice2Gain/400);
  getInstance()->output->gain(2, voice3Gain/400);
  getInstance()->output->gain(3, voice4Gain/400);
      
  getInstance()->device->setLED(0, 1, 2.5 * voice1Gain);
  getInstance()->device->setLED(1, 1, 2.5 * voice2Gain);
  getInstance()->device->setLED(2, 1, 2.5 * voice3Gain);
  getInstance()->device->setLED(3, 1, 2.5 * voice4Gain);
}

/**
 * On Mix Change
 */
inline void Drone::onMidiMixChange(byte value){
    unsigned int mapValue = map(
    value, 
    0,
    255,
    getInstance()->device->getAnalogMinValue(), 
    getInstance()->device->getAnalogMaxValue()
  );

  getInstance()->onMixChange(0, mapValue, 0);
}

/**
 * On Fm Change
 */
inline void Drone::onFmChange(byte inputIndex, unsigned int value, int diffToPrevious){

  int modulatorFrequency = 0;
  float modulatorAmplitude = 0;
    
  if(value < getInstance()->device->getAnalogMaxValue() / 3){
    modulatorFrequency = map(value, 0, getInstance()->device->getAnalogMaxValue() / 2, 0, 10);
    modulatorAmplitude = (float)map((float)value, 0, getInstance()->device->getAnalogMaxValue() / 2, 0.001, .01);
  }
  else if(value >= getInstance()->device->getAnalogMaxValue() / 3 && value < getInstance()->device->getAnalogMaxValue() / 2){
    modulatorFrequency = map(value, 0, getInstance()->device->getAnalogMaxValue() / 2, 0, 40);
    modulatorAmplitude = (float)map((float)value, 0, getInstance()->device->getAnalogMaxValue() / 2, 0.001, .01);
  }else{
    modulatorFrequency = map(value, getInstance()->device->getAnalogMaxValue() / 2, getInstance()->device->getAnalogMaxValue(), 0, 1000);
    modulatorAmplitude = (float)map((float)value, getInstance()->device->getAnalogMaxValue() / 2, getInstance()->device->getAnalogMaxValue(), 0, .5);
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
