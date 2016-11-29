//
//  autil_number.hpp
//  libblockdsp
//

/** @file autil_number.hpp */

#ifndef autil_number_hpp
#define autil_number_hpp

#include <memory>

#define APUNUM_FLOAT(_f) APUNumber::numberForFloat(_f)
#define APUNUM_INT(_i) APUNumber::numberForInteger(_i)
#define APUNUM_BOOL(_b) APUNumber::numberForBool(_b)
#define APUNUM_UINT(_u) APUNumber::numberForUnsignedInt(_u)

/** Underlying types for numbers */
enum APUNumberType {
    APUNUM_INTEGER,
    APUNUM_UINT,
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
    //Default type is float
    APUNumber(float f);
    
    /** Get a copy of the number */
    APUNumber copy();

    /** Get the primitive type of the number */
    APUNumberType getType() const;

    /** Create a new number for a float value */
    static APUNumber numberForFloat(float f);
    /** Create a new number for an integer value */
    static APUNumber numberForInteger(int32_t i);
    /** Create a new number for an unsigned integer value */
    static APUNumber numberForUnsignedInt(uint32_t u);
    /** Create a new number for a bool value */
    static APUNumber numberForBool(bool b);

    /** Get the float value of the number */
    float floatValue() const;
    /** Get the integer value of the number. If the true value is a float, the value will be floored. */
    int32_t integerValue() const;
    /** Get the unsigned integer value of the number */
    uint32_t unsignedIntValue() const;
    /** Get the boolean value. Non-zero values return true */
    bool boolValue() const;

    /** Set the value to the given float */
    void setFloatValue(float f);
    /** Set the value to the given integer */
    void setIntegerValue(int32_t i);
    /** Set the value to the given unsigned integer */
    void setUnsignedIntValue(uint32_t u);
    /** Set the value to the given bool */
    void setBoolValue(bool b);

private:
    union APUNumberContainer_
    {
        int32_t intValue;
        uint32_t unsignedIntValue;
        bool boolValue;
        float floatValue;
    };

    APUNumberContainer_ data_;
    APUNumberType numberType_;
};


#endif /* autil_number_hpp */
