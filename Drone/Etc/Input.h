#ifndef Input_h
#define Input_h

#include "IO.h"
#include "Registrar.h"
#include "InputsManager.h"

// Forward declaration
class Input;

using InputCallback = void (*)(Input *input);

elapsedMicros debugClock2;
uint32_t debugCounter2 = 0;
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

  int16_t inputBuffer[AUDIO_BLOCK_SAMPLES] = {0};
  uint8_t bufferIndex = 0;

  // Low pass filter
  // https://www.earlevel.com/main/2013/10/13/biquad-calculator-v2/

  // type low shelf, fc = 1000, Q = 0.1, Gain = 10
  // float a0 = 1.080968292566556;
  // float a1 = -1.7791861633846875;
  // float a2 = 0.7564543569181826;
  // float b1 = -1.7990964094846682;
  // float b2 = 0.817512403384758;

  // type low pass, fc = 1000, Q = 0.752, Gain = 0
  // float a0 = 0.004629214321309756;
  // float a1 = 0.009258428642619512;
  // float a2 = 0.004629214321309756;
  // float b1 = -1.8089499615142142;
  // float b2 = 0.8274668187994533;

  // type low pass, fc = 500, Q = 0.651, Gain = 0
  float a0 = 0.0012024392608342668;
  float a1 = 0.0024048785216685335;
  float a2 = 0.0012024392608342668;
  float b1 = -1.8915216632917207;
  float b2 = 0.8963314203350575;

  float z1 = 0;
  float z2 = 0;
  int16_t processLowPass(int16_t sample);

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

int16_t *block1 = NULL;
int16_t *block2 = NULL;
int16_t *block3 = NULL;
int16_t *block4 = NULL;
int16_t *block5 = NULL;
int16_t *block6 = NULL;
int16_t *block7 = NULL;
int16_t *block8 = NULL;

inline void Input::update(void)
{
  // if (debugCounter2 == 0)
  // {
  // if (this->index == 0)
  // {
  //   Serial.print("debugClock2: ");
  //   Serial.println(debugClock2);
  //   debugClock2 = 0;
  // }
  //   debugCounter2++;
  // }
  // Serial.println("Input::update");

  IO::update();

  // Serial.println("Input::update 2");

  audio_block_t *block, *triggerBlock, *gateBlock;

  // allocate the audio blocks to transmit
  block = allocate();
  triggerBlock = allocate();
  gateBlock = allocate();

  // Serial.println("Input::update 3");

  //  volatile audio_block* blockData = InputsManager::getInstance()->getData(this->index);
  //
  //  for(int i=blockData->readIndex; i < blockData->count; i++){
  //    block->data[i] = blockData->data[i];
  //    blockData->readIndex++;
  ////    Serial.println(blockData->data[i]);
  //  }

  //  int16_t* inputBuffer = InputsManager::getInstance()->getBuffers(this->index)->read();
  //

  // ------
  // if (this->index == 5)
  // {
  //   int16_t *block12 = InputsManager::getInstance()->readInput(0);
  //   if (block12 != NULL)
  //   {
  //     block1 = block12;
  //   }
  //   int16_t *block22 = InputsManager::getInstance()->readInput(1);
  //   if (block22 != NULL)
  //   {
  //     block2 = block22;
  //   }
  //   int16_t *block32 = InputsManager::getInstance()->readInput(2);
  //   if (block32 != NULL)
  //   {
  //     block3 = block32;
  //   }
  //   int16_t *block42 = InputsManager::getInstance()->readInput(3);
  //   if (block42 != NULL)
  //   {
  //     block4 = block42;
  //   }
  //   int16_t *block52 = InputsManager::getInstance()->readInput(4);
  //   if (block52 != NULL)
  //   {
  //     block5 = block52;
  //   }
  //   int16_t *block62 = InputsManager::getInstance()->readInput(5);
  //   if (block62 != NULL)
  //   {
  //     block6 = block62;
  //   }
  //   int16_t *block72 = InputsManager::getInstance()->readInput(6);
  //   if (block72 != NULL)
  //   {
  //     block7 = block72;
  //   }
  //   int16_t *block82 = InputsManager::getInstance()->readInput(7);
  //   if (block82 != NULL)
  //   {
  //     block8 = block82;
  //   }

  //   if (block1 != NULL && block2 != NULL && block3 != NULL && block4 != NULL && block5 != NULL && block6 != NULL && block7 != NULL && block8 != NULL)
  //   {
  //     for (unsigned int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
  //     {
  //       Serial.print(block1[i]);
  //       Serial.print(",");
  //       Serial.print(block2[i]);
  //       Serial.print(",");
  //       Serial.print(block3[i]);
  //       Serial.print(",");
  //       Serial.print(block4[i]);
  //       Serial.print(",");
  //       Serial.print(block5[i]);
  //       Serial.print(",");
  //       Serial.print(block6[i]);
  //       Serial.print(",");
  //       Serial.print(block7[i]);
  //       Serial.print(",");
  //       Serial.print(block8[i]);
  //       Serial.println("");
  //     }
  //     block1 = NULL;
  //     block2 = NULL;
  //     block3 = NULL;
  //     block4 = NULL;
  //     block5 = NULL;
  //     block6 = NULL;
  //     block7 = NULL;
  //     block8 = NULL;
  //   }
  // }
  // ------

  int16_t *inputBuffer = InputsManager::getInstance()->readInput(this->index);
  // Serial.println("Input::update 4");

  if (inputBuffer != NULL)
  {
    // Serial.println("Input::update 5");
    this->setBuffer(inputBuffer);
    // Serial.println("Input::update 6");

    for (unsigned int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    {

      // Raw output
      if (block)
      {
        // block->data[i] = this->value;
        block->data[i] = this->inputBuffer[i];
        // if (this->index == 0)
        // {
        //   Serial.println(block->data[i]);
        // }
      }
      //   // Trigger output

      //   if (triggerBlock)
      //   {
      //     if (inputBuffer[i] > 0 && triggerOnSamples == 0)
      //     {
      //       triggerOnSamples++;
      //     }

      //     if (triggerOnSamples > 0 && triggerOnSamples < 128)
      //     {
      //       triggerBlock->data[i] = INT16_MAX;
      //     }
      //     else
      //     {
      //       triggerBlock->data[i] = INT16_MIN;
      //     }

      //     if (triggerOnSamples >= 128)
      //     {
      //       triggerOnSamples = 0;
      //     }
      //   }

      //   // Gate output
      //   if (gateBlock)
      //   {
      //     gateBlock->data[i] = inputBuffer[i] > 0 ? INT16_MAX : INT16_MIN;
      //   }

      //   // TODO: MAKE AN AVERAGE MEMBER?
      //   // Aproximated moving average
      //   // this->target -= this->target / AUDIO_BLOCK_SAMPLES;
      //   // this->target += block->data[i] / AUDIO_BLOCK_SAMPLES;
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

// elapsedMicros testClock;
inline void Input::setBuffer(int16_t *buffer)
{
  for (unsigned int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
  {
    this->inputBuffer[i] = buffer[i]; // buffer[i]);
  }

  this->bufferIndex = 0;
}

inline int16_t Input::processLowPass(int16_t in)
{
  int32_t out = in * a0 + z1;
  if (out < -32768)
    out = -32768;
  if (out > 32767)
    out = 32767;
  z1 = in * a1 + z2 - b1 * out;
  z2 = in * a2 - b2 * out;
  return (int16_t)out;
}
#endif
