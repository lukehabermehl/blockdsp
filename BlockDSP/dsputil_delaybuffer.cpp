
//
//Author: Luke Habermehl
//
#include "dsputil_delaybuffer.hpp"
#include <stdexcept>

DelayBuffer::DelayBuffer()
{
    m_nSize = 0;
    m_pSamples = NULL;
}

DelayBuffer::~DelayBuffer()
{
    delete [] m_pSamples;
}

void DelayBuffer::initWithSize(size_t nDelaySize)
{
    if (m_pSamples != NULL)
    {
        delete [] m_pSamples;
    }
    
    m_nSize = nDelaySize;
    m_pSamples = new float[m_nSize];
    reset();
}

void DelayBuffer::shuffleIn(float fSample)
{
    m_pSamples[m_nStartIndex] = fSample;
    m_nStartIndex += 1;
    
    if (m_nStartIndex >= m_nSize)
    {
        m_nStartIndex = 0;
    }
}

void DelayBuffer::reset()
{
    for (size_t i=0; i<m_nSize; i++)
    {
        m_pSamples[i] = 0.0;
    }
    
    m_nStartIndex = 0;
}

float DelayBuffer::sampleAtDelayIndex(size_t nIndex)
{
    if (nIndex == 0 || nIndex > m_nSize)
    {
        throw std::out_of_range("");
    }
    
    long int nRealIndex = m_nStartIndex - nIndex;
    if (nRealIndex < 0)
    {
        nRealIndex = m_nSize - (nIndex - m_nStartIndex);
    }
    
    return m_pSamples[nRealIndex];
}

float DelayBuffer::operator[](size_t delay)
{
    return sampleAtDelayIndex(delay);
}

size_t DelayBuffer::size()
{
    return m_nSize;
}