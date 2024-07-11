#ifndef Combine_h
#define Combine_h

#include <Audio.h>

class Combine : public AudioStream {
private:
  audio_block_t *inputQueueArray[1];

public:
  Combine();
  void update(void);
};


/**
 * Constructor
 */
inline Combine::Combine()
  : AudioStream(2, inputQueueArray) {
    // this->index=index;
}


inline void Combine::update(void) {
  audio_block_t *block1 = NULL, *block2 = NULL, *outBlock = NULL;

  block1 = receiveReadOnly(0);
  block2 = receiveReadOnly(1);
  outBlock = allocate();

  if (!block1 || !block2 || !outBlock) {
    return;
  }

  for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
    outBlock->data[i] = constrain((block1->data[i] + INT16_MAX) + (block2->data[i] + INT16_MAX) - INT16_MAX, INT16_MIN, INT16_MAX);
  }
  // if(index==1){
  // Serial.println(outBlock->data[0]);
  // }
  transmit(outBlock);
  release(block1);
  release(block2);
  release(outBlock);
}


#endif
