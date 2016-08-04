//
//  BlockDSPAPU.cpp
//  libblockdsp
//
//  Created by Luke on 5/24/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "bdsp_apu.hpp"

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

void BlockDSPAPU::setMaxInputChannels(uint32_t num)
{
    system->setNumInputChannels(num);
}

void BlockDSPAPU::setMaxOutputChannels(uint32_t num)
{
    system->setNumOutputChannels(num);
}
