//
//  BlockDSPNumber.cpp
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "BlockDSPNumber.hpp"
#include "BlockDSPNumber_Private.hpp"
#include <stdlib.h>
#include <string.h>
#include <cmath>

static const size_t INT_SIZE = sizeof(int);
static const size_t FLOAT_SIZE = sizeof(float);
static const size_t BOOL_SIZE = sizeof(bool);

BlockDSPNumber::BlockDSPNumber()
{
    _pimpl = new pimpl;
    memset(_pimpl->data, 0, pimpl::dataSize);
}

BlockDSPNumber::~BlockDSPNumber()
{
    delete _pimpl;
}

void BlockDSPNumber::setIntegerValue(int i)
{
    memset(_pimpl->data, 0, pimpl::dataSize);
    memcpy(_pimpl->data, &i, INT_SIZE);
}

void BlockDSPNumber::setFloatValue(float f)
{
    memset(_pimpl->data, 0, pimpl::dataSize);
    memcpy(&_pimpl->data[pimpl::dataSize - FLOAT_SIZE], &f, FLOAT_SIZE);
}

void BlockDSPNumber::setBoolValue(bool b)
{
    memset(_pimpl->data, 0, pimpl::dataSize);
    memcpy(&_pimpl->data[pimpl::dataSize - BOOL_SIZE], &b, BOOL_SIZE);
}

int BlockDSPNumber::integerValue()
{
    float f;
    memcpy(&f, &_pimpl->data[pimpl::dataSize - FLOAT_SIZE], FLOAT_SIZE);
    return (int)f;
}

float BlockDSPNumber::floatValue()
{
    float f;
    memcpy(&f, &_pimpl->data[pimpl::dataSize - FLOAT_SIZE], FLOAT_SIZE);
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