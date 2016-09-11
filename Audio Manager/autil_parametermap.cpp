//
//  autil_parametermap.cpp
//  libblockdsp
//
//  Created by Luke on 9/9/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "autil_parametermap.hpp"
#include "autil_parametermap_private.hpp"

APUParameterMap::APUParameterMap()
{
    _pimpl = std::make_shared<Pimpl>();
}

APUParameterMap::APUParameterMap(const APUParameterMap& rhs)
{
    _pimpl = rhs._pimpl;
}

APUParameterMap& APUParameterMap::operator=(const APUParameterMap &rhs)
{
    _pimpl = rhs._pimpl;
    return (*this);
}

void APUParameterMap::addParameter(APUParameter *parameter)
{
    if (parameter == NULL)
        return;

    _pimpl->params[parameter->getName()] = parameter;
}

APUParameter * APUParameterMap::parameterWithName(const char *name) const
{
    auto it = _pimpl->params.find(std::string(name));
    if (it == _pimpl->params.end())
    {
        return NULL;
    }

    return it->second;
}

APUParameterMap::Pimpl::~Pimpl()
{
    for (auto it = params.begin(); it != params.end(); it++)
    {
        delete it->second;
    }
}

APUParameterMap::Iterator APUParameterMap::begin() const
{
    std::shared_ptr<Iterator::Pimpl> itPimpl = std::make_shared<Iterator::Pimpl>();
    itPimpl->mapPtr = &_pimpl->params;
    itPimpl->mapIterator = _pimpl->params.begin();

    Iterator it;
    it._pimpl = itPimpl;

    return it;
}

const char * APUParameterMap::Iterator::first()
{
    if (!valid()) return NULL;
    return _pimpl->mapIterator->first.c_str();
}

APUParameter * APUParameterMap::Iterator::second()
{
    if (!valid()) return NULL;
    return _pimpl->mapIterator->second;
}

size_t APUParameterMap::size() const
{
    return _pimpl->params.size();
}

APUParameterMap::Iterator& APUParameterMap::Iterator::operator++()
{
    _pimpl->mapIterator++;
    return (*this);
}

bool APUParameterMap::Iterator::valid()
{
    return _pimpl->mapIterator != _pimpl->mapPtr->end();
}

