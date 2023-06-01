#pragma once

#include "SampleProc.h"
#include <iostream>
#include <complex>
#include "Structs.h"

template<typename T>
class TakeSkip : public SampleProc<T, T>
{
public:
    TakeSkip(std::shared_ptr<SampleSource<T>> src, TakeSkipVars vars);    
    ~TakeSkip();  
    std::unique_ptr<std::vector<T>> GetSamples(size_t start, size_t length);
    void SetOutputRate(float r);
    void Reconfigure(TakeSkipVars vars);
    void Work(void *input, void *output, int count, size_t sampleid);
    virtual size_t Count() { return maxNumSamples; }
private:        
    void CalculateResampleRate();
    int maxNumSamples;
    int maxSamplesFromSrc;
    float resampleRate = 0.0f;
    int sampleReference;
    std::vector<int> takeSkipPattern;
    int patternLength;
    int samplesReqFromSrc;
    int samplesProduced;
    
};
