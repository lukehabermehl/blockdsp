//
//  BDAPULoader_Private.hpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef BDAPULoader_Private_h
#define BDAPULoader_Private_h

#include "BDAPULoader.hpp"

class BDAPULoader::pimpl
{
public:
    void *handle;
    BDAPULoaderError error;
};


#endif /* BDAPULoader_Private_h */
