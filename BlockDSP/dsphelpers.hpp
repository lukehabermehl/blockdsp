//
//  dsphelpers.hpp
//  Filter
//
//  Created by Luke on 4/6/16.
//
//

#ifndef dsphelpers_hpp
#define dsphelpers_hpp

#include <cmath>

struct BiQuadCoefficients {
    float a0;
    float a1;
    float a2;
    float b1;
    float b2;
    
    void calculateForLPF(float cutoff, float q, float sampleRate) {
        float thetaC = (2*M_PI*cutoff) / sampleRate;
        float sinThetaC = sinf(thetaC);
        float d = 1.0 / q;
        float beta = 0.5 * ((1.0 - (d / 2.0) * sinThetaC) / (1.0 + (d / 2.0) * sinThetaC));
        float gamma = (0.5 + beta) * cosf(thetaC);
        
        a0 = (0.5 + beta - gamma) / 2.0;
        a1 = 0.5 + beta - gamma;
        a2 = a1 / 2.0;
        b1 = -2.0 * gamma;
        b2 = 2.0 * beta;
    }
};

#endif /* dsphelpers_h */
