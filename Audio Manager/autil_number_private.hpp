//
//  autil_number_private.hpp
//  libblockdsp
//
//  Created by Luke on 8/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef autil_number_private_h
#define autil_number_private_h

#include "autil_number.hpp"

class APUNumber::pimpl
{
public:
    union DataContainer
    {
        int32_t intValue;
        uint32_t unsignedIntValue;
        bool boolValue;
        float floatValue;
    };

    DataContainer data;
    APUNumberType paramType;
    std::shared_ptr<APUNumber::pimpl> copy();
};


#endif /* autil_number_private_h */
