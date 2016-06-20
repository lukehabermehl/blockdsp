//
//  BlockDSPParameter_Private.hpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef BlockDSPParameter_Private_h
#define BlockDSPParameter_Private_h

#include "bdsp_parameter.hpp"

class BlockDSPParameter::pimpl
{
public:
    ~pimpl();
    
    char name[BDSP_MAX_PARAMLEN];
    BlockDSPNumber *target;
    BlockDSPParameterType type;
    BlockDSPSystem *system;
};


#endif /* BlockDSPParameter_Private_h */
