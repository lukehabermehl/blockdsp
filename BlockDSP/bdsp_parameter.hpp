//
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file bdsp_parameter.hpp */

#ifndef BlockDSPParameter_hpp
#define BlockDSPParameter_hpp

#include "autil_number.hpp"
#include "autil_parameter.hpp"
#include <unordered_map>
#include <string>

#define BDSP_MAX_PARAMLEN 256

class BlockDSPParameter;
class BlockDSPAPU;

/** typedef for callback function pointer for parameters. The callback is an optional function that will be called when the parameter changes. The parameters are the BlockDSPAPU instance, a pointer to the parameter that changed, an APUNumber representing the new value. */
typedef void (* BlockDSPParameterCallback)(BlockDSPAPU *, BlockDSPParameter *, const APUNumber& value);

/** A parameter that can be used to change a number and/or trigger a callback */
class BlockDSPParameter : public APUParameter {
public:
    /** Constructor
      * @param type the value type of the parameter
      * @param name the name of the parameter (ex. "Output gain")
      * @param target a pointer to the number to change when the parameter is modified. Can be NULL
      * @param system the containing system 
      */
    BlockDSPParameter(APUNumberType type, const char *name, APUNumber target, BlockDSPAPU *contextAPU);
    ~BlockDSPParameter();

    /** Holds the callback function pointer or NULL if none exists */
    BlockDSPParameterCallback callback;
    
private:
    class pimpl;
    pimpl *_pimpl;
};

#endif /* BlockDSPParameter_hpp */
