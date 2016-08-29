//
//  autil_parameter.cpp
//  libblockdsp
//
//  Created by Luke on 8/26/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "autil_parameter.hpp"
#include "autil_parameter_private.hpp"
#include <cmath>
#include <climits>

APUParameter::APUParameter(const char *name, APUNumberType valueType, APUParameterCallback *cb)
{
    _pimpl = new Pimpl(valueType, name, cb);
    setMaxValue(APUNUM_INT(INT_MAX));
    setMinValue(APUNUM_INT(INT_MIN));
}

APUParameter::~APUParameter()
{
    delete _pimpl;
}

const char * APUParameter::getName()
{
    return _pimpl->name;
}

void APUParameter::setName(const char *name)
{
    _pimpl->setName(name);
}

APUNumberType APUParameter::type()
{
    return _pimpl->valueType;
}

APUNumber APUParameter::getTarget()
{
    APUNumber num;
    num = _pimpl->target;
    return num;
}

APUNumber APUParameter::getCurrentValue()
{
    APUNumber num;
    num = _pimpl->current;
    return num;
}

void APUParameter::setValue(APUNumber &value)
{
    normalizeValue(value);
    _pimpl->isSmoothing = false;
    _pimpl->current = value;
    _pimpl->target = value;

    if (_pimpl->cb) {
        _pimpl->cb->onParameterChanged(this);
    }
}

void APUParameter::setTarget(APUNumber& target)
{
    normalizeValue(target);
    if (!isSmoothingEnabled()) {
        setValue(target);
    } else {
        _pimpl->isSmoothing = true;
        _pimpl->target = target;
        _pimpl->diffPerUpdate = (_pimpl->target.floatValue() - _pimpl->current.floatValue()) / (float)_pimpl->smoothingFrames;
    }
}

void APUParameter::setMinValue(APUNumber minVal)
{
    _pimpl->minValue = minVal;
}

void APUParameter::setMaxValue(APUNumber maxVal)
{
    _pimpl->maxValue = maxVal;
}

APUNumber APUParameter::getMinValue()
{
    APUNumber num;
    num = _pimpl->minValue;
    return num;
}

APUNumber APUParameter::getMaxValue()
{
    APUNumber num;
    num = _pimpl->maxValue;
    return num;
}

void APUParameter::setSmoothingEnabled(bool enabled)
{
    if (type() == APUNUM_FLOAT)
        _pimpl->smoothingEnabled = enabled;
}

void APUParameter::setSmoothingInterval(double millisec)
{
    _pimpl->smoothingInterval_msec = millisec;
}

double APUParameter::getSmoothingInterval()
{
    return _pimpl->smoothingInterval_msec;
}

bool APUParameter::isSmoothingEnabled()
{
    return _pimpl->smoothingEnabled;
}

void APUParameter::update()
{
    if (_pimpl->isSmoothing) {
        float cv = _pimpl->current.floatValue();
        cv += _pimpl->diffPerUpdate;
        _pimpl->current.setFloatValue(cv);
        
        if (cv == _pimpl->target.floatValue()) {
            _pimpl->isSmoothing = false;
        }

        if (_pimpl->cb) {
            _pimpl->cb->onParameterChanged(this);
        }
    }
}

void APUParameter::setSampleRate(size_t sampleRate)
{
    double dSampleRate = (double)sampleRate;
    double dFramesPerUpdate = dSampleRate * _pimpl->smoothingInterval_msec * 1000;
    _pimpl->smoothingFrames = floor(dFramesPerUpdate);
}

void APUParameter::normalizeValue(APUNumber &value)
{
    switch (type())
    {
        case APUNUM_FLOAT:
        {
            if (value.floatValue() < _pimpl->minValue.floatValue()) {
                value.setFloatValue(_pimpl->minValue.floatValue());
            } else if (value.floatValue() > _pimpl->maxValue.floatValue()) {
                value.setFloatValue(_pimpl->maxValue.floatValue());
            }
            break;
        }
        case APUNUM_INTEGER:
        {
            if (value.integerValue() < _pimpl->minValue.integerValue()) {
                value.setIntegerValue(_pimpl->minValue.integerValue());
            } else if (value.integerValue() > _pimpl->maxValue.integerValue()) {
                value.setIntegerValue(_pimpl->maxValue.integerValue());
            }
            break;
        }
        default:
            break;
    }
}
