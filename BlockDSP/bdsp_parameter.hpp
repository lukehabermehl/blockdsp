//
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file bdsp_parameter.hpp */

#ifndef BlockDSPParameter_hpp
#define BlockDSPParameter_hpp

#include <aputils/autil_number.hpp>
#include <aputils/autil_parameter.hpp>
#include <unordered_map>
#include <string>

#define BDSP_MAX_PARAMLEN 256

class BlockDSPParameter;
class BlockDSPAPU;

/** typedef for callback function pointer for parameters. The callback is an optional function that will be called when the parameter changes. The parameters are the BlockDSPAPU instance and a pointer to the parameter that changed */
typedef void (* BlockDSPParameterCallback)(BlockDSPAPU *, BlockDSPParameter *);

/** A parameter that can be used to change a number and/or trigger a callback */
class BlockDSPParameter : public APUParameter
{
public:
    /** Constructor
      * @param type the value type of the parameter
      * @param name the name of the parameter (ex. "Output gain")
      * @param system the containing system 
      */
    BlockDSPParameter(APUNumberType type, const char *name, BlockDSPAPU *contextAPU, APUNumber minValue, APUNumber maxValue, APUNumber defaultValue);
    ~BlockDSPParameter();

    /** Holds the callback function pointer or NULL if none exists */
    BlockDSPParameterCallback callback;

    /** Override APUParameter::setValue() */
    virtual void setValue(APUNumber& value);

protected:
    /** Override APUParameter::update() */
    virtual void update();
    
private:
    class pimpl;
    pimpl *_pimpl;

    void triggerCallbacks();
};

#endif /* BlockDSPParameter_hpp */
