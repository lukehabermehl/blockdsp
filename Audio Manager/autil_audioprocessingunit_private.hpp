//
//  autil_audioprocessingunit_private.hpp
//  libblockdsp
//
//  Created by Luke on 8/28/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef autil_audioprocessingunit_private_h
#define autil_audioprocessingunit_private_h

#include "autil_audioprocessingunit.hpp"
#include "autil_parameter.hpp"
#include <unordered_map>
#include <string>

class AudioProcessingUnit::Pimpl
{
public:
    Pimpl() :sampleRate(0) {}
    size_t sampleRate;

    APUParameterMap parameterMap;
};


#endif /* autil_audioprocessingunit_private_h */
