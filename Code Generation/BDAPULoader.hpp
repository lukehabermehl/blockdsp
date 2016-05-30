//
//  BDAPULoader.hpp
//  libblockdsp
//
//  Created by Luke on 5/29/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef BDAPULoader_hpp
#define BDAPULoader_hpp

class BlockDSPAPU;

class BDAPULoader
{
public:
    BDAPULoader();
    ~BDAPULoader();
    
    BlockDSPAPU *loadAPU(const char *filepath);
    void close();
    
    
private:
    void *_handle;
};

#endif /* BDAPULoader_hpp */
