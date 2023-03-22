#pragma once

#include "SampleProc.h"
#include <iostream>
#include <complex>

template<typename T>
class RealDetect : public SampleProc<T, float>
{
public:
    RealDetect(std::shared_ptr<SampleSource<T>> src);    
    void Work(void *input, void *output, int count, size_t sampleid);
};
