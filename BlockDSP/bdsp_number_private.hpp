//
//  BlockDSPNumber_Private.hpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef BlockDSPNumber_Private_h
#define BlockDSPNumber_Private_h

#include "bdsp_number.hpp"

class BlockDSPNumber::pimpl
{
public:
    int intValue;
    bool boolValue;
    float floatValue;
    
    BlockDSPNumberType paramType;
};

#endif /* BlockDSPNumber_Private_h */
