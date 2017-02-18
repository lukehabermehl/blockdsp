//
//  libblockdsp
//
//  Created by Luke on 4/2/16.
//
//

/** @file bdsp_system.hpp */

#ifndef BlockDSPSystem_hpp
#define BlockDSPSystem_hpp

#include "bdsp_node.hpp"

class BlockDSPParameter;
class BlockDSPAPU;

/** The container for all the objects associated with the audio processing chain */
class BlockDSPSystem {
public:
    /** Get the number of input channels in use in the system */
    uint32_t getNumInputChannels();
    /** Set the number of input channels in use in the system. Make sure that this is not more than the
        maximum input channel count for the input source */
    void setNumInputChannels(uint32_t num);
    /** Get the number of output channels in use in the system */
    uint32_t getNumOutputChannels();
    /** Set the number of output channels in use in the system. Make sure that this is not more than the maximum output channel count of the output device */
    void setNumOutputChannels(uint32_t num);
    /** Entry point for audio from a file or input device */
    BlockDSPInputNode *mainInputNode;
    /** Output node to the output device */
    BlockDSPNode *mainOutputNode;
    
    /** TEST FUNCTION -- create a prepackaged system for a BiQuad filter */
    static BlockDSPSystem *systemForBiQuad(uint32_t numChannels, unsigned int sampleRate);
    
    /** Add a node to the system. Once added, the node's memory will be managed and automatically deleted by the system */
    void addNode(BlockDSPNode *node);
    /** Remove a node from the system. Removing the node will give you back control of the node's memory so be careful */
    void removeNode(BlockDSPNode *node);
    /** Add a delay line */
    void addDelayLine(BlockDSPDelayLine *dl);
    /** Add a number
      * @param name a name for the number (ex. "a1")
      * @param number the number to add
      */
    void addNumber(const char *name, APUNumber number);
    /** Pull the next sample from the input node and push it into the system */
    void next();
    /** Zero out the system. This will call reset() on any delay lines */
    void reset();
    
    /** Get the node with the given ID
      * @returns a pointer to the node or NULL if it is not found
      */
    BlockDSPNode *nodeWithID(BlockDSPNodeID id);
    /** Get the number with the given name
      * @param name the name of the number
      * @param inout flag that will be set to true if the number exists
      * @returns the number or 0 if not found
      */
    APUNumber numberWithName(const char *name, bool &success);
    /** Get the delay line with the given ID
      * @returns the delay line or NULL if not found
      */
    BlockDSPDelayLine *delayLineWithID(BlockDSPNodeID id);
    /** Create a new delay line and add it to the system 
      * @param delayLineID the ID to assign to the delay line
      * @param inputNode the input to the delay line
      */
    BlockDSPDelayLine *createDelayLine(BlockDSPNodeID delayLineID, BlockDSPNode *inputNode);
    /** Create a new summer node and add it to the system */
    BlockDSPSummerNode *createSummerNode(BlockDSPNodeID nodeID);
    /** Create a new multiplier node and add it to the system */
    BlockDSPMultiplierNode *createMultiplierNode(BlockDSPNodeID nodeID);
    /** Create a new input node and add it to the system */
    BlockDSPInputNode *createInputNode(BlockDSPNodeID nodeID);
    
    BlockDSPSystem();
    ~BlockDSPSystem();
    
private:
    class pimpl;
    pimpl *_pimpl;
};

#endif /* BlockDSPSystem_hpp */
