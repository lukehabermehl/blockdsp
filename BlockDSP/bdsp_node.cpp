//
//  BlockDSPNode.cpp
//  Filter
//
//  Created by Luke on 4/2/16.
//
//

#include "bdsp_node.hpp"
#include "bdsp_node_private.hpp"
#include "dsputil_delaybuffer.hpp"
#include "bdsp_number.hpp"

#pragma mark - BlockDSPNode
BlockDSPNode::BlockDSPNode(uint32_t numChannels) {
    _pimpl = new pimpl;
    _pimpl->numChannels = numChannels;
    nodeID = -1;
}

BlockDSPNode::~BlockDSPNode() {
    delete _pimpl;
}

uint32_t BlockDSPNode::getChannelCount() {
    return _pimpl->numChannels;
}

void BlockDSPNode::connectInput(BlockDSPNode *inputNode) {
    return;
}

float BlockDSPNode::valueForChannel(uint32_t channelNo) {
    return 0.0;
}

#pragma mark - BlockDSPSummerNode

BlockDSPSummerNode::BlockDSPSummerNode(uint32_t numChannels) : BlockDSPNode(numChannels) {
    _summerNodePimpl = new summerNodePimpl;
}

BlockDSPSummerNode::~BlockDSPSummerNode()
{
    delete _summerNodePimpl;
}

void BlockDSPSummerNode::connectInput(BlockDSPNode *inputNode) {
    _summerNodePimpl->inputNodes.push_back(inputNode);
}

float BlockDSPSummerNode::valueForChannel(uint32_t channelNo) {
    float val = 0.0;
    for (size_t i=0; i<_summerNodePimpl->inputNodes.size(); i++) {
        val += _summerNodePimpl->inputNodes[i]->valueForChannel(channelNo);
    }
    
    return val;
}

#pragma mark - BlockDSPMultiplierNode
BlockDSPMultiplierNode::BlockDSPMultiplierNode(uint32_t numChannels) : BlockDSPNode(numChannels) {
    coefficient = BlockDSPNumber::numberForFloat(1.0);
    _multiplierNodePimpl = new multiplierNodePimpl;
}

void BlockDSPMultiplierNode::connectInput(BlockDSPNode *inputNode) {
    _multiplierNodePimpl->inputNode = inputNode;
}

float BlockDSPMultiplierNode::valueForChannel(uint32_t channelNo) {
    return _multiplierNodePimpl->inputNode->valueForChannel(channelNo) * coefficient->floatValue();
}

BlockDSPMultiplierNode::~BlockDSPMultiplierNode() {
    delete coefficient;
    delete _multiplierNodePimpl;
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
    _delayLinePimpl = new delayLinePimpl;
    _delayLinePimpl->numChannels = numChannels;
    _delayLinePimpl->delayBuffers = new DelayBuffer[numChannels];
}

BlockDSPDelayLine::~BlockDSPDelayLine() {
    if (_delayLinePimpl->numChannels > 0) {
        delete []_delayLinePimpl->delayBuffers;
    }
    
    delete _delayLinePimpl;
}

BlockDSPDelayLineNode *BlockDSPDelayLine::nodeForDelayIndex(size_t index) {
    BlockDSPDelayLineNode *dlnode = new BlockDSPDelayLineNode(_delayLinePimpl->numChannels);
    dlnode->delayLine = this;
    dlnode->delayIndex = index;
    
    return dlnode;
}

float BlockDSPDelayLine::valueForDelayIndex(size_t delayIndex, uint32_t channelNo) {
    return _delayLinePimpl->delayBuffers[channelNo].sampleAtDelayIndex(delayIndex);
}

void BlockDSPDelayLine::setSize(size_t delaySize) {
    for (size_t i=0; i<_delayLinePimpl->numChannels; i++) {
        _delayLinePimpl->delayBuffers[i].initWithSize(delaySize);
    }
}

void BlockDSPDelayLine::shuffle() {
    for (uint32_t i=0; i<_delayLinePimpl->numChannels; i++) {
        _delayLinePimpl->delayBuffers[i].shuffleIn(inputNode->valueForChannel(i));
    }
}

void BlockDSPDelayLine::reset() {
    for (size_t i=0; i<_delayLinePimpl->numChannels; i++) {
        _delayLinePimpl->delayBuffers[i].reset();
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
