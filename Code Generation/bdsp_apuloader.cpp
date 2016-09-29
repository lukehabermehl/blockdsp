//
//  bdsp_apuloader.cpp
//  libblockdsp
//

#include "bdsp_apuloader.hpp"
#include "bdsp_apuloader_private.hpp"
#include "bdsp_system.hpp"
#include "bdsp_logger.hpp"
#include <dlfcn.h>

#define kAPUFactoryFuncName "AudioProcessingUnitFactoryCreate"
static const char *kApuLoaderPrefix = "[BDAPULoader]";

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
    BDLogFormat(kApuLoaderPrefix, "Load APU at file path: %s", filepath);
    void *dllHandle = dlopen(filepath, RTLD_NOW);
    if (!dllHandle)
    {
        _pimpl->error = BDAPULoaderErrorLibraryLoadFailed;
        BDLog(kApuLoaderPrefix, "dlopen() call failed");

        return NULL;
    }

    BDLog(kApuLoaderPrefix, "dylib loaded!");
    
    dlerror(); //clear dlerror
    
    AudioProcessingUnit * (*apuFactoryFn)(void) = (AudioProcessingUnit *(*)(void))dlsym(dllHandle, kAPUFactoryFuncName);
    if (dlerror() != NULL)
    {
        _pimpl->error = BDAPULoaderErrorSymbolNotFound;
        BDLog(kApuLoaderPrefix, "Failed to find APU factory function in dylib");
        return NULL;
    }

    BDLog(kApuLoaderPrefix, "Found APU factory function");
    
    _pimpl->handle = dllHandle;
    
    AudioProcessingUnit *apu = apuFactoryFn();
    if (apu)
    {
        BDLogFormat(kApuLoaderPrefix, "APU: %s loaded successfully!", apu->getName());
    }
    else
    {
        BDLog(kApuLoaderPrefix, "APU factory function returned NULL");
    }
    
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
