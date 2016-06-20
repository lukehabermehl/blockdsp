//
//  BDAPULoader.hpp
//  libblockdsp
//
//  Created by Luke on 5/29/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef BDAPULoader_hpp
#define BDAPULoader_hpp

#include "bdsp_apu.hpp"

enum BDAPULoaderError
{
    BDAPULoaderErrorNoError = 0,
    BDAPULoaderErrorLibraryLoadFailed,
    BDAPULoaderErrorSymbolNotFound
};

class BDAPULoader
{
public:
    BDAPULoader();
    ~BDAPULoader();
    
    AudioProcessingUnit *loadAPU(const char *filepath);
    void close();
    
    
private:
    class pimpl;
    pimpl *_pimpl;
};

#endif /* BDAPULoader_hpp */
