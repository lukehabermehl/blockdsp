//
//  libblockdsp
//
//  Created by Luke on 4/2/16.
//
//

/** @file bdsp_node.hpp 
  * This file defines the basic BlockDSP processing blocks
  */

#ifndef BlockDSPNode_h
#define BlockDSPNode_h

#include <aputils/autil_number.hpp>
#include <aputils/autil_parameter.hpp>

#include <stdlib.h>
#include <sys/types.h>

#define kInvalidNodeID -1

class DelayBuffer;

/** typedef for BlockDSP node identifiers */
typedef ssize_t BlockDSPNodeID;

/** A single processing block that can be connected to other nodes to form a processing chain */
class BlockDSPNode {
    //Allow BlockDSPSystem instances to call setID()
    friend class BlockDSPSystem;
public:
    /** The node's identifier. This is just an incrementing number and is only associated with the node for the lifespan of the containing system */
    BlockDSPNodeID getID();
    /** Constructor
      * @param numChannels Number of output channels
      */
    BlockDSPNode(BlockDSPNodeID nodeID, uint32_t numInputChannels=1, uint32_t numOutputChannels=1);
    /** Connect a node to this node's input
      * @param inputNode node to connect
      */
    virtual void connectInput(BlockDSPNode *inputNode);
    /** Get the output value for the given channel */
    virtual float valueForChannel(uint32_t channelNo);
    /** Get the number of output channels */
    uint32_t getNumOutputChannels();
    /** Get the number of input channels */
    uint32_t getNumInputChannels();
    /** Get the number of output channels */
    void setNumOutputChannels(uint32_t num);
    /** Get the number of input channels */
    void setNumInputChannels(uint32_t num);

    /** Turn on/off bypass setting
      * @param active true if bypass is on (node becomes passthrough). False if off (node processing is active)
      */
    virtual void setBypass(bool active);
    /** @return true if the node is bypassed. Otherwise false */
    virtual bool isBypassed();
    
    /** Called when the system advances to the next frame. Override this if you need to handle the event */
    virtual void onNext();
    
    virtual ~BlockDSPNode();
    
private:
    class pimpl;
    pimpl *_pimpl;
    
    void setID(BlockDSPNodeID nId);
};

/** Performs a summing operation. Supports virtually any number of input nodes */
class BlockDSPSummerNode : public BlockDSPNode {
public:
    BlockDSPSummerNode(BlockDSPNodeID nodeID, uint32_t numInputChannels=1, uint32_t numOutputChannels=1);
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
    APUNumber coefficient;
    /** If true, use the provided parameter as the coefficient value */
    bool useParameter;
    APUParameter *parameter;
    
    BlockDSPMultiplierNode(BlockDSPNodeID nodeID, uint32_t numInputChannels=1, uint32_t numOutputChannels=1);
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
    
    /** Constructor. For delay line nodes, the input and output channel count should be equal */
    BlockDSPDelayLineNode(BlockDSPNodeID nodeID, uint32_t numChannels);
    /** Is no-op for delay line nodes */
    virtual void connectInput(BlockDSPNode *inputNode);
    virtual float valueForChannel(uint32_t channelNo);
};

/** A delay line implementation */
class BlockDSPDelayLine {
    friend class BlockDSPSystem;
public:
    /** Like a block ID, only for delay lines */
    BlockDSPNodeID getID();
    /** Constructor
      * @param numChannels number of output channels */
    BlockDSPDelayLine(BlockDSPNodeID delayLineID, uint32_t numChannels);
    ~BlockDSPDelayLine();
    
    /** Input node of the delay line */
    BlockDSPNode *inputNode;
    /** Get the node at the given index. Note that the memory allocated for each node will be freed in the destructor of the parent delay line */
    BlockDSPDelayLineNode *nodeForDelayIndex(BlockDSPNodeID nodeID, size_t index);
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
    
    void setID(BlockDSPNodeID nID);
};

/** Allows custom input. Does not support input nodes */
class BlockDSPInputNode : public BlockDSPNode {
public:
    /** Constructor. Since input is manual, only the output channel count is relevant */
    BlockDSPInputNode(BlockDSPNodeID nodeID, uint32_t numOutputChannels);
    ~BlockDSPInputNode();
    /** Set the node to pull samples from (optional)
      * @param node if NULL, use the input buffer, else use the output of the source node
      */
    void setSourceNode(BlockDSPNode *node);
    /** This must be set to a custom array of floating point samples with a length equal to the number of output channels. The array should represent one frame of audio */
    float *inputBuffer;
    /** Maps to setSourceNode for Input Nodes */
    virtual void connectInput(BlockDSPNode *inputNode);
    virtual float valueForChannel(uint32_t channelNo);

private:
    class inputNodePimpl;
    inputNodePimpl *_inputNodePimpl;
};


#endif /* BlockDSPNode_h */
