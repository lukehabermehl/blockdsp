//
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file bdsp_number.hpp */

#ifndef BlockDSPNumber_hpp
#define BlockDSPNumber_hpp

/** Underlying types for numbers */
enum BlockDSPNumberType {
    INTEGER,
    FLOAT,
    BOOLEAN
};

/** An object that represents common numeric types */
class BlockDSPNumber
{
public:
    BlockDSPNumber();
    ~BlockDSPNumber();
    
    /** Create a new number for a float value */
    static BlockDSPNumber * numberForFloat(float f);
    /** Create a new number for an integer value */
    static BlockDSPNumber * numberForInteger(int i);
    /** Create a new number for a bool value */
    static BlockDSPNumber * numberForBool(bool b);
    
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
    pimpl *_pimpl;
};

#endif /* BlockDSPNumber_hpp */
