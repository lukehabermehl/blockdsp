//
//  autil_number.cpp
//  libblockdsp
//
//  Created by Luke on 8/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "autil_number.hpp"
#include <cmath>

APUNumber::APUNumber()
{
    data_.intValue = 0;
    numberType_ = APUNUM_INTEGER;
}

APUNumber::APUNumber(float f)
{
    setFloatValue(f);
}

APUNumber APUNumber::copy()
{
    APUNumber num = (*this);
    return num;
}

APUNumberType APUNumber::getType() const
{
    return numberType_;
}

void APUNumber::setIntegerValue(int32_t i)
{
    data_.intValue = i;
    numberType_ = APUNumberType::APUNUM_INTEGER;
}

void APUNumber::setUnsignedIntValue(uint32_t u)
{
    data_.unsignedIntValue = u;
    numberType_ = APUNumberType::APUNUM_UINT;
}

void APUNumber::setFloatValue(float f)
{
    data_.floatValue = f;
    numberType_ = APUNumberType::APUNUM_FLOAT;
}

void APUNumber::setBoolValue(bool b)
{
    data_.boolValue = b;
    numberType_ = APUNumberType::APUNUM_BOOLEAN;
}

int32_t APUNumber::integerValue() const
{
    switch (numberType_) {
        case APUNumberType::APUNUM_INTEGER:
            return data_.intValue;
        case APUNumberType::APUNUM_UINT:
            return data_.unsignedIntValue;
        case APUNumberType::APUNUM_FLOAT:
            return (int)floorf(data_.floatValue);
        case APUNumberType::APUNUM_BOOLEAN:
            return data_.boolValue ? 1 : 0;
        default:
            return 0;
    }
}

uint32_t APUNumber::unsignedIntValue() const
{
    switch(numberType_) {
        case APUNumberType::APUNUM_UINT:
            return data_.unsignedIntValue;
        default:
            return integerValue();
    }
}

float APUNumber::floatValue() const
{
    switch (numberType_) {
        case APUNumberType::APUNUM_INTEGER:
            return (float)(data_.intValue);
        case APUNumberType::APUNUM_UINT:
            return (float)(data_.unsignedIntValue);
        case APUNumberType::APUNUM_FLOAT:
            return data_.floatValue;
        case APUNumberType::APUNUM_BOOLEAN:
            return data_.boolValue ? 1.f : 0.f;
        default:
            return 0;
    }
}

bool APUNumber::boolValue() const
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

APUNumber APUNumber::numberForUnsignedInt(uint32_t u)
{
    APUNumber num;
    num.setUnsignedIntValue(u);

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
