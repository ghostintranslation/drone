#ifndef Combine_h
#define Combine_h

#include <Audio.h>

class Combine : public AudioStream
{
private:
  audio_block_t *inputQueueArray[3];

public:
  Combine();
  void update(void);
};

/**
 * Constructor
 */
inline Combine::Combine()
    : AudioStream(3, inputQueueArray)
{
  // this->index=index;
}

inline void Combine::update(void)
{
  audio_block_t *block1 = NULL, *block2 = NULL, *block3 = NULL, *outBlock = NULL;

  block1 = receiveReadOnly(0);
  block2 = receiveReadOnly(1);
  block3 = receiveReadOnly(2);
  outBlock = allocate();

  // if (!block1 || !block2 || !block3 || !outBlock)
  if (!outBlock)
  {
    return;
  }

  for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
  {
    uint32_t temp = 0;
    if (block1)
    {
      temp += block1->data[i] + INT16_MAX;
    }
    if (block2)
    {
      temp += block2->data[i] + INT16_MAX;
    }
    if (block3)
    {
      temp += block3->data[i] + INT16_MAX;
    }
    outBlock->data[i] = constrain((int32_t)temp - INT16_MAX, INT16_MIN, INT16_MAX);
  }

  transmit(outBlock);
  if (block1)
  {
    release(block1);
  }
  if (block2)
  {
    release(block2);
  }
  if (block3)
  {
    release(block3);
  }
  release(outBlock);
}

#endif
