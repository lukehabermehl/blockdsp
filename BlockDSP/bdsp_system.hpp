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
#include "bdsp_parameter.hpp"
#include "bdsp_number.hpp"

/** The container for all the objects associated with the audio processing chain */
class BlockDSPSystem {
public:
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
    /** Add a parameter */
    void addParameter(BlockDSPParameter *param);
    /** Add a number
      * @param name a name for the number (ex. "a1")
      * @param number the number to add
      */
    void addNumber(const char *name, BlockDSPNumber *number);
    /** Pull the next sample from the input node and push it into the system */
    void next();
    /** Zero out the system. This will call reset() on any delay lines */
    void reset();
    
    /** Get the parameter with the given name
      * @returns a pointer to the parameter or NULL if it is not found 
      */
    BlockDSPParameter *parameterWithName(const char *name);
    /** Get the node with the given ID
      * @returns a pointer to the node or NULL if it is not found
      */
    BlockDSPNode *nodeWithID(BlockDSPNodeID id);
    /** Create a new delay line and add it to the system 
      * @param inputNode the input to the delay line
      */
    BlockDSPDelayLine *createDelayLine(BlockDSPNode *inputNode);
    /** Create a new summer node and add it to the system */
    BlockDSPSummerNode *createSummerNode();
    /** Create a new multiplier node and add it to the system */
    BlockDSPMultiplierNode *createMultiplierNode();
    /** Create a new input node and add it to the system */
    BlockDSPInputNode *createInputNode();
    /** Create a new parameter and add it to the system
      * @param name the name of the parameter
      * @param type the value type
      * @param target optional target number for the parameter. See the documentation for BlockDSPParameter
      */
    BlockDSPParameter *createParameter(const char *name, BlockDSPParameterType type, BlockDSPNumber *target);
    
    BlockDSPSystem(uint32_t numChannels);
    ~BlockDSPSystem();
    
private:
    class pimpl;
    pimpl *_pimpl;
};

#endif /* BlockDSPSystem_hpp */
