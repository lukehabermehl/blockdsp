//
//  BlockDSPSystem.cpp
//  Filter
//
//  Created by Luke on 4/2/16.
//
//

#include "bdsp_system.hpp"
#include "bdsp_system_private.hpp"
#include "bdsp_parameter.hpp"
#include "dsphelpers.hpp"
#include "bdsp_number.hpp"

BlockDSPSystem::BlockDSPSystem() {
    _pimpl = new pimpl;
    //Default channels to MONO -> MONO
    _pimpl->outputChannelCount = 1;
    _pimpl->inputChannelCount = 1;
    _pimpl->counter = 0;
    
    mainInputNode = new BlockDSPInputNode(0, getNumInputChannels());
    mainOutputNode = mainInputNode;
}

BlockDSPSystem::~BlockDSPSystem() {
    delete mainInputNode;
    //Don't ever delete mainOutputNode here
    
    for (auto it=_pimpl->nodeMap.begin(); it != _pimpl->nodeMap.end(); it++) {
        BlockDSPNode *node = it->second;
        delete node;
    }
    
    for (auto it = _pimpl->delayLineMap.begin(); it != _pimpl->delayLineMap.end(); it++) {
        delete it->second;
    }
    
    delete _pimpl;
}

uint32_t BlockDSPSystem::getNumInputChannels()
{
    return _pimpl->inputChannelCount;
}

void BlockDSPSystem::setNumInputChannels(uint32_t num)
{
    _pimpl->inputChannelCount = num;
}

uint32_t BlockDSPSystem::getNumOutputChannels()
{
    return _pimpl->outputChannelCount;
}

void BlockDSPSystem::setNumOutputChannels(uint32_t num)
{
    _pimpl->outputChannelCount = num;
}

void BlockDSPSystem::addNode(BlockDSPNode *node) {
    //TODO: assert node ID?
    _pimpl->nodeMap[node->getID()] = node;
}

void BlockDSPSystem::removeNode(BlockDSPNode *node) {
    auto it = _pimpl->nodeMap.find(node->getID());
    if (it != _pimpl->nodeMap.end())
        _pimpl->nodeMap.erase(it);
}

void BlockDSPSystem::addDelayLine(BlockDSPDelayLine *dl) {
    dl->setID(_pimpl->counter++);
    _pimpl->delayLineMap[dl->getID()] = dl;
}

void BlockDSPSystem::next() {
    for (auto it = _pimpl->delayLineMap.begin(); it != _pimpl->delayLineMap.end(); it++) {
        it->second->shuffle();
    }
    
    for (auto it = _pimpl->nodeMap.begin(); it != _pimpl->nodeMap.end(); it++) {
        it->second->onNext();
    }
}

BlockDSPSummerNode *BlockDSPSystem::createSummerNode(BlockDSPNodeID nodeID) {
    BlockDSPSummerNode *summerNode = new BlockDSPSummerNode(nodeID, getNumInputChannels(), getNumOutputChannels());
    addNode(summerNode);
    
    return summerNode;
}

BlockDSPDelayLine *BlockDSPSystem::createDelayLine(BlockDSPNodeID delayLineID, BlockDSPNode *inputNode) {
    BlockDSPDelayLine *delayLine = new BlockDSPDelayLine(delayLineID, getNumInputChannels());
    delayLine->inputNode = inputNode;
    addDelayLine(delayLine);
    
    return delayLine;
}

BlockDSPMultiplierNode *BlockDSPSystem::createMultiplierNode(BlockDSPNodeID nodeID) {
    BlockDSPMultiplierNode *node = new BlockDSPMultiplierNode(nodeID, getNumInputChannels(), getNumOutputChannels());
    addNode(node);
    
    return node;
}

BlockDSPInputNode *BlockDSPSystem::createInputNode(BlockDSPNodeID nodeID)
{
    BlockDSPInputNode *node = new BlockDSPInputNode(nodeID, getNumInputChannels());
    addNode(node);
    
    return node;
}

void BlockDSPSystem::addNumber(const char *name, APUNumber number)
{
    std::string numName = std::string(name);
    auto it = _pimpl->numberMap.find(numName);
    if (it != _pimpl->numberMap.end())
        return;
    
    _pimpl->numberMap[numName] = number;
}

BlockDSPNode *BlockDSPSystem::nodeWithID(ssize_t id) {
    auto it = _pimpl->nodeMap.find(id);
    if (it == _pimpl->nodeMap.end())
        return nullptr;
    
    return it->second;
}

APUNumber BlockDSPSystem::numberWithName(const char *name, bool &success) {
    auto it = _pimpl->numberMap.find(name);
    if (it != _pimpl->numberMap.end()) {
        success = true;
        return it->second;
    }

    success = false;
    return APUNumber::numberForBool(false);
}

BlockDSPDelayLine * BlockDSPSystem::delayLineWithID(BlockDSPNodeID id) {
    auto it = _pimpl->delayLineMap.find(id);
    if (it != _pimpl->delayLineMap.end()) {
        return it->second;
    } else {
        return NULL;
    }
}

BlockDSPSystem *BlockDSPSystem::systemForBiQuad(uint32_t numChannels, unsigned int sampleRate) {
    BlockDSPSystem *system = new BlockDSPSystem();
    system->setNumInputChannels(numChannels);
    system->setNumOutputChannels(numChannels);

    const BlockDSPNodeID IN_DELAY_ID = 1;
    const BlockDSPNodeID SUMMER_ID = 2;
    const BlockDSPNodeID OUT_DELAY_ID = 3;
    const BlockDSPNodeID A0_NODE_ID = 4;
    const BlockDSPNodeID A1_NODE_ID = 5;
    const BlockDSPNodeID A2_NODE_ID = 6;
    const BlockDSPNodeID B1_NODE_ID = 7;
    const BlockDSPNodeID B2_NODE_ID = 8;
    const BlockDSPNodeID OUT_GAIN_ID = 9;
    const BlockDSPNodeID IN_DELAY_TAP1_ID = 10;
    const BlockDSPNodeID IN_DELAY_TAP2_ID = 11;
    const BlockDSPNodeID OUT_DELAY_TAP1_ID = 12;
    const BlockDSPNodeID OUT_DELAY_TAP2_ID = 13;
    
    BlockDSPDelayLine *inDelayLine = system->createDelayLine(IN_DELAY_ID, system->mainInputNode);
    inDelayLine->setSize(2);
    BlockDSPSummerNode *summer = system->createSummerNode(SUMMER_ID);
    BlockDSPDelayLine *outDelayLine = system->createDelayLine(OUT_DELAY_ID, summer);
    outDelayLine->setSize(2);
    
    BlockDSPMultiplierNode *a0Node = system->createMultiplierNode(A0_NODE_ID);
    BlockDSPMultiplierNode *a1Node = system->createMultiplierNode(A1_NODE_ID);
    BlockDSPMultiplierNode *a2Node = system->createMultiplierNode(A2_NODE_ID);
    BlockDSPMultiplierNode *b1Node = system->createMultiplierNode(B1_NODE_ID);
    BlockDSPMultiplierNode *b2Node = system->createMultiplierNode(B2_NODE_ID);
    
    BlockDSPMultiplierNode *outGainNode = system->createMultiplierNode(OUT_GAIN_ID);
    outGainNode->connectInput(summer);
    
    a0Node->connectInput(system->mainInputNode);
    a1Node->connectInput(inDelayLine->nodeForDelayIndex(IN_DELAY_TAP1_ID, 1));
    a2Node->connectInput(inDelayLine->nodeForDelayIndex(IN_DELAY_TAP2_ID, 2));
    b1Node->connectInput(outDelayLine->nodeForDelayIndex(OUT_DELAY_TAP1_ID, 1));
    b2Node->connectInput(outDelayLine->nodeForDelayIndex(OUT_DELAY_TAP2_ID, 2));
    
    summer->connectInput(a0Node);
    summer->connectInput(a1Node);
    summer->connectInput(a2Node);
    summer->connectInput(b1Node);
    summer->connectInput(b2Node);
    
    BiQuadCoefficients coeffs;
    coeffs.calculateForLPF(1000, 3.0, sampleRate);
    
    a0Node->coefficient = APUNumber::numberForFloat(coeffs.a0);
    a1Node->coefficient = APUNumber::numberForFloat(coeffs.a1);
    a2Node->coefficient = APUNumber::numberForFloat(coeffs.a2);
    b1Node->coefficient = APUNumber::numberForFloat(-1 * coeffs.b1);
    b2Node->coefficient = APUNumber::numberForFloat(-1 * coeffs.b2);
    
    system->mainOutputNode = outGainNode;
    outGainNode->coefficient = APUNumber::numberForFloat(2.0);
    
    return system;
}

void BlockDSPSystem::reset() {
    for (auto it = _pimpl->delayLineMap.begin(); it != _pimpl->delayLineMap.end(); it++) {
        it->second->reset();
    }
}
