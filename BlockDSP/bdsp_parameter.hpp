//
//  BlockDSPParameter.hpp
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef BlockDSPParameter_hpp
#define BlockDSPParameter_hpp

class BlockDSPNumber;

#define BDSP_MAX_PARAMLEN 256

class BlockDSPSystem;
class BlockDSPParameter;

enum BlockDSPParameterType
{
    BlockDSPParameterTypeInteger,
    BlockDSPParameterTypeUnsignedInt,
    BlockDSPParameterTypeFloat,
    BlockDSPParameterTypeBoolean
};

typedef void (* BlockDSPParameterCallback)(const BlockDSPSystem *, const BlockDSPParameter *, void *value);

class BlockDSPParameter {
public:
    BlockDSPParameter(BlockDSPParameterType type, const char *name, BlockDSPNumber *target, BlockDSPSystem *);
    ~BlockDSPParameter();
    
    void setName(const char *name);
    const char *name();
    
    void setTarget(BlockDSPNumber *target);
    bool setValue(float val);
    bool setValue(bool val);
    bool setValue(int val);
    
    BlockDSPParameterType type();
    
    BlockDSPParameterCallback callback;
    
private:
    class pimpl;
    pimpl *_pimpl;
};

#endif /* BlockDSPParameter_hpp */
