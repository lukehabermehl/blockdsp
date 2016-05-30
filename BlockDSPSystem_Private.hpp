//
//  BlockDSPSystem_Private.hpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef BlockDSPSystem_Private_h
#define BlockDSPSystem_Private_h

#include "BlockDSPSystem.hpp"

class BlockDSPSystem::pimpl
{
public:
    std::unordered_map<BlockDSPNodeID, BlockDSPNode *> nodeMap;
    std::vector<BlockDSPDelayLine *>delayLines;
    std::unordered_map<std::string, BlockDSPParameter *> parameterMap;
    std::unordered_map<std::string, BlockDSPNumber *> numberMap;
    
    uint32_t counter;
    uint32_t numChannels;
};

#endif /* BlockDSPSystem_Private_h */
