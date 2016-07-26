//
//  libblockdsp
//
//  Created by Luke on 5/29/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file bdsp_apuloader.hpp */

#ifndef BDAPULoader_hpp
#define BDAPULoader_hpp

#include "bdsp_apu.hpp"

/** Error codes for loading the APU dylib */
enum BDAPULoaderError
{
    BDAPULoaderErrorNoError = 0,
    BDAPULoaderErrorLibraryLoadFailed,
    BDAPULoaderErrorSymbolNotFound
};

/** Used to load APU dynamically linked libraries */
class BDAPULoader
{
public:
    BDAPULoader();
    ~BDAPULoader();
    
    /** Load an APU instance from the dylib at the given path */
    AudioProcessingUnit *loadAPU(const char *filepath);
    /** Close the dylib */
    void close();
    
    
private:
    class pimpl;
    pimpl *_pimpl;
};

#endif /* BDAPULoader_hpp */
