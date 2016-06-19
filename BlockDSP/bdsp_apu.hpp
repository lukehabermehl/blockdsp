//
//  BlockDSPAPU.hpp
//  libblockdsp
//
//  Created by Luke on 5/24/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef BlockDSPAPU_hpp
#define BlockDSPAPU_hpp

#include "autil_audioprocessingunit.hpp"
#include "bdsp_system.hpp"

class BlockDSPAPU : public AudioProcessingUnit
{
public:
    BlockDSPSystem *system;
    
    virtual void processAudio(float *inputBuffer, float *outputBuffer, int numInputChannels, int numOutputChannels);
};


#endif /* BlockDSPAPU_hpp */
