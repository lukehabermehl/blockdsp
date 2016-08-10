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

BlockDSPParameter::BlockDSPParameter(BlockDSPNumberType type, const char *name, BlockDSPNumber *target, BlockDSPAPU *contextAPU)
{
    _pimpl = new pimpl;
    _pimpl->type = type;
    setTarget(target);
    setName(name);
    callback = 0;
    _pimpl->contextAPU = contextAPU;
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
    if (_pimpl->type != BlockDSPNumberType::FLOAT)
        return false;
    
    if (_pimpl->target)
        _pimpl->target->setFloatValue(val);
    
    BlockDSPNumber *num = BlockDSPNumber::numberForFloat(val);
    
    if (callback) {
        (*callback)(_pimpl->contextAPU, this, num);
    }
    _pimpl->contextAPU->onParameterChanged(this, num);
    delete num;
    
    return true;
    
}

bool BlockDSPParameter::setValue(bool b)
{
    if (_pimpl->type != BlockDSPNumberType::BOOLEAN)
        return false;
        
    if (_pimpl->target)
        _pimpl->target->setBoolValue(b);
    
    BlockDSPNumber *num = BlockDSPNumber::numberForBool(b);
    
    if (callback) {
        (*callback)(_pimpl->contextAPU, this, num);
    }
    
    _pimpl->contextAPU->onParameterChanged(this, num);
    delete num;
    
    return true;
}

bool BlockDSPParameter::setValue(int val)
{
    if (_pimpl->type != BlockDSPNumberType::INTEGER)
        return false;
    
    if (_pimpl->target)
        _pimpl->target->setIntegerValue(val);
    
    BlockDSPNumber *num = BlockDSPNumber::numberForInteger(val);
    
    if (callback) {
        (*callback)(_pimpl->contextAPU, this, num);
    }
    _pimpl->contextAPU->onParameterChanged(this, num);
    delete num;
    
    return true;
}

BlockDSPParameter::pimpl::~pimpl()
{
    
}

