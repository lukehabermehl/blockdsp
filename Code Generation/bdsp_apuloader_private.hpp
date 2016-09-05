//
//  bdsp_apuloader_private.hpp
//  libblockdsp
//

#ifndef BDAPULoader_Private_h
#define BDAPULoader_Private_h

#include "bdsp_apuloader.hpp"

class BDAPULoader::pimpl
{
public:
    void *handle;
    BDAPULoaderError error;
};


#endif /* BDAPULoader_Private_h */
