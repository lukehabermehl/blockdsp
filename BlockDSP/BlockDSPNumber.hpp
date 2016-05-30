//
//  BlockDSPNumber.hpp
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef BlockDSPNumber_hpp
#define BlockDSPNumber_hpp

class BlockDSPNumber
{
public:
    BlockDSPNumber();
    
    static BlockDSPNumber * numberForFloat(float f);
    static BlockDSPNumber * numberForInteger(int i);
    static BlockDSPNumber * numberForBool(bool b);
    
    float floatValue();
    int integerValue();
    bool boolValue();
    
    void setFloatValue(float f);
    void setIntegerValue(int i);
    void setBoolValue(bool b);
    
private:
    unsigned char _data[8];
};

#endif /* BlockDSPNumber_hpp */
