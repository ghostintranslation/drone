#ifndef MidiManager_h
#define MidiManager_h

#include <vector>
#include <algorithm>
#include <functional>
#include <MIDI.h>
#include "Input.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI); // MIDI library init

using MidiControlChangeCallbackFunction = void (*)(int16_t);

struct MidiControlChangeCallback{
  byte control;
  MidiControlChangeCallbackFunction callback = nullptr;
};

class MidiManager
{
  public:
    static MidiManager *getInstance();
    void addControlChangeCallback(byte control, MidiControlChangeCallbackFunction fptr);
    void removeControlChangeCallback(MidiControlChangeCallbackFunction fptr, byte control);
    bool isExistControlChangeCallback(MidiControlChangeCallbackFunction fptr, byte control);
    void update();
    std::vector<MidiControlChangeCallback> midiControlChangeCallbacks;

  private:
    // Singleton
    static MidiManager *instance;
    MidiManager();


    static void handleMidiControlChange(byte channel, byte control, byte value);
};

// Singleton pre init
MidiManager * MidiManager::instance = nullptr;

inline MidiManager::MidiManager(){

  // MIDI init
  MIDI.setHandleControlChange(handleMidiControlChange);
  MIDI.begin();
  usbMIDI.setHandleControlChange(handleMidiControlChange);

  Serial1.begin(31250, SERIAL_8N1_RXINV);
}

/**
 * Singleton instance
 */
inline MidiManager *MidiManager::getInstance() {
  if (!instance)
     instance = new MidiManager;
  return instance;
}

inline void MidiManager::addControlChangeCallback(byte control, MidiControlChangeCallbackFunction fptr){
  this->midiControlChangeCallbacks.push_back({
    control: control,
    callback: fptr
  });
}

  
inline void MidiManager::removeControlChangeCallback(MidiControlChangeCallbackFunction fptr, byte control){
//  MidiControlChangeCallback valueToRemove = {control: control, callback: fptr};
//  this->midiControlChangeCallbacks.erase(std::remove(this->midiControlChangeCallbacks.begin(), this->midiControlChangeCallbacks.end(), valueToRemove), this->midiControlChangeCallbacks.end());
}

inline bool MidiManager::isExistControlChangeCallback(MidiControlChangeCallbackFunction fptr, byte control){
//  MidiControlChangeCallback valueToCheck = {control: control, callback: fptr};
//  if ( std::find(this->midiControlChangeCallbacks.begin(), this->midiControlChangeCallbacks.end(), valueToCheck) != this->midiControlChangeCallbacks.end() ){
//    return true;
//  }

  return false;
}

inline void MidiManager::update(){
  MIDI.read();
  usbMIDI.read();
}

inline void MidiManager::handleMidiControlChange(byte channel, byte control, byte value){
  // This value is converted from the MIDI range of unsigned 8 bits to a signed 16 bits range to match
  // the audio library's blocks samples range. So a 0 value is now -32768.
  int16_t newValue = value * 256 - 32768;

//  for(MidiControlChangeCallback i : getInstance()->midiControlChangeCallbacks){
//    if(i.control == control && i.callback != nullptr){
//      i.callback(newValue);
//    }
//  }

  for(Input* input : Input::getAll()){
    if(input->getMidiCC() == control){
      input->onMidiCC(newValue);
    }
  }
}

#endif
