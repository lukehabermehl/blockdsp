//
//  BlockDSPNode_Private.hpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef BlockDSPNode_Private_h
#define BlockDSPNode_Private_h

#include "bdsp_node.hpp"
#include <vector>

class BlockDSPNode::pimpl
{
public:
    uint32_t numInputChannels;
    uint32_t numOutputChannels;
};

class BlockDSPSummerNode::summerNodePimpl
{
public:
    std::vector<BlockDSPNode *>inputNodes;
};

class BlockDSPMultiplierNode::multiplierNodePimpl
{
public:
    BlockDSPNode *inputNode;
};

class BlockDSPDelayLine::delayLinePimpl
{
public:
    DelayBuffer *delayBuffers;
    uint32_t numChannels;
};


#endif /* BlockDSPNode_Private_h */
