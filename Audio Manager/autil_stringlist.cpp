//
//  autil_stringlist.cpp
//  libblockdsp
//
//  Created by Luke on 9/8/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "autil_stringlist.hpp"
#include "autil_stringlist_private.hpp"

APUStringList::APUStringList()
{
    _pimpl = std::make_shared<Pimpl>();
}

APUStringList::APUStringList(APUStringList& rhs)
{
    _pimpl = rhs._pimpl;
}

APUStringList& APUStringList::operator=(APUStringList const &rhs)
{
    _pimpl = rhs._pimpl;
    return (*this);
}

APUStringList APUStringList::copy()
{
    APUStringList cp;
    for (auto it = _pimpl->vStrings.begin(); it != _pimpl->vStrings.end(); it++)
    {
        cp._pimpl->vStrings.push_back(*it);
    }

    return cp;
}

size_t APUStringList::size()
{
    return _pimpl->vStrings.size();
}

const char * APUStringList::getString(size_t index)
{
    if (index < size())
    {
        return _pimpl->vStrings[index].c_str();
    }

    return NULL;
}

void APUStringList::setString(size_t index, const char *str)
{
    if (index < size())
    {
        _pimpl->vStrings[index] = std::string(str);
    }
}

void APUStringList::append(const char *str)
{
    _pimpl->vStrings.push_back(std::string(str));
}