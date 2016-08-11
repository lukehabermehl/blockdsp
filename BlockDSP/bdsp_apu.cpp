//
//  BlockDSPAPU.cpp
//  libblockdsp
//
//  Created by Luke on 5/24/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "bdsp_apu.hpp"
#include "bdsp_parameter.hpp"

BlockDSPAPU::BlockDSPAPU(BlockDSPSystem *system) : system_(system)
{
}

BlockDSPAPU::BlockDSPAPU(BlockDSPSystemFactoryFunc systemFactoryFunc)
{
    system_ = systemFactoryFunc(this);
}

BlockDSPSystem * BlockDSPAPU::getSystem()
{
    return system_;
}

void BlockDSPAPU::processAudio(float *inputBuffer, float *outputBuffer, int numInputChannels, int numOutputChannels)
{
    system_->mainInputNode->inputBuffer = inputBuffer;
    outputBuffer[0] = system_->mainOutputNode->valueForChannel(0);
    
    if (numInputChannels == 1 && numOutputChannels == 2)
        outputBuffer[1] = outputBuffer[0];
    
    else if (numOutputChannels == 2)
        outputBuffer[1] = system_->mainOutputNode->valueForChannel(1);
    
    system_->next();
}

void BlockDSPAPU::setMaxInputChannels(uint32_t num)
{
    system_->setNumInputChannels(num);
}

void BlockDSPAPU::setMaxOutputChannels(uint32_t num)
{
    system_->setNumOutputChannels(num);
}

void BlockDSPAPU::onParameterChanged(BlockDSPParameter *parameter, BlockDSPNumber *value)
{
}

BlockDSPParameter * BlockDSPAPU::createParameter(const char *name, BlockDSPNumberType numberType, BlockDSPNumber *target)
{
    BlockDSPParameter * param = new BlockDSPParameter(numberType, name, target, this);
    paramList_.append(param);
    
    return param;
}

BlockDSPParameterList& BlockDSPAPU::getParameterList()
{
    return paramList_;
}
