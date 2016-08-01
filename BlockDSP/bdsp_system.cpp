//
//  BlockDSPSystem.cpp
//  Filter
//
//  Created by Luke on 4/2/16.
//
//

#include "bdsp_system.hpp"
#include "bdsp_system_private.hpp"
#include "dsphelpers.hpp"
#include "bdsp_number.hpp"

BlockDSPSystem::BlockDSPSystem(uint32_t numChannels) {
    _pimpl = new pimpl;
    _pimpl->numChannels = numChannels;
    _pimpl->counter = 0;
    
    mainInputNode = new BlockDSPInputNode(numChannels);
    mainOutputNode = mainInputNode;
}

BlockDSPSystem::~BlockDSPSystem() {
    for (auto it=_pimpl->nodeMap.begin(); it != _pimpl->nodeMap.end(); it++) {
        BlockDSPNode *node = (*it).second;
        delete node;
    }
    
    for (auto it2=_pimpl->parameterMap.begin(); it2 != _pimpl->parameterMap.end(); it2++)
    {
        BlockDSPParameter *param = (*it2).second;
        delete param;
    }
    
    for (auto it3=_pimpl->numberMap.begin(); it3 != _pimpl->numberMap.end(); it3++)
    {
        BlockDSPNumber *num = (*it3).second;
        delete num;
    }
    
    for (size_t i=0; i<_pimpl->delayLines.size(); i++) {
        delete _pimpl->delayLines[i];
    }
    
    delete _pimpl;
}

void BlockDSPSystem::addNode(BlockDSPNode *node) {
    if (node->nodeID < 0)
        node->nodeID = _pimpl->counter++;
    
    _pimpl->nodeMap[node->nodeID] = node;
}

void BlockDSPSystem::removeNode(BlockDSPNode *node) {
    auto it = _pimpl->nodeMap.find(node->nodeID);
    if (it != _pimpl->nodeMap.end())
        _pimpl->nodeMap.erase(it);
}

void BlockDSPSystem::addDelayLine(BlockDSPDelayLine *dl) {
    dl->id = _pimpl->counter++;
    _pimpl->delayLines.push_back(dl);
}

void BlockDSPSystem::next() {
    for (size_t i=0; i<_pimpl->delayLines.size(); i++) {
        BlockDSPDelayLine *dl = _pimpl->delayLines[i];
        dl->shuffle();
    }
}

BlockDSPSummerNode *BlockDSPSystem::createSummerNode() {
    BlockDSPSummerNode *summerNode = new BlockDSPSummerNode(_pimpl->numChannels);
    addNode(summerNode);
    
    return summerNode;
}

BlockDSPDelayLine *BlockDSPSystem::createDelayLine(BlockDSPNode *inputNode) {
    BlockDSPDelayLine *delayLine = new BlockDSPDelayLine(_pimpl->numChannels);
    delayLine->inputNode = inputNode;
    addDelayLine(delayLine);
    
    return delayLine;
}

BlockDSPMultiplierNode *BlockDSPSystem::createMultiplierNode() {
    BlockDSPMultiplierNode *node = new BlockDSPMultiplierNode(_pimpl->numChannels);
    addNode(node);
    
    return node;
}

BlockDSPInputNode *BlockDSPSystem::createInputNode()
{
    BlockDSPInputNode *node = new BlockDSPInputNode(_pimpl->numChannels);
    addNode(node);
    
    return node;
}

void BlockDSPSystem::addParameter(BlockDSPParameter *param) {
    _pimpl->parameterMap[param->name()] = param;
}

BlockDSPParameter * BlockDSPSystem::createParameter(const char *name, BlockDSPNumberType type, BlockDSPNumber *target)
{
    BlockDSPParameter *param = new BlockDSPParameter(type, name, target, this);
    addParameter(param);
    
    return param;
}

void BlockDSPSystem::addNumber(const char *name, BlockDSPNumber *number)
{
    std::string numName = std::string(name);
    auto it = _pimpl->numberMap.find(numName);
    if (it != _pimpl->numberMap.end())
        return;
    
    _pimpl->numberMap[numName] = number;
}

BlockDSPParameter *BlockDSPSystem::parameterWithName(const char *name) {
    auto it = _pimpl->parameterMap.find(name);
    if (it != _pimpl->parameterMap.end()) {
        return it->second;
    }
    
    return nullptr;
}

BlockDSPNode *BlockDSPSystem::nodeWithID(ssize_t id) {
    auto it = _pimpl->nodeMap.find(id);
    if (it == _pimpl->nodeMap.end())
        return nullptr;
    
    return it->second;
}

BlockDSPNumber * BlockDSPSystem::numberWithName(const char *name) {
    auto it = _pimpl->numberMap.find(name);
    if (it != _pimpl->numberMap.end()) {
        return it->second;
    }
    return nullptr;
}

BlockDSPSystem *BlockDSPSystem::systemForBiQuad(uint32_t numChannels, unsigned int sampleRate) {
    BlockDSPSystem *system = new BlockDSPSystem(numChannels);
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
    
    system->createParameter("a0", BlockDSPNumberType::FLOAT, a0Node->coefficient);
    system->createParameter("a1", BlockDSPNumberType::FLOAT, a1Node->coefficient);
    system->createParameter("a2", BlockDSPNumberType::FLOAT, a2Node->coefficient);
    system->createParameter("b1", BlockDSPNumberType::FLOAT, b1Node->coefficient);
    system->createParameter("b2", BlockDSPNumberType::FLOAT, b2Node->coefficient);
    
    system->mainOutputNode = outGainNode;
    outGainNode->coefficient = BlockDSPNumber::numberForFloat(2.0);
    
    return system;
}

void BlockDSPSystem::reset() {
    for (size_t i=0; i<_pimpl->delayLines.size(); i++) {
        _pimpl->delayLines[i]->reset();
    }
    
    memset(mainInputNode->inputBuffer, 0, sizeof(float) * _pimpl->numChannels);
}
