//
//  BlockDSPNumber_Private.hpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef BlockDSPNumber_Private_h
#define BlockDSPNumber_Private_h

#include "BlockDSPNumber.hpp"

class BlockDSPNumber::pimpl
{
public:
    static const int dataSize = 8;
    unsigned char data[dataSize];
};

#endif /* BlockDSPNumber_Private_h */
