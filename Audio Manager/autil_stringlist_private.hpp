//
//  autil_stringlist_private.hpp
//  libblockdsp
//
//  Created by Luke on 9/8/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef autil_stringlist_private_h
#define autil_stringlist_private_h

#include "autil_stringlist.hpp"
#include <vector>
#include <string>

class APUStringList::Pimpl
{
public:
    std::vector<std::string> vStrings;
};


#endif /* autil_stringlist_private_h */
