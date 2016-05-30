//
//  BDAPULoader.cpp
//  libblockdsp
//
//  Created by Luke on 5/29/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "BDAPULoader.hpp"
#include "BDAPULoader_Private.hpp"
#include "BlockDSPSystem.hpp"
#include <dlfcn.h>

#define kSystemFactoryFuncName "BlockDSPFactoryCreateSystem"

BDAPULoader::BDAPULoader()
{
    _pimpl = new pimpl;
    
    _pimpl->handle = 0;
    _pimpl->error = BDAPULoaderErrorNoError;
}

BDAPULoader::~BDAPULoader()
{
    if (_pimpl->handle)
        dlclose(_pimpl->handle);
    
    delete _pimpl;
}

BlockDSPAPU * BDAPULoader::loadAPU(const char *filepath)
{
    void *dllHandle = dlopen(filepath, RTLD_NOW);
    if (!dllHandle)
    {
        _pimpl->error = BDAPULoaderErrorLibraryLoadFailed;
        return NULL;
    }
    
    dlerror(); //clear dlerror
    
    BlockDSPSystem * (*systemFactoryFn)(void) = (BlockDSPSystem *(*)(void))dlsym(dllHandle, kSystemFactoryFuncName);
    if (dlerror() != NULL)
    {
        _pimpl->error = BDAPULoaderErrorSymbolNotFound;
        return NULL;
    }
    
    _pimpl->handle = dllHandle;
    
    BlockDSPSystem *system = systemFactoryFn();
    BlockDSPAPU *apu = new BlockDSPAPU();
    apu->system = system;
    
    return apu;
}

void BDAPULoader::close()
{
    if (_pimpl->handle)
    {
        dlclose(_pimpl->handle);
        _pimpl->handle = 0;
    }
}