//
//  BlockDSPSystem.hpp
//  Filter
//
//  Created by Luke on 4/2/16.
//
//

#ifndef BlockDSPSystem_hpp
#define BlockDSPSystem_hpp

#include "BlockDSPNode.hpp"
#include "BlockDSPParameter.hpp"
#include "BlockDSPNumber.hpp"

class BlockDSPSystem {
public:
    BlockDSPInputNode *mainInputNode;
    BlockDSPNode *mainOutputNode;
    
    static BlockDSPSystem *systemForBiQuad(uint32_t numChannels);
    
    void addNode(BlockDSPNode *node);
    void removeNode(BlockDSPNode *node);
    void addDelayLine(BlockDSPDelayLine *dl);
    void next();
    void addParameter(BlockDSPParameter *param);
    void addNumber(const char *name, BlockDSPNumber *number);
    void reset();
    
    BlockDSPParameter *parameterWithName(const char *name);
    BlockDSPNode *nodeWithID(BlockDSPNodeID id);
    
    BlockDSPDelayLine *createDelayLine(BlockDSPNode *inputNode);
    BlockDSPSummerNode *createSummerNode();
    BlockDSPMultiplierNode *createMultiplierNode();
    BlockDSPInputNode *createInputNode();
    BlockDSPParameter *createParameter(const char *name, BlockDSPParameterType type, BlockDSPNumber *target);
    
    BlockDSPSystem(uint32_t numChannels);
    ~BlockDSPSystem();
    
private:
    class pimpl;
    pimpl *_pimpl;
};

#endif /* BlockDSPSystem_hpp */
