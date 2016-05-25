//
//  AudioProcessingUnit.hpp
//  libblockdsp
//
//  Created by Luke on 5/24/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef AudioProcessingUnit_hpp
#define AudioProcessingUnit_hpp

#include "audio_constants.h"

/** Abstract class */
class AudioProcessingUnit
{
public:
    /** Override this method to do audio processing */
    virtual void processAudio(float *inputBuffer, float *outputBuffer, int numInputChannels, int numOutputChannels);
    virtual ~AudioProcessingUnit() {};
    
    static AudioProcessingUnit * createPassthroughUnit();
};


#endif /* AudioProcessingUnit_hpp */
