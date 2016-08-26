//
//  BlockDSPParameter.cpp
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "bdsp_parameter.hpp"
#include "bdsp_parameter_private.hpp"
#include "bdsp_number.hpp"
#include "bdsp_apu.hpp"
#include <string.h>
#include <limits.h>

BlockDSPParameter::BlockDSPParameter(APUNumberType type, const char *name, APUNumber target, BlockDSPAPU *contextAPU)
{
    _pimpl = new pimpl;
    _pimpl->type = type;
    setTarget(target);
    setName(name);
    callback = 0;
    _pimpl->contextAPU = contextAPU;
    
    switch (type) {
        case APUNUM_INTEGER:
            _pimpl->minValue.setIntegerValue(INT_MIN);
            _pimpl->maxValue.setIntegerValue(INT_MAX);
            break;
        case APUNUM_FLOAT:
            _pimpl->minValue.setFloatValue(INT_MIN);
            _pimpl->maxValue.setFloatValue(INT_MAX);
            break;
        default:
            break;
    }
}

BlockDSPParameter::~BlockDSPParameter()
{
    delete _pimpl;
}

void BlockDSPParameter::setName(const char *name)
{
    strcpy(_pimpl->name, name);
}

const char *BlockDSPParameter::name()
{
    return _pimpl->name;
}

void BlockDSPParameter::setTarget(APUNumber target)
{
    _pimpl->target = target;
}

void BlockDSPParameter::setValue(APUNumber value)
{
    switch (type()) {
        case APUNUM_INTEGER:
        {
            int minVal = getMinValue().integerValue();
            int maxVal = getMaxValue().integerValue();
            int nVal = value.integerValue();
            if (nVal < minVal) {
                value.setIntegerValue(minVal);
            } else if (nVal > maxVal) {
                value.setIntegerValue(maxVal);
            }
                _pimpl->target.setFloatValue(value.floatValue());
            
            break;
        }
        case APUNUM_FLOAT:
        {
            float maxVal = getMaxValue().floatValue();
            float minVal = getMinValue().floatValue();
            float fVal = value.floatValue();
            if (fVal < minVal) {
                value.setFloatValue(minVal);
            } else if (fVal > maxVal) {
                value.setFloatValue(maxVal);
            }
            _pimpl->target.setFloatValue(value.floatValue());

            break;
        }
        case APUNUM_BOOLEAN:
        {
            _pimpl->target.setBoolValue(value.boolValue());
        }
    }

    if (callback) {
        callback(_pimpl->contextAPU, this, value);
    }
    
    if (_pimpl->contextAPU) {
        _pimpl->contextAPU->onParameterChanged(this, value);
    }
}

APUNumberType BlockDSPParameter::type()
{
    return _pimpl->type;
}

void BlockDSPParameter::setMinValue(APUNumber minVal)
{
    switch (type()) {
        case APUNUM_INTEGER:
            _pimpl->minValue.setIntegerValue(minVal.integerValue());
            break;
        case APUNUM_FLOAT:
            _pimpl->minValue.setFloatValue(minVal.floatValue());
            break;
        default:
            break;
    }
}

void BlockDSPParameter::setMaxValue(APUNumber maxVal)
{
    switch (type()) {
        case APUNUM_INTEGER:
            _pimpl->maxValue.setIntegerValue(maxVal.integerValue());
            break;
        case APUNUM_FLOAT:
            _pimpl->maxValue.setFloatValue(maxVal.floatValue());
            break;
        default:
            break;
    }
}

APUNumber BlockDSPParameter::getMaxValue()
{
    //Initialize to create copy
    APUNumber val;
    val = _pimpl->maxValue;
    return val;
}

APUNumber BlockDSPParameter::getMinValue()
{
    APUNumber val;
    val = _pimpl->minValue;
    return val;
}

BlockDSPParameter::pimpl::~pimpl()
{
}

void BlockDSPParameterMap::append(BlockDSPParameter *param)
{
    items_[param->name()] = param;
}

size_t BlockDSPParameterMap::size()
{
    return items_.size();
}

BlockDSPParameterMap::iterator BlockDSPParameterMap::begin()
{
    return items_.begin();
}

BlockDSPParameterMap::iterator BlockDSPParameterMap::end()
{
    return items_.end();
}

BlockDSPParameter * BlockDSPParameterMap::parameterWithName(const char *name)
{
    auto it = items_.find(name);
    if (it != items_.end()) {
        return it->second;
    }
    return NULL;
}

BlockDSPParameter * BlockDSPParameterMap::operator[](const char *name)
{
    return parameterWithName(name);
}

