//
//  libblockdsp
//
//  Created by Luke on 4/2/16.
//
//

/** @file bdsp_node.hpp */

#ifndef BlockDSPNode_h
#define BlockDSPNode_h

#include <stdlib.h>
#include <sys/types.h>

class BlockDSPNumber;
class DelayBuffer;

/** typedef for BlockDSP node identifiers */
typedef ssize_t BlockDSPNodeID;

/** A single processing block that can be connected to other nodes to form a processing chain */
class BlockDSPNode {
public:
    /** The node's identifier. This is just an incrementing number and is only associated with the node for the lifespan of the containing system */
    BlockDSPNodeID nodeID;
    /** Constructor
      * @param numChannels Number of output channels
      */
    BlockDSPNode(uint32_t numChannels);
    /** Connect a node to this node's input
      * @param inputNode node to connect
      */
    virtual void connectInput(BlockDSPNode *inputNode);
    /** Get the output value for the given channel */
    virtual float valueForChannel(uint32_t channelNo);
    /** Get the number of output channels */
    uint32_t getChannelCount();
    
    virtual ~BlockDSPNode();
    
private:
    class pimpl;
    pimpl *_pimpl;
};

/** Performs a summing operation. Supports virtually any number of input nodes */
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

/** Performs multiplication with a coefficient. Supports **one** input node */
class BlockDSPMultiplierNode : public BlockDSPNode {
public:
    /** The coefficient number */
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

/** Represents a single index of a delay line. Does not support input nodes */
class BlockDSPDelayLineNode : public BlockDSPNode {
public:
    /** The parent delay line */
    BlockDSPDelayLine *delayLine;
    /** The index of the delay node */
    size_t delayIndex;
    
    BlockDSPDelayLineNode(uint32_t numChannels);
    /** Is no-op for delay line nodes */
    virtual void connectInput(BlockDSPNode *inputNode);
    virtual float valueForChannel(uint32_t channelNo);
};

/** A delay line implementation */
class BlockDSPDelayLine {
public:
    /** Like a block ID, only for delay lines */
    ssize_t id;
    /** Constructor
      * @param numChannels number of output channels */
    BlockDSPDelayLine(uint32_t numChannels);
    ~BlockDSPDelayLine();
    
    /** Input node of the delay line */
    BlockDSPNode *inputNode;
    /** Get the node at the given index. Note that the memory allocated for each node will be freed in the destructor of the parent delay line */
    BlockDSPDelayLineNode *nodeForDelayIndex(size_t index);
    /** Get the sample value at the given delay index and channel */
    float valueForDelayIndex(size_t delayIndex, uint32_t channelNo);
    /** Set the delay length in samples */
    void setSize(size_t delaySize);
    /** Pull the next sample from the input node and push it into the delay line */
    void shuffle();
    /** Clear the delay line */
    void reset();
    
private:
    class delayLinePimpl;
    delayLinePimpl *_delayLinePimpl;
};

/** Allows custom input. Does not support input nodes */
class BlockDSPInputNode : public BlockDSPNode {
public:
    BlockDSPInputNode(uint32_t numChannels);
    ~BlockDSPInputNode();
    /** This can be set to a custom array of floating point samples. The array should represent one frame of audio */
    float *inputBuffer;
    /** Is no-op for input nodes */
    virtual void connectInput(BlockDSPNode *inputNode);
    virtual float valueForChannel(uint32_t channelNo);
};


#endif /* BlockDSPNode_h */
