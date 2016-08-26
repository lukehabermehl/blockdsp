//
//  autil_number.cpp
//  libblockdsp
//
//  Created by Luke on 8/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "autil_number.hpp"
#include "autil_number_private.hpp"
#include <cmath>

APUNumber::APUNumber()
{
    _pimpl.reset(new pimpl);
    _pimpl->paramType = APUNumberType::APUNUM_INTEGER;
    _pimpl->intValue = 0;
}

APUNumber::APUNumber(const APUNumber& num)
{
    _pimpl = num._pimpl;
}

APUNumber& APUNumber::operator=(const APUNumber &num)
{
    _pimpl = num._pimpl->copy();
    return *this;
}

APUNumber::~APUNumber()
{
}

void APUNumber::setIntegerValue(int i)
{
    _pimpl->intValue = i;
    _pimpl->paramType = APUNumberType::APUNUM_INTEGER;
}

void APUNumber::setFloatValue(float f)
{
    _pimpl->floatValue = f;
    _pimpl->paramType = APUNumberType::APUNUM_FLOAT;
}

void APUNumber::setBoolValue(bool b)
{
    _pimpl->boolValue = b;
    _pimpl->paramType = APUNumberType::APUNUM_BOOLEAN;
}

int APUNumber::integerValue()
{
    switch (_pimpl->paramType) {
        case APUNumberType::APUNUM_INTEGER:
            return _pimpl->intValue;
        case APUNumberType::APUNUM_FLOAT:
            return (int)floorf(_pimpl->floatValue);
        case APUNumberType::APUNUM_BOOLEAN:
            return _pimpl->boolValue ? 1 : 0;
        default:
            return 0;
    }
}

float APUNumber::floatValue()
{
    switch (_pimpl->paramType) {
        case APUNumberType::APUNUM_INTEGER:
            return (float)(_pimpl->intValue);
        case APUNumberType::APUNUM_FLOAT:
            return _pimpl->floatValue;
        case APUNumberType::APUNUM_BOOLEAN:
            return _pimpl->boolValue ? 1.f : 0.f;
        default:
            return 0;
    }
}

bool APUNumber::boolValue()
{
    int i = integerValue();
    return (i != 0);
}

APUNumber APUNumber::numberForInteger(int i)
{
    APUNumber num;
    num.setIntegerValue(i);

    return num;
}

APUNumber APUNumber::numberForFloat(float f)
{
    APUNumber num;
    num.setFloatValue(f);

    return num;
}

APUNumber APUNumber::numberForBool(bool b)
{
    APUNumber num;
    num.setBoolValue(b);

    return num;
}

std::shared_ptr<APUNumber::pimpl> APUNumber::pimpl::copy()
{
    std::shared_ptr<pimpl> cpy = std::make_shared<pimpl>();
    cpy->boolValue = boolValue;
    cpy->intValue = intValue;
    cpy->floatValue = floatValue;
    cpy->paramType = paramType;

    return cpy;
}

