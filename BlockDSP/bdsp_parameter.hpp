//
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file bdsp_parameter.hpp */

#ifndef BlockDSPParameter_hpp
#define BlockDSPParameter_hpp

class BlockDSPNumber;

#define BDSP_MAX_PARAMLEN 256

class BlockDSPSystem;
class BlockDSPParameter;

/** Value types for parameters and numbers */
enum BlockDSPParameterType
{
    BlockDSPParameterTypeInteger,
    BlockDSPParameterTypeUnsignedInt,
    BlockDSPParameterTypeFloat,
    BlockDSPParameterTypeBoolean
};

/** typedef for callback function pointer for parameters. The callback is an optional function that will be called when the parameter changes. The parameters are the BlockDSPSystem instance, a pointer to the parameter that changed, and a pointer to the new value of the parameter. Use the *type()* method of the parameter to get the type to which to cast the value pointer */
typedef void (* BlockDSPParameterCallback)(const BlockDSPSystem *, const BlockDSPParameter *, void *value);

/** A parameter that can be used to change a number and/or trigger a callback */
class BlockDSPParameter {
public:
    /** Constructor
      * @param type the value type of the parameter
      * @param name the name of the parameter (ex. "Output gain")
      * @param target a pointer to the number to change when the parameter is modified. Can be NULL
      * @param system the containing system 
      */
    BlockDSPParameter(BlockDSPParameterType type, const char *name, BlockDSPNumber *target, BlockDSPSystem *system);
    ~BlockDSPParameter();
    /** Set the name of the parameter */
    void setName(const char *name);
    /** Get the name of the parameter */
    const char *name();
    /** Set the target number
      * @param target a pointer to a BlockDSPNumber or NULL */
    void setTarget(BlockDSPNumber *target);
    /** Set the float value of the parameter */
    bool setValue(float val);
    /** Set the bool value of the parameter */
    bool setValue(bool val);
    /** Set the integer value of the parameter */
    bool setValue(int val);
    
    /** Get the value type of the parameter */
    BlockDSPParameterType type();
    /** Holds the callback function pointer or NULL if none exists */
    BlockDSPParameterCallback callback;
    
private:
    class pimpl;
    pimpl *_pimpl;
};

#endif /* BlockDSPParameter_hpp */
