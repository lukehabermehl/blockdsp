//
//  BDAPULoader.cpp
//  libblockdsp
//
//  Created by Luke on 5/29/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "bdsp_apuloader.hpp"
#include "bdsp_apuloader_private.hpp"
#include "bdsp_system.hpp"
#include <dlfcn.h>

#define kAPUFactoryFuncName "AudioProcessingUnitFactoryCreate"

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

AudioProcessingUnit * BDAPULoader::loadAPU(const char *filepath)
{
    void *dllHandle = dlopen(filepath, RTLD_NOW);
    if (!dllHandle)
    {
        _pimpl->error = BDAPULoaderErrorLibraryLoadFailed;
        return NULL;
    }
    
    dlerror(); //clear dlerror
    
    AudioProcessingUnit * (*apuFactoryFn)(void) = (AudioProcessingUnit *(*)(void))dlsym(dllHandle, kAPUFactoryFuncName);
    if (dlerror() != NULL)
    {
        _pimpl->error = BDAPULoaderErrorSymbolNotFound;
        return NULL;
    }
    
    _pimpl->handle = dllHandle;
    
    AudioProcessingUnit *apu = apuFactoryFn();
    
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

BDAPULoaderError BDAPULoader::error()
{
    return _pimpl->error;
}
