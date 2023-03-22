#pragma once

#include "SampleProc.h"
#include <iostream>
#include <complex>

template<typename T>
class FreqShift : public SampleProc<T, T>
{
public:
    FreqShift(std::shared_ptr<SampleSource<T>> src, float shift);    
    void Work(void *input, void *output, int count, size_t sampleid);
    void SetShift(float s );
private:
    float shift = 0.0f;
    float normShift = 0.0f;
    float _rate = 0.0f;
    float prevPhase = 0.0f;
    float twoPi = 2*M_PI;
};
