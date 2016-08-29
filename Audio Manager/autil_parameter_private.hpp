//
//  autil_parameter_private.hpp
//  libblockdsp
//
//  Created by Luke on 8/26/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef autil_parameter_private_h
#define autil_parameter_private_h

#include "autil_parameter.hpp"

class APUParameter::Pimpl
{
public:
    Pimpl(APUNumberType vtype, const char *pname, APUParameterCallback *callbck)
    :isSmoothing(false), smoothingEnabled(false), smoothingInterval_msec(0), smoothingFrames(0)
    ,name(NULL), valueType(vtype)
    ,cb(NULL)
    {
        switch (valueType) {
            case APUNUM_FLOAT:
                target.setFloatValue(0);
                current.setFloatValue(0);
                break;
            case APUNUM_INTEGER:
                target.setIntegerValue(0);
                current.setIntegerValue(0);
                break;
            case APUNUM_BOOLEAN:
                target.setBoolValue(false);
                current.setBoolValue(false);
        }

        setName(pname);
    }

    void setName(const char *pname)
    {
        if (name) {
            delete []name;
        }

        size_t lenName = strlen(pname) + 1;
        name = new char[lenName];
        memset(name, 0, lenName);
        memcpy(name, pname, lenName-1);
    }

    bool isSmoothing;
    bool smoothingEnabled;
    double smoothingInterval_msec;
    size_t smoothingFrames;

    char *name;

    APUNumber target;
    APUNumber current;
    APUNumber minValue;
    APUNumber maxValue;
    float diffPerUpdate;
    APUNumberType valueType;

    APUParameterCallback *cb;
};


#endif /* autil_parameter_private_h */
