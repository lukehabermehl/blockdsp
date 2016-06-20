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
#include <string.h>

BlockDSPParameter::BlockDSPParameter(BlockDSPParameterType type, const char *name, BlockDSPNumber *target, BlockDSPSystem *system)
{
    _pimpl = new pimpl;
    _pimpl->type = type;
    setTarget(target);
    setName(name);
    callback = 0;
    _pimpl->system = system;
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

void BlockDSPParameter::setTarget(BlockDSPNumber *target)
{
    _pimpl->target = target;
}

bool BlockDSPParameter::setValue(float val)
{
    if (_pimpl->type != BlockDSPParameterTypeFloat)
        return false;
    
    if (_pimpl->target)
        _pimpl->target->setFloatValue(val);
    
    if (callback)
        (*callback)(_pimpl->system, this, &val);
    
    return true;
    
}

bool BlockDSPParameter::setValue(bool b)
{
    if (_pimpl->type != BlockDSPParameterTypeBoolean)
        return false;
        
    if (_pimpl->target)
        _pimpl->target->setBoolValue(b);
    
    if (callback)
        (*callback)(_pimpl->system, this, &b);
    
    return true;
}

bool BlockDSPParameter::setValue(int val)
{
    if (_pimpl->type != BlockDSPParameterTypeInteger)
        return false;
    
    if (_pimpl->target)
        _pimpl->target->setIntegerValue(val);
    
    if (callback)
        (*callback)(_pimpl->system, this, &val);
    
    return true;
}

BlockDSPParameter::pimpl::~pimpl()
{
    
}

