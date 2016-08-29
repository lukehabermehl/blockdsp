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

    /** Set the minimum permissible value */
    void setMinValue(APUNumber minVal);
    /** Set the maximum permissible value */
    void setMaxValue(APUNumber maxVal);

    /** Get the minimum allowed value for the parameter */
    APUNumber getMinValue();
    /** Get the maximum allowed value for the parameter */
    APUNumber getMaxValue();

    /** Set the value (no smoothing) of the parameter. If the value is outside of the min/max range, it will be clamped. You can check if the value was clamped by checking the value of the parameter after calling the method */
    virtual void setValue(APUNumber& value);
    /** Set the value of the parameter to be smoothed if smoothing is enabled. See `setValue()` for value clamping. */
    virtual void setTarget(APUNumber& target);
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

private:
    /** Called by the APU to update the current parameter value for smoothing */
    void update();
    /** Set the sample rate internally to calculate smoothing time */
    void setSampleRate(size_t sampleRate);
    /** Clamp the value to the min/max if needed */
    void normalizeValue(APUNumber &value);
    /** Set the UI attribute flags */
    void setUIAttributes(APUUIAttribute attr);

    class Pimpl;
    Pimpl *_pimpl;
};

#endif /* autil_parameter_hpp */