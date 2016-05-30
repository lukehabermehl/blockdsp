//
//  BDAPULoader.cpp
//  libblockdsp
//
//  Created by Luke on 5/29/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "BDAPULoader.hpp"
#include "BlockDSPAPU.hpp"
#include "BlockDSPSystem.hpp"
#include <dlfcn.h>

#define kSystemFactoryFuncName "BlockDSPFactoryCreateSystem"

BDAPULoader::BDAPULoader()
{
    _handle = 0;
    _error = BDAPULoaderErrorNoError;
}

BDAPULoader::~BDAPULoader()
{
    if (_handle)
        dlclose(_handle);
}

BlockDSPAPU * BDAPULoader::loadAPU(const char *filepath)
{
    void *dllHandle = dlopen(filepath, RTLD_NOW);
    if (!dllHandle)
    {
        _error = BDAPULoaderErrorLibraryLoadFailed;
        return NULL;
    }
    
    dlerror(); //clear dlerror
    
    BlockDSPSystem * (*systemFactoryFn)(void) = (BlockDSPSystem *(*)(void))dlsym(dllHandle, kSystemFactoryFuncName);
    if (dlerror() != NULL)
    {
        _error = BDAPULoaderErrorSymbolNotFound;
        return NULL;
    }
    
    _handle = dllHandle;
    
    BlockDSPSystem *system = systemFactoryFn();
    BlockDSPAPU *apu = new BlockDSPAPU();
    apu->system = system;
    
    return apu;
}

void BDAPULoader::close()
{
    if (_handle)
    {
        dlclose(_handle);
        _handle = 0;
    }
}