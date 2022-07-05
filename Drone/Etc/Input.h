#ifndef Input_h
#define Input_h

#include "IO.h"
#include "Registrar.h"
#include "InputsManager.h"

// Forward declaration
class Input;

using InputCallback = void (*)(Input *input);

/**
   Physical input object
   0 inputs
   3 outputs: raw, trigger, gate
*/
class Input : public IO, public Registrar<Input>
{
public:
  enum MergeMode
  {
    Add,
    Multiply
  };

  Input(byte index);
  //    void init();
  virtual void update(void) override;
  void realTimeUpdate();
  void setOnChange(InputCallback changeCallback);
  void setOnGateOpen(InputCallback gateOpenCallback);
  void setOnGateClose(InputCallback gateCloseCallback);
  void setMidiCC(byte midiCC);
  void setMergeMode(MergeMode mergeMode);
  uint8_t getMidiCC();
  void onMidiCC(int16_t value);
  void setBuffer(int16_t *buffer);

private:
  elapsedMicros updateClock;

  int16_t *inputBuffer;
  uint8_t bufferIndex = 0;

  // The number of samples the trigger has been on
  byte triggerOnSamples = 0;

  // How to merge ADC and MIDI values
  MergeMode mergeMode = Add;

  // MIDI
  uint8_t midiCC = 0;
  int16_t midiValue = 0;

  // Callbacks
  InputCallback changeCallback = nullptr;
  InputCallback gateOpenCallback = nullptr;
  InputCallback gateCloseCallback = nullptr;
};

inline Input::Input(byte index) : IO(index, 0, NULL)
{
  //  inputBuffer = new int16_t;
}

inline void Input::update(void)
{
  IO::update();

  audio_block_t *block, *triggerBlock, *gateBlock;

  // allocate the audio blocks to transmit
  block = allocate();
  triggerBlock = allocate();
  gateBlock = allocate();

  //  volatile audio_block* blockData = InputsManager::getInstance()->getData(this->index);
  //
  //  for(int i=blockData->readIndex; i < blockData->count; i++){
  //    block->data[i] = blockData->data[i];
  //    blockData->readIndex++;
  ////    Serial.println(blockData->data[i]);
  //  }

  //  int16_t* inputBuffer = InputsManager::getInstance()->getBuffers(this->index)->read();
  //
  int16_t *inputBuffer = InputsManager::getInstance()->readInput(this->index);

  if (inputBuffer != NULL)
  {
    this->setBuffer(inputBuffer);

    for (unsigned int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    {

      // Raw output
      if (block)
      {
        block->data[i] = inputBuffer[i];
      }
      // Trigger output

      if (triggerBlock)
      {
        if (inputBuffer[i] > 0 && triggerOnSamples == 0)
        {
          triggerOnSamples++;
        }

        if (triggerOnSamples > 0 && triggerOnSamples < 128)
        {
          triggerBlock->data[i] = INT16_MAX;
        }
        else
        {
          triggerBlock->data[i] = INT16_MIN;
        }

        if (triggerOnSamples >= 128)
        {
          triggerOnSamples = 0;
        }
      }

      // Gate output
      if (gateBlock)
      {
        gateBlock->data[i] = inputBuffer[i] > 0 ? INT16_MAX : INT16_MIN;
      }

      // TODO: MAKE AN AVERAGE MEMBER?
      // Aproximated moving average
      // this->target -= this->target / AUDIO_BLOCK_SAMPLES;
      // this->target += block->data[i] / AUDIO_BLOCK_SAMPLES;
    }

    if (block)
    {
      transmit(block, 0);
    }

    if (triggerBlock)
    {
      transmit(triggerBlock, 1);
    }

    if (gateBlock)
    {
      transmit(gateBlock, 2);
    }
  }

  if (block)
  {
    release(block);
  }
  if (triggerBlock)
  {
    release(triggerBlock);
  }
  if (gateBlock)
  {
    release(gateBlock);
  }
}

/**
 * This is called every 22us to consume the buffer
 */
inline void Input::realTimeUpdate()
{
  if (this->bufferIndex >= AUDIO_BLOCK_SAMPLES || this->inputBuffer == NULL)
  {
    return;
  }

  // Setting target with next sample
  this->target = this->inputBuffer[this->bufferIndex++];

  // Merging the ADC and Midi values to form the target value
  switch (this->mergeMode)
  {
  case Multiply:
    this->target *= this->midiValue;
    break;

  case Add:
  default:
    this->target += this->midiValue;
    break;
  }

  // Triggering Gate
  if (this->gateOpenCallback != nullptr)
  {
    if (this->value > 0 && this->prevValue < 0)
    {
      this->gateOpenCallback(this);
    }
    else if (this->value < 0 && this->prevValue > 0)
    {
      this->gateCloseCallback(this);
    }
  }

  // Triggering Change
  if (this->changeCallback != nullptr)
  {
    if (this->value != this->prevValue)
    {
      this->changeCallback(this);
    }
  }

  this->prevValue = this->value;
}

/**
   Set the callback function to call when the value changes
*/
inline void Input::setOnChange(InputCallback changeCallback)
{
  this->changeCallback = changeCallback;
}

/**

*/
inline void Input::setOnGateOpen(InputCallback gateOpenCallback)
{
  this->gateOpenCallback = gateOpenCallback;
}

/**

*/
inline void Input::setOnGateClose(InputCallback gateCloseCallback)
{
  this->gateCloseCallback = gateCloseCallback;
}

inline void Input::setMidiCC(byte midiCC)
{
  // If a callback is already registered for this input, remove it
  //  if(MidiManager::getInstance()->isExistControlChangeCallback(onMidiCC, this->midiCC)){
  //    MidiManager::getInstance()->removeControlChangeCallback(onMidiCC, this->midiCC);
  //  }

  // Register the callback for this input on the specified CC number
  //  MidiManager::getInstance()->addControlChangeCallback(midiCC, this);

  this->midiCC = midiCC;
}

inline void Input::setMergeMode(MergeMode mergeMode)
{
  this->mergeMode = mergeMode;
}

inline void Input::onMidiCC(int16_t value)
{
  this->midiValue = value;
}

inline uint8_t Input::getMidiCC()
{
  return this->midiCC;
}

//elapsedMicros testClock;
inline void Input::setBuffer(int16_t *buffer)
{
  //  TODO: THERE IS A TIMING ISSUE
  //  Serial.println(testClock);
  //  testClock = 0;

  this->inputBuffer = buffer;
  this->bufferIndex = 0;
}
#endif
