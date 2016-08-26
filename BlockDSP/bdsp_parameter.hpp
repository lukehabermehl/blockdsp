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
#include <unordered_map>
#include <string>

#define BDSP_MAX_PARAMLEN 256

class BlockDSPParameter;
class BlockDSPAPU;

/** typedef for callback function pointer for parameters. The callback is an optional function that will be called when the parameter changes. The parameters are the BlockDSPAPU instance, a pointer to the parameter that changed, an APUNumber representing the new value. */
typedef void (* BlockDSPParameterCallback)(BlockDSPAPU *, BlockDSPParameter *, APUNumber value);

/** A parameter that can be used to change a number and/or trigger a callback */
class BlockDSPParameter {
public:
    /** Constructor
      * @param type the value type of the parameter
      * @param name the name of the parameter (ex. "Output gain")
      * @param target a pointer to the number to change when the parameter is modified. Can be NULL
      * @param system the containing system 
      */
    BlockDSPParameter(APUNumberType type, const char *name, APUNumber target, BlockDSPAPU *contextAPU);
    ~BlockDSPParameter();
    /** Set the name of the parameter */
    void setName(const char *name);
    /** Get the name of the parameter */
    const char *name();
    /** Set the target number
      * @deprecated target paradigm change needed */
    void setTarget(APUNumber target);
    
    /** Set the value of the parameter */
    void setValue(APUNumber value);
    
    /** Set the minimum permissible value */
    void setMinValue(APUNumber minVal);
    /** Set the maximum permissible value */
    void setMaxValue(APUNumber maxVal);

    /** Get the minimum allowed value for the parameter */
    APUNumber getMinValue();
    /** Get the maximum allowed value for the parameter */
    APUNumber getMaxValue();
    
    /** Get the value type of the parameter */
    APUNumberType type();
    /** Holds the callback function pointer or NULL if none exists */
    BlockDSPParameterCallback callback;
    
private:
    class pimpl;
    pimpl *_pimpl;
};

/** Iterable container for parameters */
class BlockDSPParameterMap {
    friend class BlockDSPAPU;
public:
    typedef std::unordered_map<std::string, BlockDSPParameter *>::iterator iterator;
    iterator begin();
    iterator end();
    
    /** Get the number of parameters in the list */
    size_t size();
    /** Lookup a parameter given its name */
    BlockDSPParameter *parameterWithName(const char *name);
    /** Identical to `parameterWithName()` */
    BlockDSPParameter * operator[](const char *name);
    
protected:
    void append(BlockDSPParameter *param);
    
private:
    std::unordered_map<std::string, BlockDSPParameter *> items_;
};

#endif /* BlockDSPParameter_hpp */
