#include "FreqShift.h"

template <typename T>
FreqShift<T>::FreqShift(std::shared_ptr<SampleSource<T>> src, float shift) 
{
    
}

template <>
FreqShift<float>::FreqShift(std::shared_ptr<SampleSource<float>> src, float shift) : SampleProc(src), shift(shift)
{
    _rate = src->Rate();
    normShift = shift/_rate;
}


template <>
FreqShift<std::complex<float>>::FreqShift(std::shared_ptr<SampleSource<std::complex<float>>> src, float shift) : SampleProc(src), shift(shift)
{
    _rate = src->Rate();
    normShift = shift/_rate;
}

template <typename T>
void FreqShift<T>::SetShift(float s) 
{
    shift = s;
    normShift = shift/_rate;
}

template <typename T>
void FreqShift<T>::Work(void* input, void* output, int count, size_t sampleid)
{ 
}

template <>
void FreqShift<float>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<float>*>(input);
    auto out = static_cast<std::vector<float>*>(output);
    float theta = 0.0f;
    if(keepPhaseRef) theta = prevPhase;    
    float thetaInc = 2*float(M_PI)*normShift;
    for (size_t i = 0; i < count; i++)
    {        
        out->at(i) = in->at(i) * sin(theta);
        theta += thetaInc;
        if(theta>twoPi)
            theta -= twoPi;
        else if(theta<twoPi)
            theta += twoPi;
    } 
    prevPhase = theta;
}

template <>
void FreqShift<std::complex<float>>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<std::complex<float>>*>(input);
    auto out = static_cast<std::vector<std::complex<float>>*>(output);    
    float theta = 0.0f;
    if(keepPhaseRef) theta = prevPhase;    
    float thetaInc = 2*float(M_PI)*normShift;
    for (size_t i = 1; i < count; i++)
    {        
        out->at(i) = in->at(i) * std::polar(1.0f,theta);
        theta += thetaInc;
        if(theta>twoPi)
            theta -= twoPi;
        else if(theta<twoPi)
            theta += twoPi;
    }     
    prevPhase = theta;
}

template class FreqShift<std::complex<float>>;
template class FreqShift<float>;