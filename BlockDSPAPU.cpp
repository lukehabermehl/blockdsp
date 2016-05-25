//
//  BlockDSPAPU.cpp
//  libblockdsp
//
//  Created by Luke on 5/24/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "BlockDSPAPU.hpp"

void BlockDSPAPU::processAudio(float *inputBuffer, float *outputBuffer, int numInputChannels, int numOutputChannels)
{
    system->mainInputNode->inputBuffer = inputBuffer;
    outputBuffer[0] = system->mainOutputNode->valueForChannel(0);
    
    if (numInputChannels == 1 && numOutputChannels == 2)
        outputBuffer[1] = outputBuffer[0];
    
    else if (numOutputChannels == 2)
        outputBuffer[1] = system->mainOutputNode->valueForChannel(1);
    
    system->next();
}
