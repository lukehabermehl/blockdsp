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
#include "bdsp_logger.hpp"

#pragma mark - BlockDSPNode
BlockDSPNode::BlockDSPNode(BlockDSPNodeID nodeID, uint32_t numInputChannels, uint32_t numOutputChannels) {
    _pimpl = new pimpl;
    _pimpl->numInputChannels = numInputChannels;
    _pimpl->numOutputChannels = numOutputChannels;
    _pimpl->bypass = false;
    setID(nodeID);
}

BlockDSPNode::~BlockDSPNode() {
    delete _pimpl;
}

BlockDSPNodeID BlockDSPNode::getID()
{
    return _pimpl->nodeID;
}

void BlockDSPNode::setID(BlockDSPNodeID nId)
{
    _pimpl->nodeID = nId;
}

uint32_t BlockDSPNode::getNumOutputChannels()
{
    return _pimpl->numOutputChannels;
}

uint32_t BlockDSPNode::getNumInputChannels()
{
    return _pimpl->numInputChannels;
}

void BlockDSPNode::setNumInputChannels(uint32_t num)
{
    _pimpl->numInputChannels = num;
}

void BlockDSPNode::setNumOutputChannels(uint32_t num)
{
    _pimpl->numOutputChannels = num;
}

void BlockDSPNode::connectInput(BlockDSPNode *inputNode) {
    setNumInputChannels(inputNode->getNumOutputChannels());
}

float BlockDSPNode::valueForChannel(uint32_t channelNo) {
    return 0.0;
}

void BlockDSPNode::setBypass(bool active)
{
    _pimpl->bypass = active;
}

bool BlockDSPNode::isBypassed()
{
    return _pimpl->bypass;
}

void BlockDSPNode::onNext()
{
}

#pragma mark - BlockDSPSummerNode

BlockDSPSummerNode::BlockDSPSummerNode(BlockDSPNodeID nodeID, uint32_t numInputChannels, uint32_t numOutputChannels)
: BlockDSPNode(nodeID, numInputChannels, numOutputChannels)
{
    _summerNodePimpl = new summerNodePimpl;
}

BlockDSPSummerNode::~BlockDSPSummerNode()
{
    delete _summerNodePimpl;
}

void BlockDSPSummerNode::connectInput(BlockDSPNode *inputNode) {
    _summerNodePimpl->inputNodes.push_back(inputNode);
    if (inputNode->getNumOutputChannels() > getNumInputChannels()) {
        setNumInputChannels(inputNode->getNumOutputChannels());
    }
}

float BlockDSPSummerNode::valueForChannel(uint32_t channelNo) {
    float val = 0.0;
    if (isBypassed())
        return val;

    for (size_t i=0; i<_summerNodePimpl->inputNodes.size(); i++) {
        val += _summerNodePimpl->inputNodes[i]->valueForChannel(channelNo);
    }
    
    return val;
}

#pragma mark - BlockDSPMultiplierNode
BlockDSPMultiplierNode::BlockDSPMultiplierNode(BlockDSPNodeID nodeID, uint32_t numInputChannels, uint32_t numOutputChannels)
: BlockDSPNode(nodeID, numInputChannels, numOutputChannels)
{
    coefficient.setFloatValue(1.0);
    useParameter = false;
    _multiplierNodePimpl = new multiplierNodePimpl;
}

void BlockDSPMultiplierNode::connectInput(BlockDSPNode *inputNode) {
    _multiplierNodePimpl->inputNode = inputNode;
    setNumInputChannels(inputNode->getNumOutputChannels());
}

float BlockDSPMultiplierNode::valueForChannel(uint32_t channelNo) {
    float coeff = coefficient.floatValue();
    if (useParameter && parameter != NULL)
    {
        coeff = parameter->getCurrentValue().floatValue();
    }
    return _multiplierNodePimpl->inputNode->valueForChannel(channelNo) * (isBypassed() ? 1.f : coeff);
}

BlockDSPMultiplierNode::~BlockDSPMultiplierNode() {
    delete _multiplierNodePimpl;
}


#pragma mark - BlockDSPDelayLineNode
BlockDSPDelayLineNode::BlockDSPDelayLineNode(BlockDSPNodeID nodeID, uint32_t numChannels) : BlockDSPNode(nodeID, numChannels) {
    delayIndex = 0;
    delayLine = 0;
}

void BlockDSPDelayLineNode::connectInput(BlockDSPNode *inputNode)
{
}

float BlockDSPDelayLineNode::valueForChannel(uint32_t channelNo) {
    if (isBypassed())
        return delayLine->inputNode->valueForChannel(channelNo);

    return delayLine->valueForDelayIndex(delayIndex, channelNo);
}

#pragma mark - BlockDSPDelayLine
BlockDSPDelayLine::BlockDSPDelayLine(BlockDSPNodeID delayLineID, uint32_t numChannels) {
    _delayLinePimpl = new delayLinePimpl;
    _delayLinePimpl->numChannels = numChannels;
    _delayLinePimpl->delayBuffers = new DelayBuffer[numChannels];
    setID(delayLineID);
}

BlockDSPDelayLine::~BlockDSPDelayLine() {
    if (_delayLinePimpl->numChannels > 0) {
        delete []_delayLinePimpl->delayBuffers;
    }
    
    delete _delayLinePimpl;
}

BlockDSPNodeID BlockDSPDelayLine::getID()
{
    return _delayLinePimpl->dlID;
}

void BlockDSPDelayLine::setID(BlockDSPNodeID nID)
{
    _delayLinePimpl->dlID = nID;
}

BlockDSPDelayLineNode *BlockDSPDelayLine::nodeForDelayIndex(BlockDSPNodeID nodeID, size_t index) {
    BlockDSPDelayLineNode *dlnode = new BlockDSPDelayLineNode(nodeID, _delayLinePimpl->numChannels);
    dlnode->delayLine = this;
    dlnode->delayIndex = index;

    BDLogFormat("[BlockDSPDelayLine]", "Create delay line tap on delay line %lu at index %lu", nodeID, index);
    
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
BlockDSPInputNode::BlockDSPInputNode(BlockDSPNodeID nodeID, uint32_t numInputChannels)
: BlockDSPNode(nodeID, numInputChannels, numInputChannels)
, inputBuffer(NULL)
{
    _inputNodePimpl = new inputNodePimpl;
    _inputNodePimpl->sourceNode = NULL;
}

BlockDSPInputNode::~BlockDSPInputNode()
{
    delete _inputNodePimpl;
}

void BlockDSPInputNode::setSourceNode(BlockDSPNode *node)
{
    _inputNodePimpl->sourceNode = node;
}

void BlockDSPInputNode::connectInput(BlockDSPNode *inputNode)
{
    setSourceNode(inputNode);
}

float BlockDSPInputNode::valueForChannel(uint32_t channelNo) {
    if (isBypassed())
    {
        return 0.f;
    }
    else if (_inputNodePimpl->sourceNode)
    {
        return _inputNodePimpl->sourceNode->valueForChannel(channelNo);
    }

    return inputBuffer[channelNo];
}
