//
//  BlockDSPNumber.cpp
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "BlockDSPNumber.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

BlockDSPNumber::BlockDSPNumber()
{
    strcpy(_data, "0");
}

void BlockDSPNumber::setIntegerValue(int i)
{
    sprintf(_data, "%d", i);
}

void BlockDSPNumber::setFloatValue(float f)
{
    sprintf(_data, "%.7f", f);
}

void BlockDSPNumber::setBoolValue(bool b)
{
    sprintf(_data, "%d", b ? 1 : 0);
}

int BlockDSPNumber::integerValue()
{
    return atoi(_data);
}

float BlockDSPNumber::floatValue()
{
    return strtof(_data, NULL);
}

bool BlockDSPNumber::boolValue()
{
    int i = integerValue();
    return (i != 0);
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