//
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file bdsp_number.hpp */

#ifndef BlockDSPNumber_hpp
#define BlockDSPNumber_hpp

#include <memory>

#define BDSP_FLOAT(_f) BlockDSPNumber::numberForFloat(_f)
#define BDSP_INT(_i) BlockDSPNumber::numberForInteger(_i)
#define BDSP_BOOL(_b) BlockDSPNumber::numberForBool(_b)

/** Underlying types for numbers */
enum BlockDSPNumberType {
    INTEGER,
    FLOAT,
    BOOLEAN
};

class BlockDSPNumber;

/** typedef reference counted pointer to BlockDSPNumber. You should pretty much always use this instead of raw pointers */
typedef std::shared_ptr<BlockDSPNumber> BlockDSPNumberRef;

/** An object that represents common numeric types */
class BlockDSPNumber
{
public:
    BlockDSPNumber();
    ~BlockDSPNumber();
    
    /** Create a new number for a float value */
    static BlockDSPNumberRef numberForFloat(float f);
    /** Create a new number for an integer value */
    static BlockDSPNumberRef numberForInteger(int i);
    /** Create a new number for a bool value */
    static BlockDSPNumberRef numberForBool(bool b);
    
    /** Create a new number ref with the same value */
    BlockDSPNumberRef copy();
    
    /** Get the float value of the number */
    float floatValue();
    /** Get the integer value of the number. If the true value is a float, the value will be floored. */
    int integerValue();
    /** Get the boolean value. Non-zero values return true */
    bool boolValue();
    
    /** Set the value to the given float */
    void setFloatValue(float f);
    /** Set the value to the given integer */
    void setIntegerValue(int i);
    /** Set the value to the given bool */
    void setBoolValue(bool b);
    
private:
    class pimpl;
    pimpl * _pimpl;
};

#endif /* BlockDSPNumber_hpp */
