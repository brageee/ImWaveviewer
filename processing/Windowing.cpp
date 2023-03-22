#include "Windowing.h"

using namespace std;

namespace processing::windowing
{
    float WindowStepHAMMING(unsigned int idx, unsigned int windowLength)
    {
        if(idx>=windowLength)
        {
            cout << "Window hamming step error, idx>=windowLength" << endl;
            return 0.0f;
        }
        return 0.53836f - 0.46164f*cosf(2*M_PI*static_cast<float>(idx)/static_cast<float>(windowLength-1));
    }
    
    float WindowStepHANN(unsigned int idx, unsigned int windowLength)
    {
        if(idx>=windowLength)
        {
            cout << "Window Hann step error, idx>=windowLength" << endl;
            return 0.0f;
        }
         return 0.5f - 0.5f*cosf( (2*M_PI*static_cast<float>(idx)) / static_cast<float>(windowLength-1) );
    }
    
    float WindowStepBLACKMANHARRIS(unsigned int idx, unsigned int windowLength)
    {
        if(idx>=windowLength)
        {
            cout << "Window WINDOW_BLACKMANHARRIS step error, idx>=windowLength" << endl;
            return 0.0f;
        }
        float a0 = 0.35875f;
        float a1 = 0.48829f;
        float a2 = 0.14128f;
        float a3 = 0.01168f;
        float t = 2*M_PI*static_cast<float>(idx) / static_cast<float>(windowLength-1);

        return a0 - a1*cosf(t) + a2*cosf(2*t) - a3*cosf(3*t);
    }
    
    float WindowStepBLACKMANHARRIS7(unsigned int idx, unsigned int windowLength)
    {
        if(idx>=windowLength)
        {
            cout << "Window WINDOW_BLACKMANHARRIS7 step error, idx>=windowLength" << endl;
            return 0.0f;
        }
        float a0 = 0.27105f;
        float a1 = 0.43329f;
        float a2 = 0.21812f;
        float a3 = 0.06592f;
        float a4 = 0.01081f;
        float a5 = 0.00077f;
        float a6 = 0.00001f;
        float t = 2*M_PI*static_cast<float>(idx) / static_cast<float>(windowLength-1);

        return a0 - a1*cosf(  t) + a2*cosf(2*t) - a3*cosf(3*t)
                + a4*cosf(4*t) - a5*cosf(5*t) + a6*cosf(6*t);
    }
    
    float WindowStepKAISER(unsigned int idx, unsigned int windowLength, float beta)
    {
        if(idx>=windowLength || beta < 0)
        {
            cout << "Window WINDOW_BLACKMANHARRIS7 step error, idx>=windowLength" << endl;
            return 0.0f;
        }
        return liquid_kaiser(idx, windowLength, beta);
    }
    
    float WindowStepFLATTOP(unsigned int idx, unsigned int windowLength)
    {
        if(idx>=windowLength)
        {
            cout << "Window WINDOW_BLACKMANHARRIS7 step error, idx>=windowLength" << endl;
            return 0.0f;
        }
        float a0 = 1.000f;
        float a1 = 1.930f;
        float a2 = 1.290f;
        float a3 = 0.388f;
        float a4 = 0.028f;
        float t = 2*M_PI*static_cast<float>(idx) / static_cast<float>(windowLength-1);

        return a0 - a1*cosf(t) + a2*cosf(2*t) - a3*cosf(3*t) + a4*cosf(4*t);
    }

    float WindowStepTRIANGULAR(unsigned int idx, unsigned int windowLength, float n)
    {
        if(idx>=windowLength)
        {
            cout << "Window TRIANGULAR step error, idx>=windowLength" << endl;
            return 0.0f;
        }
        float v0 = static_cast<float>(idx) - static_cast<float>(windowLength-1)/2.0f;
        float v1 = (n)/2.0f;
        return 1.0 - fabsf(v0 / v1);
    }

    float WindowStepRCOSTAPER(unsigned int idx, unsigned int windowLength, float t)
    {
        if(idx>=windowLength)
        {
            cout << "Window WINDOW_BLACKMANHARRIS7 step error, idx>=windowLength" << endl;
            return 0.0f;
        }
        float _i = static_cast<float>(idx);
        if (idx > static_cast<float>(windowLength) - t - 1)
            _i = static_cast<float>(windowLength) - _i - 1;

        // return ramp or flat component
        return (_i < t) ? 0.5f - 0.5f*cosf(M_PI*(_i + 0.5f) / t) : 1.0f;
    }
    
    float WindowStepKBD(unsigned int idx, unsigned int windowLength, float zeta)
    {
        if(idx>=windowLength)
        {
            cout << "Window WINDOW_BLACKMANHARRIS7 step error, idx>=windowLength" << endl;
            return 0.0f;
        }
        return liquid_kbd(idx,windowLength,zeta);
    }

    void SetupWindow(WindowType wt, int windowLength, vector<float> &output)
    {
        vector<float> taps(windowLength);
        output.resize(windowLength);
        float scale = 0.0f;
        for (int i = 0; i < windowLength; i++)
        {
            switch (wt) {
            case WINDOW_HAMMING:         
                taps[i] = WindowStepHAMMING(i,windowLength);   
                scale += taps[i]*taps[i];                      
                break;
            case WINDOW_HANN:
                taps[i] = WindowStepHANN(i,windowLength);   
                scale += taps[i]*taps[i];      
                break;
            case WINDOW_BLACKMANHARRIS: 
                taps[i] = WindowStepBLACKMANHARRIS(i,windowLength);   
                scale += taps[i]*taps[i];      
                break;
            case WINDOW_BLACKMANHARRIS7:
                taps[i] = WindowStepBLACKMANHARRIS7(i,windowLength);   
                scale += taps[i]*taps[i];      
                break;
            case WINDOW_KAISER: 
                taps[i] = WindowStepKAISER(i,windowLength,1);   
                scale += taps[i]*taps[i];      
                break;
            case WINDOW_FLATTOP:
                taps[i] = WindowStepFLATTOP(i,windowLength);   
                scale += taps[i]*taps[i];      
                break;
            case WINDOW_TRIANGULAR:
                taps[i] = WindowStepTRIANGULAR(i,windowLength, windowLength);   
                scale += taps[i]*taps[i];      
                break;
            case WINDOW_RCOSTAPER:      
                taps[i] = WindowStepRCOSTAPER(i,windowLength, 1);   
                scale += taps[i]*taps[i];      
                break;
            case WINDOW_KBD:
                taps[i] = WindowStepKBD(i,windowLength,1);   
                scale += taps[i]*taps[i];      
                break;
            default:
                cout << "Failed to create window tap point " << endl;
                break;
            }
        }
        scale = 1 / sqrt(scale);
        for (size_t i = 0; i < windowLength; i++)
        {
            output[i] = taps[i] * scale;
        }
    }
}