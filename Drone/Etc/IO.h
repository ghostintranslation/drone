#ifndef IO_h
#define IO_h

#include "AudioStream.h"

class IO : public AudioStream
{
public:
  IO(uint8_t index, uint8_t numberOfInputsPorts, audio_block_t **iqueue);
  virtual void update(void);
  virtual uint8_t getIndex();
  virtual int16_t getValue();
  virtual void setValue(int16_t value);
  virtual int16_t getTarget();
  virtual void setTarget(int16_t target);
  virtual void setSmoothing(uint16_t smoothing);

protected:
  uint8_t index = 0;
  int16_t value = -32768;
  int16_t prevValue = -32768;
  int16_t target = -32768;
  uint16_t smoothing = 50;
};

inline IO::IO(uint8_t index, uint8_t numberOfInputsPorts, audio_block_t **iqueue) : AudioStream(numberOfInputsPorts, iqueue)
{
  this->index = index;
  if (this->index > 8)
  {
    this->index = 8;
  }

  this->active = true;
}

inline void IO::update(void)
{
  // Update the value to reach the target
  if (this->target != this->value)
  {
    // If no smoothing no need to run any maths
    if (this->smoothing == 0)
    {
      this->value = this->target;
    }
    else
    {
      // time (ms) between each update = 1000 / AUDIO_SAMPLE_RATE_EXACT * 128 = 2.9
      this->value += (((float)1 / this->smoothing) * (this->target - this->value)) * (1000 / AUDIO_SAMPLE_RATE_EXACT * 128);
    }
  }
}

/**
   The smoothing is non linear, it uses the difference between target and value.
   The bigger the difference the bigger the increments.

   0 = no smoothing

   For example, for a difference of 10000 between target and value, and with a
   smoothing of 1000, the first millisecond it will increment 10, the next will be 9.99,
   the next will be 9.98...

   @param int16_t smoothing The smoothing.
*/
inline void IO::setSmoothing(uint16_t smoothing)
{
  this->smoothing = smoothing;
}

inline uint8_t IO::getIndex()
{
  return this->index;
}

/**
   Get the value.

   @return int16_t The value.
*/
inline int16_t IO::getValue()
{
  return this->value;
}

/**
   Set the value.

   @param int16_t value The value.
*/
inline void IO::setValue(int16_t value)
{
  this->value = value;
}

/**
   Get the target.

   @return int16_t The target.
*/
inline int16_t IO::getTarget()
{
  return this->target;
}

/**
   Set the target.

   @param int16_t target The target.
*/
inline void IO::setTarget(int16_t target)
{
  this->target = target;
}

#endif
