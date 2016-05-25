//
//  BlockDSPParameter.cpp
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "BlockDSPParameter.hpp"
#include <string.h>

BlockDSPParameter::BlockDSPParameter(BlockDSPParameterType type, const char *name, BlockDSPNumber *target, BlockDSPSystem *system)
{
    _type = type;
    setTarget(target);
    setName(name);
    callback = 0;
    _system = system;
}

void BlockDSPParameter::setName(const char *name)
{
    strcpy(_name, name);
}

const char *BlockDSPParameter::name()
{
    return _name;
}

void BlockDSPParameter::setTarget(BlockDSPNumber *target)
{
    _target = target;
}

bool BlockDSPParameter::setValue(float val)
{
    if (_type != BlockDSPParameterTypeFloat)
        return false;
    
    if (_target)
        _target->setFloatValue(val);
    
    if (callback)
        (*callback)(_system, this, &val);
    
    return true;
    
}

bool BlockDSPParameter::setValue(bool b)
{
    if (_type != BlockDSPParameterTypeBoolean)
        return false;
        
    if (_target)
        _target->setBoolValue(b);
    
    if (callback)
        (*callback)(_system, this, &b);
    
    return true;
}

bool BlockDSPParameter::setValue(int val)
{
    if (_type != BlockDSPParameterTypeInteger)
        return false;
    
    if (_target)
        _target->setIntegerValue(val);
    
    if (callback)
        (*callback)(_system, this, &val);
    
    return true;
}

