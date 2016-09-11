//
//  autil_parametermap.hpp
//  libblockdsp
//

/** @file autil_parametermap.hpp */

#ifndef autil_parametermap_hpp
#define autil_parametermap_hpp

#include "autil_parameter.hpp"
#include <memory>

/** @file autil_parametermap.hpp */

/** HashMap implementation that maps parameters by name. Instances are always passed by reference using internal reference counting */
class APUParameterMap
{
public:
    /** A simple iterator object that behaves similarly to std::iterator, but is not a pointer */
    class Iterator
    {
        friend class APUParameterMap;
        
    public:
        /** The name of the parameter */
        const char *first();
        /** The parameter instance */
        APUParameter *second();
        /** Move the iterator forward to the next key/value pair */
        Iterator& operator++();
        /** @return true if iterator points to a valid key/value pair. Otherwise, false. */
        bool valid();

    private:
        class Pimpl;
        std::shared_ptr<Pimpl> _pimpl;
    };

    APUParameterMap();
    APUParameterMap(const APUParameterMap& rhs);
    APUParameterMap& operator=(const APUParameterMap& rhs);

    void addParameter(APUParameter *parameter);
    APUParameter *parameterWithName(const char *name) const;
    size_t size() const;

    Iterator begin() const;

private:
    class Pimpl;
    std::shared_ptr<Pimpl> _pimpl;
};

#endif /* autil_parametermap_hpp */
