#pragma once

#include "SampleProc.h"
#include <iostream>
#include <complex>

template<typename T>
class FMDetect : public SampleProc<T, float>
{
public:
    FMDetect(std::shared_ptr<SampleSource<T>> src);    
    void Work(void *input, void *output, int count, size_t sampleid);
};
