//
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file bdsp_parameter.hpp */

#ifndef BlockDSPParameter_hpp
#define BlockDSPParameter_hpp

#include "bdsp_number.hpp"
#include <vector>

class BlockDSPAPU;

#define BDSP_MAX_PARAMLEN 256

class BlockDSPParameter;

/** typedef for callback function pointer for parameters. The callback is an optional function that will be called when the parameter changes. The parameters are the BlockDSPAPU instance, a pointer to the parameter that changed, and a pointer to a BlockDSPNumber instance representing the new value. *NOTE* the BlockDSPNumber instance is only guaranteed to be valid for the lifespan of the callback. *DO NOT* keep a reference to this value */
typedef void (* BlockDSPParameterCallback)(BlockDSPAPU *, BlockDSPParameter *, BlockDSPNumber *value);

/** A parameter that can be used to change a number and/or trigger a callback */
class BlockDSPParameter {
public:
    /** Constructor
      * @param type the value type of the parameter
      * @param name the name of the parameter (ex. "Output gain")
      * @param target a pointer to the number to change when the parameter is modified. Can be NULL
      * @param system the containing system 
      */
    BlockDSPParameter(BlockDSPNumberType type, const char *name, BlockDSPNumber *target, BlockDSPAPU *contextAPU);
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
    BlockDSPNumberType type();
    /** Holds the callback function pointer or NULL if none exists */
    BlockDSPParameterCallback callback;
    
private:
    class pimpl;
    pimpl *_pimpl;
};

class BlockDSPParameterList {
    friend class BlockDSPAPU;
public:
    size_t size();
    BlockDSPParameter *getAt(size_t index);
    
    BlockDSPParameter * operator[](size_t index);
    
protected:
    void append(BlockDSPParameter *param);
    
private:
    std::vector<BlockDSPParameter *> items_;
};

#endif /* BlockDSPParameter_hpp */
