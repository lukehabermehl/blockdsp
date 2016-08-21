//
//  bdsp_node_apu.hpp
//  libblockdsp
//
//  Created by Luke on 8/16/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file bdsp_node_apu.hpp */

#ifndef bdsp_node_apu_hpp
#define bdsp_node_apu_hpp

#include "bdsp_node.hpp"
#include "bdsp_apu.hpp"

class BlockDSPAPUNode : public BlockDSPNode
{
public:
    BlockDSPAPUNode(BlockDSPAPU * apu, uint32_t numInputChannels=1, uint32_t numOutputChannels=1);
    ~BlockDSPAPUNode();
    
    virtual float valueForChannel(uint32_t channelNo);
    virtual void connectInput(BlockDSPNode *inputNode);
    virtual void onNext();
    
    virtual BlockDSPParameterMap& getParameters();
    
private:
    BlockDSPAPU *apu_;
    BlockDSPNode *inputNode_;
    float * inputFrame_;
    float * outputFrame_;
    bool needsUpdate_;
};

#endif /* bdsp_node_apu_hpp */
