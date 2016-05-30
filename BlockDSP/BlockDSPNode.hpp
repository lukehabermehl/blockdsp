//
//  BlockDSPNode.h
//  Filter
//
//  Created by Luke on 4/2/16.
//
//

#ifndef BlockDSPNode_h
#define BlockDSPNode_h

#include <stdlib.h>
#include <sys/types.h>

class BlockDSPNumber;
class DelayBuffer;

typedef ssize_t BlockDSPNodeID;

class BlockDSPNode {
public:
    BlockDSPNodeID nodeID;
    
    BlockDSPNode(uint32_t numChannels);
    virtual void connectInput(BlockDSPNode *inputNode);
    virtual float valueForChannel(uint32_t channelNo);
    uint32_t getChannelCount();
    
    virtual ~BlockDSPNode();
    
private:
    class pimpl;
    pimpl *_pimpl;
};

class BlockDSPSummerNode : public BlockDSPNode {
public:
    BlockDSPSummerNode(uint32_t numChannels);
    virtual ~BlockDSPSummerNode();
    virtual void connectInput(BlockDSPNode *inputNode);
    virtual float valueForChannel(uint32_t channelNo);
    
private:
    class summerNodePimpl;
    summerNodePimpl *_summerNodePimpl;
};

class BlockDSPMultiplierNode : public BlockDSPNode {
public:
    BlockDSPNumber *coefficient;
    
    BlockDSPMultiplierNode(uint32_t numChannels);
    virtual void connectInput(BlockDSPNode *inputNode);
    virtual float valueForChannel(uint32_t channelNo);
    virtual ~BlockDSPMultiplierNode();
    
private:
    class multiplierNodePimpl;
    multiplierNodePimpl *_multiplierNodePimpl;
};

class BlockDSPDelayLine;

class BlockDSPDelayLineNode : public BlockDSPNode {
public:
    BlockDSPDelayLine *delayLine;
    size_t delayIndex;
    
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
    class delayLinePimpl;
    delayLinePimpl *_delayLinePimpl;
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
