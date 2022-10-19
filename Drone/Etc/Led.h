#ifndef Led_h
#define Led_h

#include "IO.h"
#include "Registrar.h"

class LedManager;

class Led : public IO, public Registrar<Led>
{
public:
  enum Status
  {
    Off,
    On,
    Blink,
    BlinkFast,
    BlinkOnce
  };

  Led(byte index);
  void update(void) override;
  void set(Led::Status status, int16_t brightness);
  void setStatus(Led::Status status);
  void setBrightness(int16_t brightness);
  int16_t getBrightness();

private:
  audio_block_t *inputQueueArray[1];
  uint16_t target = 0;
  uint16_t value = 0;
  int16_t brightness = ABSOLUTE_ANALOG_MIN;

  Status status = Off;

  // Time counter for the blinking
  elapsedMillis blickClock;
};

inline Led::Led(byte index) : IO(index, 1, inputQueueArray)
{
}

inline void Led::update(void)
{
  IO::update();

  // Receive input data
  audio_block_t *block;
  block = receiveReadOnly(0);

  if (block)
  {
    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    {
      int16_t x = block->data[i];

      // Aproximated moving average
      this->brightness -= this->brightness / AUDIO_BLOCK_SAMPLES;
      this->brightness += x / AUDIO_BLOCK_SAMPLES;
      //      this->brightness = x;
    }

    release(block);
  }

  // Setting target according to status and brightness
  switch (this->status)
  {
  case Blink:
    if (this->blickClock % 400 < 200)
    {
      this->setTarget(INT16_MIN);
    }
    else
    {
      this->setTarget(this->brightness);
    }
    break;
  case BlinkFast:
    if (this->blickClock % 200 < 100)
    {
      this->setTarget(INT16_MIN);
    }
    else
    {
      this->setTarget(this->brightness);
    }
    break;
  case BlinkOnce:
    if (this->blickClock > 100)
    {
      this->setTarget(INT16_MIN);
    }
    else
    {
      this->setTarget(this->brightness);
    }
    break;

  default:
    this->setTarget(this->brightness);
    break;
  }
}

inline void Led::set(Led::Status status, int16_t brightness)
{
  this->setStatus(status);
  this->setBrightness(brightness);
}

inline void Led::setStatus(Led::Status status)
{
  this->status = status;

  switch (this->status)
  {
  case Off:
    this->brightness = 0;
    break;
  case BlinkOnce:
    this->blickClock = 0;
    break;
  default:
    break;
  }
}

inline void Led::setBrightness(int16_t brightness)
{
  this->brightness = brightness;
}

inline int16_t Led::getBrightness()
{
  return this->brightness;
}
#endif
