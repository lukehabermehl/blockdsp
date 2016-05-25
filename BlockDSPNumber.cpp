//
//  BlockDSPNumber.cpp
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "BlockDSPNumber.hpp"
#include <string.h>
#include <cmath>

BlockDSPNumber::BlockDSPNumber()
{
    _data = 0;
}

void BlockDSPNumber::setIntegerValue(int i)
{
    _data = (float)i;
}

void BlockDSPNumber::setFloatValue(float f)
{
    _data = f;
}

void BlockDSPNumber::setBoolValue(bool b)
{
    _data = b ? 1.0 : 0.0;
}

int BlockDSPNumber::integerValue()
{
    return (int)(floorf(_data));
}

float BlockDSPNumber::floatValue()
{
    return _data;
}

bool BlockDSPNumber::boolValue()
{
    return (_data != 0.0);
}

BlockDSPNumber * BlockDSPNumber::numberForInteger(int i)
{
    BlockDSPNumber *num = new BlockDSPNumber;
    num->setIntegerValue(i);
    
    return num;
}

BlockDSPNumber * BlockDSPNumber::numberForFloat(float f)
{
    BlockDSPNumber *num = new BlockDSPNumber;
    num->setFloatValue(f);
    
    return num;
}

BlockDSPNumber * BlockDSPNumber::numberForBool(bool b)
{
    BlockDSPNumber *num = new BlockDSPNumber;
    num->setBoolValue(b);
    
    return num;
}