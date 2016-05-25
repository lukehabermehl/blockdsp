//
//  BlockDSPNode.h
//  Filter
//
//  Created by Luke on 4/2/16.
//
//

#ifndef BlockDSPNode_h
#define BlockDSPNode_h

#include <vector>
#include "DelayBuffer.h"
#include "BlockDSPNumber.hpp"

class BlockDSPNode {
public:
    ssize_t nodeID;
    
    BlockDSPNode(uint32_t numChannels);
    virtual void connectInput(BlockDSPNode *inputNode);
    virtual float valueForChannel(uint32_t channelNo);
    uint32_t getChannelCount();
    
    virtual ~BlockDSPNode();
    
private:
    uint32_t numChannels;
};

class BlockDSPSummerNode : public BlockDSPNode {
public:
    BlockDSPSummerNode(uint32_t numChannels);
    virtual void connectInput(BlockDSPNode *inputNode);
    virtual float valueForChannel(uint32_t channelNo);
    
private:
    std::vector<BlockDSPNode *>inputNodes;
};

class BlockDSPMutiplierNode : public BlockDSPNode {
public:
    BlockDSPNumber *coefficient;
    
    BlockDSPMutiplierNode(uint32_t numChannels);
    virtual void connectInput(BlockDSPNode *inputNode);
    virtual float valueForChannel(uint32_t channelNo);
    virtual ~BlockDSPMutiplierNode();
    
private:
    BlockDSPNode *inputNode;
};

class BlockDSPDelayLine;

class BlockDSPDelayLineNode : public BlockDSPNode {
public:
    BlockDSPDelayLine *delayLine = nullptr;
    size_t delayIndex = 0;
    
    BlockDSPDelayLineNode(uint32_t numChannels);
    virtual void connectInput(BlockDSPNode *inputNode);
    virtual float valueForChannel(uint32_t channelNo);
};

class BlockDSPDelayLine {
public:
    ssize_t id;
    BlockDSPDelayLine(uint32_t numChannels);
    ~BlockDSPDelayLine();
    
    BlockDSPNode *inputNode;
    BlockDSPDelayLineNode *nodeForDelayIndex(size_t index);
    float valueForDelayIndex(size_t delayIndex, uint32_t channelNo);
    void setSize(size_t delaySize);
    void shuffle();
    void reset();
    
private:
    DelayBuffer *delayBuffers;
    uint32_t numChannels;
};

class BlockDSPInputNode : public BlockDSPNode {
public:
    BlockDSPInputNode(uint32_t numChannels);
    ~BlockDSPInputNode();
    float *inputBuffer;
    
    virtual void connectInput(BlockDSPNode *inputNode);
    virtual float valueForChannel(uint32_t channelNo);
};


#endif /* BlockDSPNode_h */
