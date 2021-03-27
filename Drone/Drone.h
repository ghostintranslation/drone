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
    // Inputs callbacks
    static void onTuneChange(byte inputIndex, unsigned int value, int diffToPrevious);
    static void onMixChange(byte inputIndex, unsigned int value, int diffToPrevious);
    static void onFmChange(byte inputIndex, unsigned int value, int diffToPrevious);
    // Midi callbacks
    static void onMidiTuneChange(byte channel, byte control, byte value);
    static void onMidiMixChange(byte channel, byte control, byte value);
    static void onMidiFmChange(byte channel, byte control, byte value);
    // Audio output
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
    this->voices[i]->setUpdateMillis(this->updateMillis);
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
  device->setHandleMidiControlChange(0, 0, "Tune 1", onMidiTuneChange);
  device->setHandleMidiControlChange(0, 1, "Tune 2", onMidiTuneChange);
  device->setHandleMidiControlChange(0, 2, "Tune 3", onMidiTuneChange);
  device->setHandleMidiControlChange(0, 3, "Tune 4", onMidiTuneChange);
  device->setHandleMidiControlChange(0, 4, "Mix", onMidiMixChange);
  device->setHandleMidiControlChange(0, 5, "FM", onMidiFmChange);
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

  if(this->clockUpdate > updateMillis){
    for (int i = 0; i < voiceCount ; i++) {
      this->voices[i]->update();
    }
    
    this->clockUpdate = 0;
  }
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
 * On MIDI Tune Change
 */
void Drone::onMidiTuneChange(byte channel, byte control, byte value){
  unsigned int mapValue = map(
    value, 
    0,
    127,
    getInstance()->device->getAnalogMinValue(), 
    getInstance()->device->getAnalogMaxValue()
  );
  
  getInstance()->onTuneChange(control, mapValue, 0);
}

/**
 * On Mix Change
 */
inline void Drone::onMixChange(byte inputIndex, unsigned int value, int diffToPrevious){
  if(diffToPrevious == -1 || 
     diffToPrevious == 0 ||
     diffToPrevious ==1){
      return;
  }
     
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

  getInstance()->voices[0]->setGain(voice1Gain/400);
  getInstance()->voices[1]->setGain(voice2Gain/400);
  getInstance()->voices[2]->setGain(voice3Gain/400);
  getInstance()->voices[3]->setGain(voice4Gain/400);
      
  getInstance()->device->setLED(0, 1, 2.5 * voice1Gain);
  getInstance()->device->setLED(1, 1, 2.5 * voice2Gain);
  getInstance()->device->setLED(2, 1, 2.5 * voice3Gain);
  getInstance()->device->setLED(3, 1, 2.5 * voice4Gain);
}

/**
 * On MIDI Mix Change
 */
inline void Drone::onMidiMixChange(byte channel, byte control, byte value){
    unsigned int mapValue = map(
    value, 
    0,
    127,
    getInstance()->device->getAnalogMinValue(), 
    getInstance()->device->getAnalogMaxValue()
  );

  getInstance()->onMixChange(0, mapValue, 10);
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

/**
 * On MIDI Fm Change
 */
inline void Drone::onMidiFmChange(byte channel, byte control, byte value){
    unsigned int mapValue = map(
    value, 
    0,
    127,
    getInstance()->device->getAnalogMinValue(), 
    getInstance()->device->getAnalogMaxValue()
  );

  getInstance()->onFmChange(0, mapValue, 0);
}


#endif
