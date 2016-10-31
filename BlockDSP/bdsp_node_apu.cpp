//
//  bdsp_node_apu.cpp
//  libblockdsp
//
//  Created by Luke on 8/16/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "bdsp_node_apu.hpp"

BlockDSPAPUNode::BlockDSPAPUNode(BlockDSPNodeID nodeID, BlockDSPAPU *apu, uint32_t numInputChannels, uint32_t numOutputChannels)
: BlockDSPNode(nodeID, numInputChannels, numOutputChannels)
{
    apu->setMaxInputChannels(numInputChannels);
    apu->setMaxOutputChannels(numOutputChannels);
    
    outputFrame_ = new float[numOutputChannels];
    memset(outputFrame_, 0, sizeof(float) * numOutputChannels);
    inputFrame_ = new float[numInputChannels];
    memset(inputFrame_, 0, sizeof(float) * numInputChannels);
    
    this->apu_ = apu;
    needsUpdate_ = true;
}

BlockDSPAPUNode::~BlockDSPAPUNode()
{
    delete []inputFrame_;
    delete []outputFrame_;
}

void BlockDSPAPUNode::onNext()
{
    needsUpdate_ = true;
}

void BlockDSPAPUNode::connectInput(BlockDSPNode *inputNode)
{
    inputNode_ = inputNode;
}

float BlockDSPAPUNode::valueForChannel(uint32_t channelNo)
{
    if (needsUpdate_) {
        for (uint32_t i=0; i<getNumInputChannels(); i++) {
            inputFrame_[i] = inputNode_->valueForChannel(i);
        }
        apu_->processAudio(inputFrame_, outputFrame_, getNumInputChannels(), getNumOutputChannels());
        needsUpdate_ = false;
    }
    
    return outputFrame_[channelNo];
}

const APUParameterMap BlockDSPAPUNode::getParameters()
{
    return apu_->getParameterMap();
}
