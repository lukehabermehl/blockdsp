//
//  autil_number.hpp
//  libblockdsp
//
//  Created by Luke on 8/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef autil_number_hpp
#define autil_number_hpp

#include <memory>

#define APUNUM_FLOAT(_f) APUNumber::numberForFloat(_f)
#define APUNUM_INT(_i) APUNumber::numberForInteger(_i)
#define APUNUM_BOOL(_b) APUNumber::numberForBool(_b)

/** Underlying types for numbers */
enum APUNumberType {
    APUNUM_INTEGER,
    APUNUM_FLOAT,
    APUNUM_BOOLEAN
};

/** An object that represents common numeric types
  * Note: this class has built-in reference counting. No need for pointers! 
  */
class APUNumber
{
public:
    APUNumber();
    /** The copy constructor will increment the reference count */
    APUNumber(APUNumber const &num);
    /** The assignment operator will create a unique copy */
    APUNumber& operator=(APUNumber const& num);
    ~APUNumber();

    /** Create a new number for a float value */
    static APUNumber numberForFloat(float f);
    /** Create a new number for an integer value */
    static APUNumber numberForInteger(int i);
    /** Create a new number for a bool value */
    static APUNumber numberForBool(bool b);

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
    std::shared_ptr<pimpl> _pimpl;
};


#endif /* autil_number_hpp */
