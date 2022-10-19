#ifndef DC_h
#define DC_h

#include <Audio.h>

/**
 * This object converts 1 input into 4 outputs following a spiral pattern
 */
class DC : public AudioStream{
  public:
    DC();
    void update(void);
};


/**
 * Constructor
 */
inline DC::DC() : AudioStream(0, NULL){
}


inline void DC::update(void) {
   
    audio_block_t *block;
    block = allocate();

    if(!block){
      return;
    }
    
    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
      block->data[i] = -32768;
    }

    transmit(block, 0);
    release(block);
}


#endif
