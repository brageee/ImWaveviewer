#pragma once

#include "SampleProc.h"
#include <iostream>

template<typename T>
class AMDetect : public SampleProc<T, float>
{
public:
    AMDetect(std::shared_ptr<SampleSource<T>> src);    
    void Work(void *input, void *output, int count, size_t sampleid);
};
