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
: APUParameter(name, type, NULL)
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
