//
//  BlockDSPSystem.cpp
//  Filter
//
//  Created by Luke on 4/2/16.
//
//

#include "BlockDSPSystem.hpp"
#include "dsphelpers.hpp"

BlockDSPSystem::BlockDSPSystem(uint32_t numChannels) {
    this->numChannels = numChannels;
    counter = 0;
    
    mainInputNode = new BlockDSPInputNode(numChannels);
    mainOutputNode = mainInputNode;
}

BlockDSPSystem::~BlockDSPSystem() {
    for (auto it=nodeMap.begin(); it != nodeMap.end(); it++) {
        BlockDSPNode *node = (*it).second;
        delete node;
    }
    
    for (size_t i=0; i<delayLines.size(); i++) {
        delete delayLines[i];
    }
}

void BlockDSPSystem::addNode(BlockDSPNode *node) {
    if (node->nodeID < 0)
        node->nodeID = counter++;
    
    nodeMap[node->nodeID] = node;
}

void BlockDSPSystem::removeNode(BlockDSPNode *node) {
    auto it = nodeMap.find(node->nodeID);
    if (it != nodeMap.end())
        nodeMap.erase(it);
}

void BlockDSPSystem::addDelayLine(BlockDSPDelayLine *dl) {
    dl->id = counter++;
    delayLines.push_back(dl);
}

void BlockDSPSystem::next() {
    for (size_t i=0; i<delayLines.size(); i++) {
        BlockDSPDelayLine *dl = delayLines[i];
        dl->shuffle();
    }
}

BlockDSPSummerNode *BlockDSPSystem::createSummerNode() {
    BlockDSPSummerNode *summerNode = new BlockDSPSummerNode(numChannels);
    addNode(summerNode);
    
    return summerNode;
}

BlockDSPDelayLine *BlockDSPSystem::createDelayLine(BlockDSPNode *inputNode) {
    BlockDSPDelayLine *delayLine = new BlockDSPDelayLine(numChannels);
    delayLine->inputNode = inputNode;
    addDelayLine(delayLine);
    
    return delayLine;
}

BlockDSPMutiplierNode *BlockDSPSystem::createMultiplierNode() {
    BlockDSPMutiplierNode *node = new BlockDSPMutiplierNode(numChannels);
    addNode(node);
    
    return node;
}

void BlockDSPSystem::addParameter(BlockDSPParameter *param) {
    parameterMap[param->name()] = param;
}

BlockDSPParameter *BlockDSPSystem::parameterWithName(const char *name) {
    auto it = parameterMap.find(name);
    if (it != parameterMap.end()) {
        return it->second;
    }
    
    return nullptr;
}

BlockDSPNode *BlockDSPSystem::nodeWithID(ssize_t id) {
    auto it = nodeMap.find(id);
    if (it == nodeMap.end())
        return nullptr;
    
    return it->second;
}

BlockDSPSystem *BlockDSPSystem::systemForBiQuad(uint32_t numChannels) {
    BlockDSPSystem *system = new BlockDSPSystem(numChannels);
    BlockDSPDelayLine *inDelayLine = system->createDelayLine(system->mainInputNode);
    inDelayLine->setSize(2);
    BlockDSPSummerNode *summer = system->createSummerNode();
    BlockDSPDelayLine *outDelayLine = system->createDelayLine(summer);
    outDelayLine->setSize(2);
    
    BlockDSPMutiplierNode *a0Node = system->createMultiplierNode();
    BlockDSPMutiplierNode *a1Node = system->createMultiplierNode();
    BlockDSPMutiplierNode *a2Node = system->createMultiplierNode();
    BlockDSPMutiplierNode *b1Node = system->createMultiplierNode();
    BlockDSPMutiplierNode *b2Node = system->createMultiplierNode();
    
    BlockDSPMutiplierNode *outGainNode = system->createMultiplierNode();
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
    coeffs.calculateForLPF(1000, 3.0, 44100);

    
    a0Node->coefficient = BlockDSPNumber::numberForFloat(coeffs.a0);
    a1Node->coefficient = BlockDSPNumber::numberForFloat(coeffs.a1);
    a2Node->coefficient = BlockDSPNumber::numberForFloat(coeffs.a2);
    b1Node->coefficient = BlockDSPNumber::numberForFloat(-1 * coeffs.b1);
    b2Node->coefficient = BlockDSPNumber::numberForFloat(-1 * coeffs.b2);
    
    system->addParameter(new BlockDSPParameter(BlockDSPParameterTypeFloat, "a0", a0Node->coefficient, system));
    system->addParameter(new BlockDSPParameter(BlockDSPParameterTypeFloat, "a1", a1Node->coefficient, system));
    system->addParameter(new BlockDSPParameter(BlockDSPParameterTypeFloat, "a2", a2Node->coefficient, system));
    system->addParameter(new BlockDSPParameter(BlockDSPParameterTypeFloat, "b1", b1Node->coefficient, system));
    system->addParameter(new BlockDSPParameter(BlockDSPParameterTypeFloat, "b2", b2Node->coefficient, system));
    
    printf("a0 coeff: %f\n", a0Node->coefficient->floatValue());
    printf("a1 coeff: %f\n", a1Node->coefficient->floatValue());
    
    system->mainOutputNode = outGainNode;
    outGainNode->coefficient = BlockDSPNumber::numberForFloat(2.0);
    
    return system;
}

void BlockDSPSystem::reset() {
    for (size_t i=0; i<delayLines.size(); i++) {
        delayLines[i]->reset();
    }
    
    memset(mainInputNode->inputBuffer, 0, sizeof(float) * numChannels);
}
