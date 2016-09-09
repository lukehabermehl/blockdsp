//
//  autil_parameter.hpp
//  libblockdsp
//
//  Created by Luke on 8/26/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef autil_parameter_hpp
#define autil_parameter_hpp

#include "autil_number.hpp"
#include "autil_uiattrs.h"
#include "autil_stringlist.hpp"

class APUParameter;
class AudioProcessingUnit;

/** Interface for a callback object that handles parameter changes */
class APUParameterCallback
{
public:
    /** This method will be called when the parameters current value is changed.
      * @param parameter a pointer to the parameter that was changed 
      */
    virtual void onParameterChanged(APUParameter *parameter) = 0;
};

class APUParameter
{
    friend class AudioProcessingUnit;

public:
    APUParameter(const char *name, APUNumberType valueType, APUParameterCallback *cb=NULL);
    virtual ~APUParameter();
    void setName(const char *name);
    const char *getName();

    /** Get the target value of the parameter (value after smoothing) (copy) */
    APUNumber getTarget();
    /** Get the current value of the parameter (copy) */
    APUNumber getCurrentValue();
    /** Get the value type of the parameter */
    APUNumberType type();

    /** Set the callback object for the parameter */
    void setCallback(APUParameterCallback *cb);

    /** Set the minimum permissible value */
    virtual void setMinValue(APUNumber minVal);
    /** Set the maximum permissible value */
    virtual void setMaxValue(APUNumber maxVal);

    /** Get the minimum allowed value for the parameter */
    APUNumber getMinValue();
    /** Get the maximum allowed value for the parameter */
    APUNumber getMaxValue();

    /** Set the value (no smoothing) of the parameter. If the value is outside of the min/max range, it will be clamped.
      * @return false if the value was clamped. Otherwise, true
      */
    virtual bool setValue(APUNumber value);
    /** Set the value of the parameter to be smoothed if smoothing is enabled.
      * @return false if the value was clamped. Otherwise, true
      */
    virtual bool setTarget(APUNumber target);
    /** Set whether or not parameters will ramp to the target value over time. Smoothing is only allowed for FLOAT types. */
    virtual void setSmoothingEnabled(bool enabled);
    /** Set the interval (in millseconds) over which the parameter value will ramp if smoothing is enabled */
    virtual void setSmoothingInterval(double millisec);
    /** Get the number of milliseconds over which the parameter value will ramp if smoothing is enabled */
    virtual double getSmoothingInterval();
    /** Determine if smoothing is enabled for the parameter */
    bool isSmoothingEnabled();

    /** Get the description of the UI control for the parameter */
    virtual APUUIAttribute getUIAttributes();

protected:
    /** Called by the APU to update the current parameter value for smoothing */
    virtual void update();
    /** Set the sample rate internally to calculate smoothing time */
    virtual void setSampleRate(size_t sampleRate);
    /** Clamp the value to the min/max if needed */
    bool normalizeValue(APUNumber &value);
    /** Set the UI attribute flags */
    virtual void setUIAttributes(APUUIAttribute attr);

private:
    class Pimpl;
    Pimpl *_pimpl;
};

/** Parameter representing a set of discrete values */
class APUEnumParameter : public APUParameter
{
public:
    /** Constructor
      * @param name name of the parameter
      * @param numItems number of values. Must be non-zero
      * @param cb optional callback object
      */
    APUEnumParameter(const char *name, APUStringList strings, APUParameterCallback *cb=NULL);
    virtual ~APUEnumParameter();

    /** Cannot set min value for enums. Minimum is always 0 */
    void setMinValue(APUNumber minVal);
    /** Cannot set max value for enums. Maximum is always numItems - 1 */
    void setMaxValue(APUNumber maxVal);

    /** Get the string representation of the given value */
    const char * stringForValue(uint32_t value);

private:
    class EnumParamPimpl;
    EnumParamPimpl *_enumParamPimpl;
};

#endif /* autil_parameter_hpp */
