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
#include <unordered_map>
#include <vector>
#include <string>

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
    void reset();
    
    BlockDSPParameter *parameterWithName(const char *name);
    BlockDSPNode *nodeWithID(ssize_t id);
    
    BlockDSPDelayLine *createDelayLine(BlockDSPNode *inputNode);
    BlockDSPSummerNode *createSummerNode();
    BlockDSPMutiplierNode *createMultiplierNode();
    BlockDSPParameter *createParameter(const char *name, BlockDSPParameterType type, BlockDSPNumber *target);
    
    BlockDSPSystem(uint32_t numChannels);
    ~BlockDSPSystem();
    
private:
    std::unordered_map<ssize_t, BlockDSPNode *> nodeMap;
    std::vector<BlockDSPDelayLine *>delayLines;
    std::unordered_map<std::string, BlockDSPParameter *> parameterMap;
    
    uint32_t counter;
    uint32_t numChannels;
};

#endif /* BlockDSPSystem_hpp */
