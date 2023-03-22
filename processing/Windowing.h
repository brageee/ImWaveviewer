#include <vector>
#include <complex>
#include <math.h>
#include <iostream>

#include "Filter.h"

#pragma once

using namespace std;

namespace processing::windowing
{
    typedef enum {
        WINDOW_UNKNOWN=0,        // unknown/unsupported scheme
        WINDOW_HAMMING,          // Hamming
        WINDOW_HANN,             // Hann
        WINDOW_BLACKMANHARRIS,   // Blackman-harris (4-term)
        WINDOW_BLACKMANHARRIS7,  // Blackman-harris (7-term)
        WINDOW_KAISER,           // Kaiser (beta factor unspecified)
        WINDOW_FLATTOP,          // flat top (includes negative values)
        WINDOW_TRIANGULAR,       // triangular
        WINDOW_RCOSTAPER,        // raised-cosine taper (taper size unspecified)
        WINDOW_KBD,              // Kaiser-Bessel derived window (beta factor unspecified)
    }  WindowType;    
    
    float WindowStepHAMMING(unsigned int idx, unsigned int windowLength);
    
    float WindowStepHANN(unsigned int idx, unsigned int windowLength);
    
    float WindowStepBLACKMANHARRIS(unsigned int idx, unsigned int windowLength);
    
    float WindowStepBLACKMANHARRIS7(unsigned int idx, unsigned int windowLength);
    
    float WindowStepKAISER(unsigned int idx, unsigned int windowLength, float beta);
    
    float WindowStepFLATTOP(unsigned int idx, unsigned int windowLength);

    float WindowStepTRIANGULAR(unsigned int idx, unsigned int windowLength, float n);

    float WindowStepRCOSTAPER(unsigned int idx, unsigned int windowLength, float t);
    
    float WindowStepKBD(unsigned int idx, unsigned int windowLength, float zeta);

    void SetupWindow(WindowType wt, int windowLength, vector<float> &output);
    /*foat WindowStep(WindowType WINDOW_HANN, float idx, float windowLength)
    {
        if(idx>=windowLength)
        {
            cout << "Window Hann step error, idx>=windowLength" << endl;
            return 0.0f;
        }
        return 0.5f - 0.5f*cosf( (2*M_PI*idx) / (windowLength-1) );
    }*/

/*
    // Kaiser window [Kaiser:1980]
//  _i      :   sample index
//  _wlen   :   window length (samples)
//  _beta   :   window taper parameter
float liquid_kaiser(unsigned int _i,
                    unsigned int _wlen,
                    float        _beta)
{
    // validate input
    if (_i >= _wlen) {
        cout << "liquid_kaiser(), sample index must not exceed window length" << endl;
        return 0.0f;
    } else if (_beta < 0) {
        cout << "liquid_kaiser(), beta must be greater than or equal to zero" << endl;
        return 0.0f;
    }

    float t = (float)_i - (float)(_wlen-1)/2;
    float r = 2.0f*t/(float)(_wlen-1);
    float a = liquid_besseli0f(_beta*sqrtf(1-r*r));
    float b = liquid_besseli0f(_beta);
    //printf("kaiser(%3u,%u3,%6.3f) t:%8.3f, r:%8.3f, a:%8.3f, b:%8.3f\n", _i,_wlen,_beta,t,r,a,b);
    return a / b;
}

// Hamming window [Nuttall:1981]
float liquid_hamming(unsigned int _i,
                     unsigned int _wlen)
{
    // validate input
    if (_i > _wlen) {
        cout << "liquid_hamming(), sample index must not exceed window length" << endl;
        return 0.0f;
    }

    return 0.53836 - 0.46164*cosf( (2*M_PI*(float)_i) / ((float)(_wlen-1)) );
}

// Hann window
float liquid_hann(unsigned int _i,
                  unsigned int _wlen)
{
    // validate input
    if (_i > _wlen) {
        cout << "liquid_hann(), sample index must not exceed window length" << endl;
        return 0.0f;
    }

    // TODO test this function
    // TODO add reference
    return 0.5f - 0.5f*cosf( (2*M_PI*(float)_i) / ((float)(_wlen-1)) );
}

// Blackman-harris window [harris:1978]
float Blackmanharris(unsigned int _i,
                            unsigned int _wlen)
{
    // validate input
    if (_i > _wlen) {
        cout << "liquid_blackmanharris(), sample index must not exceed window length" << endl;
        return 0.0f;
    }

    // TODO test this function
    // TODO add reference
    float a0 = 0.35875f;
    float a1 = 0.48829f;
    float a2 = 0.14128f;
    float a3 = 0.01168f;
    float t = 2*M_PI*(float)_i / ((float)(_wlen-1));

    return a0 - a1*cosf(t) + a2*cosf(2*t) - a3*cosf(3*t);
}

// 7th-order Blackman-harris window
float BlackmanHarris7(unsigned int _i,
                             unsigned int _wlen)
{
    // validate input
    if (_i > _wlen) {
        cout << "liquid_blackmanharris7(), sample index must not exceed window length" << endl;
        return 0.0f;
    }

	float a0 = 0.27105f;
	float a1 = 0.43329f;
	float a2 = 0.21812f;
	float a3 = 0.06592f;
	float a4 = 0.01081f;
	float a5 = 0.00077f;
	float a6 = 0.00001f;
	float t = 2*M_PI*(float)_i / ((float)(_wlen-1));

	return a0 - a1*cosf(  t) + a2*cosf(2*t) - a3*cosf(3*t)
			  + a4*cosf(4*t) - a5*cosf(5*t) + a6*cosf(6*t);
}

*/

}