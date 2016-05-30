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

BDAPULoader::BDAPULoader()
{
    _handle = 0;
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
        printf("%s\n", dlerror());
        return NULL;
    }
    
    dlerror();
    
    BlockDSPSystem * (*systemFactoryFn)(void) = (BlockDSPSystem *(*)(void))dlsym(dllHandle, "BlockDSPFactoryCreateSystem");
    if (dlerror() != NULL)
    {
        printf("%s\n", dlerror());
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