//
//  BlockDSPNode.cpp
//  Filter
//
//  Created by Luke on 4/2/16.
//
//

#include "BlockDSPNode.hpp"
#include "DelayBuffer.h"
#include "BlockDSPNumber.hpp"

#pragma mark - BlockDSPNode
BlockDSPNode::BlockDSPNode(uint32_t numChannels) {
    this->numChannels = numChannels;
    nodeID = -1;
}

BlockDSPNode::~BlockDSPNode() {}

uint32_t BlockDSPNode::getChannelCount() {
    return numChannels;
}

void BlockDSPNode::connectInput(BlockDSPNode *inputNode) {
    return;
}

float BlockDSPNode::valueForChannel(uint32_t channelNo) {
    return 0.0;
}

#pragma mark - BlockDSPSummerNode

BlockDSPSummerNode::BlockDSPSummerNode(uint32_t numChannels) : BlockDSPNode(numChannels) {}

void BlockDSPSummerNode::connectInput(BlockDSPNode *inputNode) {
    inputNodes.push_back(inputNode);
}

float BlockDSPSummerNode::valueForChannel(uint32_t channelNo) {
    float val = 0.0;
    for (size_t i=0; i<inputNodes.size(); i++) {
        val += inputNodes[i]->valueForChannel(channelNo);
    }
    
    return val;
}

#pragma mark - BlockDSPMultiplierNode
BlockDSPMutiplierNode::BlockDSPMutiplierNode(uint32_t numChannels) : BlockDSPNode(numChannels) {
    coefficient = BlockDSPNumber::numberForFloat(1.0);
}

void BlockDSPMutiplierNode::connectInput(BlockDSPNode *inputNode) {
    this->inputNode = inputNode;
}

float BlockDSPMutiplierNode::valueForChannel(uint32_t channelNo) {
    return inputNode->valueForChannel(channelNo) * coefficient->floatValue();
}

BlockDSPMutiplierNode::~BlockDSPMutiplierNode() {
    delete coefficient;
}


#pragma mark - BlockDSPDelayLineNode
BlockDSPDelayLineNode::BlockDSPDelayLineNode(uint32_t numChannels) : BlockDSPNode(numChannels) {
    delayIndex = 0;
    delayLine = 0;
}

void BlockDSPDelayLineNode::connectInput(BlockDSPNode *inputNode) {
    return;
}

float BlockDSPDelayLineNode::valueForChannel(uint32_t channelNo) {
    return delayLine->valueForDelayIndex(delayIndex, channelNo);
}

#pragma mark - BlockDSPDelayLine
BlockDSPDelayLine::BlockDSPDelayLine(uint32_t numChannels) {
    id = -1;
    this->numChannels = numChannels;
    delayBuffers = new DelayBuffer[numChannels];
}

BlockDSPDelayLine::~BlockDSPDelayLine() {
    if (numChannels > 0) {
        delete []delayBuffers;
    }
}

BlockDSPDelayLineNode *BlockDSPDelayLine::nodeForDelayIndex(size_t index) {
    BlockDSPDelayLineNode *dlnode = new BlockDSPDelayLineNode(numChannels);
    dlnode->delayLine = this;
    dlnode->delayIndex = index;
    
    return dlnode;
}

float BlockDSPDelayLine::valueForDelayIndex(size_t delayIndex, uint32_t channelNo) {
    return delayBuffers[channelNo].sampleAtDelayIndex(delayIndex);
}

void BlockDSPDelayLine::setSize(size_t delaySize) {
    for (size_t i=0; i<numChannels; i++) {
        delayBuffers[i].initWithSize(delaySize);
    }
}

void BlockDSPDelayLine::shuffle() {
    for (uint32_t i=0; i<numChannels; i++) {
        delayBuffers[i].shuffleIn(inputNode->valueForChannel(i));
    }
}

void BlockDSPDelayLine::reset() {
    for (size_t i=0; i<numChannels; i++) {
        delayBuffers[i].reset();
    }
}

#pragma mark - BlockDSPInputNode
BlockDSPInputNode::BlockDSPInputNode(uint32_t numChannels) : BlockDSPNode(numChannels) {
    inputBuffer = new float[getChannelCount()];
    memset(inputBuffer, 0, sizeof(float) * getChannelCount());
}

BlockDSPInputNode::~BlockDSPInputNode() {
    if (getChannelCount() > 0)
        delete []inputBuffer;
}

void BlockDSPInputNode::connectInput(BlockDSPNode *inputNode) {
    return;
}

float BlockDSPInputNode::valueForChannel(uint32_t channelNo) {
    return inputBuffer[channelNo];
}
