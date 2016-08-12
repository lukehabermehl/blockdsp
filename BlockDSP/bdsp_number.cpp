//
//  BlockDSPNumber.cpp
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "bdsp_number.hpp"
#include "bdsp_number_private.hpp"
#include "bdsp_parameter.hpp"
#include <cmath>

BlockDSPNumber::BlockDSPNumber()
{
    _pimpl = new pimpl;
    _pimpl->paramType = BlockDSPNumberType::INTEGER;
    _pimpl->intValue = 0;
}

BlockDSPNumber::~BlockDSPNumber()
{
    delete _pimpl;
}

void BlockDSPNumber::setIntegerValue(int i)
{
    _pimpl->intValue = i;
    _pimpl->paramType = BlockDSPNumberType::INTEGER;
}

void BlockDSPNumber::setFloatValue(float f)
{
    _pimpl->floatValue = f;
    _pimpl->paramType = BlockDSPNumberType::FLOAT;
}

void BlockDSPNumber::setBoolValue(bool b)
{
    _pimpl->boolValue = b;
    _pimpl->paramType = BlockDSPNumberType::BOOLEAN;
}

int BlockDSPNumber::integerValue()
{
    switch (_pimpl->paramType) {
        case BlockDSPNumberType::INTEGER:
            return _pimpl->intValue;
        case BlockDSPNumberType::FLOAT:
            return (int)floorf(_pimpl->floatValue);
        case BlockDSPNumberType::BOOLEAN:
            return _pimpl->boolValue ? 1 : 0;
        default:
            return 0;
    }
}

float BlockDSPNumber::floatValue()
{
    switch (_pimpl->paramType) {
        case BlockDSPNumberType::INTEGER:
            return (float)(_pimpl->intValue);
        case BlockDSPNumberType::FLOAT:
            return _pimpl->floatValue;
        case BlockDSPNumberType::BOOLEAN:
            return _pimpl->boolValue ? 1.f : 0.f;
        default:
            return 0;
    }
}

bool BlockDSPNumber::boolValue()
{
    int i = integerValue();
    return (i != 0);
}

BlockDSPNumberRef  BlockDSPNumber::numberForInteger(int i)
{
    BlockDSPNumberRef num(new BlockDSPNumber());
    num->setIntegerValue(i);
    
    return num;
}

BlockDSPNumberRef BlockDSPNumber::numberForFloat(float f)
{
    BlockDSPNumberRef num(new BlockDSPNumber());
    num->setFloatValue(f);
    
    return num;
}

BlockDSPNumberRef BlockDSPNumber::numberForBool(bool b)
{
    BlockDSPNumberRef num(new BlockDSPNumber());
    num->setBoolValue(b);
    
    return num;
}