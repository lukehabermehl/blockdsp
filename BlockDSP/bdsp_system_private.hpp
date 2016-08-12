//
//  BlockDSPSystem_Private.hpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef BlockDSPSystem_Private_h
#define BlockDSPSystem_Private_h

#include "bdsp_system.hpp"

#include <vector>
#include <unordered_map>
#include <string>

class BlockDSPSystem::pimpl
{
public:
    std::unordered_map<BlockDSPNodeID, BlockDSPNode *> nodeMap;
    std::unordered_map<BlockDSPNodeID, BlockDSPDelayLine *> delayLineMap;
    std::unordered_map<std::string, BlockDSPParameter *> parameterMap;
    std::unordered_map<std::string, BlockDSPNumberRef> numberMap;
    
    uint32_t counter;
    uint32_t inputChannelCount;
    uint32_t outputChannelCount;
};

#endif /* BlockDSPSystem_Private_h */
