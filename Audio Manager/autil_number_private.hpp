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
    int intValue;
    bool boolValue;
    float floatValue;

    APUNumberType paramType;
    std::shared_ptr<APUNumber::pimpl> copy();
};


#endif /* autil_number_private_h */
