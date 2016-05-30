//
//  BlockDSPNumber.cpp
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "BlockDSPNumber.hpp"
#include <stdlib.h>
#include <string.h>
#include <cmath>

static const int DATA_SIZE = 8;
static const size_t INT_SIZE = sizeof(int);
static const size_t FLOAT_SIZE = sizeof(float);
static const size_t BOOL_SIZE = sizeof(bool);

BlockDSPNumber::BlockDSPNumber()
{
    memset(_data, 0, DATA_SIZE);
}

void BlockDSPNumber::setIntegerValue(int i)
{
    memset(_data, 0, DATA_SIZE);
    memcpy(_data, &i, INT_SIZE);
}

void BlockDSPNumber::setFloatValue(float f)
{
    memset(_data, 0, DATA_SIZE);
    memcpy(&_data[DATA_SIZE - FLOAT_SIZE], &f, FLOAT_SIZE);
}

void BlockDSPNumber::setBoolValue(bool b)
{
    memset(_data, 0, DATA_SIZE);
    memcpy(&_data[DATA_SIZE - BOOL_SIZE], &b, BOOL_SIZE);
}

int BlockDSPNumber::integerValue()
{
    float f;
    memcpy(&f, &_data[DATA_SIZE - FLOAT_SIZE], FLOAT_SIZE);
    return (int)f;
}

float BlockDSPNumber::floatValue()
{
    float f;
    memcpy(&f, &_data[DATA_SIZE - FLOAT_SIZE], FLOAT_SIZE);
    return f;
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