//
//  libblockdsp
//
//  Created by Luke on 5/24/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file bdsp_apu.hpp */

#ifndef BlockDSPAPU_hpp
#define BlockDSPAPU_hpp

#include "autil_audioprocessingunit.hpp"
#include "bdsp_system.hpp"

/** An AudioProcessingUnit built with a BlockDSP system */
class BlockDSPAPU : public AudioProcessingUnit
{
public:
    /** The system that drives the audio processing */
    BlockDSPSystem *system;
    /** Process a frame of audio. See AudioProcessingUnit */
    virtual void processAudio(float *inputBuffer, float *outputBuffer, int numInputChannels, int numOutputChannels);
};


#endif /* BlockDSPAPU_hpp */
