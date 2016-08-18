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
    
    mainInputNode = new BlockDSPInputNode(getNumInputChannels());
    mainOutputNode = mainInputNode;
}

BlockDSPSystem::~BlockDSPSystem() {
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
    if (node->getID() == kInvalidNodeID)
        node->setID(_pimpl->counter++);
    
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

BlockDSPSummerNode *BlockDSPSystem::createSummerNode() {
    BlockDSPSummerNode *summerNode = new BlockDSPSummerNode(getNumInputChannels(), getNumOutputChannels());
    addNode(summerNode);
    
    return summerNode;
}

BlockDSPDelayLine *BlockDSPSystem::createDelayLine(BlockDSPNode *inputNode) {
    BlockDSPDelayLine *delayLine = new BlockDSPDelayLine(getNumInputChannels());
    delayLine->inputNode = inputNode;
    addDelayLine(delayLine);
    
    return delayLine;
}

BlockDSPMultiplierNode *BlockDSPSystem::createMultiplierNode() {
    BlockDSPMultiplierNode *node = new BlockDSPMultiplierNode(getNumInputChannels(), getNumOutputChannels());
    addNode(node);
    
    return node;
}

BlockDSPInputNode *BlockDSPSystem::createInputNode()
{
    BlockDSPInputNode *node = new BlockDSPInputNode(getNumInputChannels());
    addNode(node);
    
    return node;
}

void BlockDSPSystem::addNumber(const char *name, BlockDSPNumberRef number)
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

BlockDSPNumberRef BlockDSPSystem::numberWithName(const char *name) {
    auto it = _pimpl->numberMap.find(name);
    if (it != _pimpl->numberMap.end()) {
        return it->second;
    }
    return nullptr;
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
    
    BlockDSPDelayLine *inDelayLine = system->createDelayLine(system->mainInputNode);
    inDelayLine->setSize(2);
    BlockDSPSummerNode *summer = system->createSummerNode();
    BlockDSPDelayLine *outDelayLine = system->createDelayLine(summer);
    outDelayLine->setSize(2);
    
    BlockDSPMultiplierNode *a0Node = system->createMultiplierNode();
    BlockDSPMultiplierNode *a1Node = system->createMultiplierNode();
    BlockDSPMultiplierNode *a2Node = system->createMultiplierNode();
    BlockDSPMultiplierNode *b1Node = system->createMultiplierNode();
    BlockDSPMultiplierNode *b2Node = system->createMultiplierNode();
    
    BlockDSPMultiplierNode *outGainNode = system->createMultiplierNode();
    outGainNode->connectInput(summer);
    
    a0Node->connectInput(system->mainInputNode);
    a1Node->connectInput(inDelayLine->nodeForDelayIndex(1));
    a2Node->connectInput(inDelayLine->nodeForDelayIndex(2));
    b1Node->connectInput(outDelayLine->nodeForDelayIndex(1));
    b2Node->connectInput(outDelayLine->nodeForDelayIndex(2));
    
    summer->connectInput(a0Node);
    summer->connectInput(a1Node);
    summer->connectInput(a2Node);
    summer->connectInput(b1Node);
    summer->connectInput(b2Node);
    
    BiQuadCoefficients coeffs;
    coeffs.calculateForLPF(1000, 3.0, sampleRate);
    
    a0Node->coefficient = BlockDSPNumber::numberForFloat(coeffs.a0);
    a1Node->coefficient = BlockDSPNumber::numberForFloat(coeffs.a1);
    a2Node->coefficient = BlockDSPNumber::numberForFloat(coeffs.a2);
    b1Node->coefficient = BlockDSPNumber::numberForFloat(-1 * coeffs.b1);
    b2Node->coefficient = BlockDSPNumber::numberForFloat(-1 * coeffs.b2);
    
    system->mainOutputNode = outGainNode;
    outGainNode->coefficient = BlockDSPNumber::numberForFloat(2.0);
    
    return system;
}

void BlockDSPSystem::reset() {
    for (auto it = _pimpl->delayLineMap.begin(); it != _pimpl->delayLineMap.end(); it++) {
        it->second->reset();
    }
}
