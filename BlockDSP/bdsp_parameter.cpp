//
//  BlockDSPParameter.cpp
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "bdsp_parameter.hpp"
#include "bdsp_parameter_private.hpp"
#include "bdsp_apu.hpp"
#include <string.h>
#include <limits.h>

BlockDSPParameter::BlockDSPParameter(APUNumberType type, const char *name, BlockDSPAPU *contextAPU, APUNumber minValue, APUNumber maxValue, APUNumber defaultValue)
: APUParameter(name, type, minValue, maxValue, defaultValue, NULL)
, callback(NULL)
{
    _pimpl = new pimpl;
    _pimpl->contextAPU = contextAPU;
}

BlockDSPParameter::~BlockDSPParameter()
{
    delete _pimpl;
}

BlockDSPParameter::pimpl::~pimpl()
{
}

void BlockDSPParameter::setValue(APUNumber &value)
{
    APUParameter::setValue(value);
    triggerCallbacks();
}

void BlockDSPParameter::update()
{
    float old = getCurrentValue().floatValue();
    APUParameter::update();
    if (old != getCurrentValue().floatValue()) {
        triggerCallbacks();
    }
}

void BlockDSPParameter::triggerCallbacks()
{
    _pimpl->contextAPU->onParameterChanged(this);

    if (callback) {
        callback(_pimpl->contextAPU, this);
    }
}
