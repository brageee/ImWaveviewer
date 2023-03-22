#pragma once

#include "SampleProc.h"
#include <iostream>
#include <complex>

#include <liquid/liquid.h> //Provides arbitrary resampler
 

template<typename T>
class Resampler : public SampleProc<T, T>
{
public:
    Resampler(std::shared_ptr<SampleSource<T>> src, float _or, float ir, float as);    
    ~Resampler();  
    std::unique_ptr<std::vector<T>> GetSamples(size_t start, size_t length);
    void SetOutputRate(float r);
    void Work(void *input, void *output, int count, size_t sampleid);
    virtual size_t Count() { return maxNumSamples; }
private:
    int maxNumSamples;
    int maxSamplesFromSrc;
    float resampleRate = 0.0f;
    float outputRate;
    float inputRate;
    float As = 60.0f; //stop band attenuation
    msresamp_rrrf qr;
    msresamp_crcf qc;
};
