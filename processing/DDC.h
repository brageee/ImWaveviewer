#pragma once

#include "SampleProc.h"
#include <iostream>
#include <complex>

#include <liquid/liquid.h> //Provides arbitrary resampler and filter
 

template<typename T>
class DDCProc : public SampleProc<T, T>
{
public:
    DDCProc(std::shared_ptr<SampleSource<T>> src, float c, float _r, float _cutoff, float _as);    
    ~DDCProc();  
    std::unique_ptr<std::vector<T>> GetSamples(size_t start, size_t length);
    void Reconfigure(float c, float _r, float _cutoff, float as);
    void Work(void *input, void *output, int count, size_t sampleid);
    virtual size_t Count() { return maxNumSamples; }
private:
    void InitReal();
    void InitComplex();
    float outputRate = 0.0f;
    float inputRate;
    float resampleRate;
    float As = 60.0f; //stop band attenuation
    float cutoff = 0.0f;
    float center = 0.0f;
    float prevPhase = 0.0f;
    float normShift;
    int maxNumSamples;
    int maxSamplesFromSrc;
    firfilt_rrrf realFilter;
    firfilt_crcf complexFilter;
    msresamp_rrrf qr;
    msresamp_crcf qc;

};
