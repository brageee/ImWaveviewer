#pragma once

#include "SampleProc.h"
#include <iostream>
#include <complex>

template<typename T>
class ImagDetect : public SampleProc<T, float>
{
public:
    ImagDetect(std::shared_ptr<SampleSource<T>> src);    
    void Work(void *input, void *output, int count, size_t sampleid);
};
