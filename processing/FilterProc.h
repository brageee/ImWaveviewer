#pragma once

#include "SampleProc.h"
#include <iostream>
#include <complex>

#include <liquid/liquid.h> //Provides firfilt
 

template<typename T>
class FilterProc : public SampleProc<T, T>
{
public:
    FilterProc(std::shared_ptr<SampleSource<T>> src, FilterVars vars);    
    ~FilterProc();      
    void Work(void *input, void *output, int count, size_t sampleid);
    void Reconfigure(FilterVars vars);
    //void UseProvidedTaps(std::vector<float> taps, int inputType); //0 = cplx input, 1 = real input
    //void UseProvidedTaps(std::vector<std::complex<float>> taps, int inputType); //0 = cplx input
private:
    void InitComplexFilterFromBw();
    void InitComplexFilterFromTaps(std::vector<float> taps);
    void InitComplexFilterFromTaps(std::vector<std::complex<float>> taps);
    void InitRealFilterFromBw();
    void InitRealFilterFromTaps(std::vector<float> taps);
    float As = 60.0f; //stop band attenuation
    float cutoff = 0.0f;
    float centerFreq = 0.0f;    
    float sampleRate;
    firfilt_rrrf realFilter;
    firfilt_cccf complexFilter;
    firfilt_crcf complexFilterRealTaps;
    bool complexTaps = true;

};
