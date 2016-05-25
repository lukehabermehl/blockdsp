//
// Author: Luke Habermehl
//
#ifndef _DELAY_BUFFER
#define _DELAY_BUFFER

#include <cstdlib>

class DelayBuffer
{
public:
    DelayBuffer();
    ~DelayBuffer();
    
    void initWithSize(size_t nDelaySize);
    void shuffleIn(float fSample);
    void reset();
    float sampleAtDelayIndex(size_t nIndex);
    float operator[](size_t);
    size_t size();
    
protected:
    float * m_pSamples;
    size_t m_nStartIndex;
    size_t m_nSize;
};

#endif