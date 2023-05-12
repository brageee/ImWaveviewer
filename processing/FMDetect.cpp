#include "FMDetect.h"

template <typename T>
FMDetect<T>::FMDetect(std::shared_ptr<SampleSource<T>> src) 
{
    
}

template <>
FMDetect<float>::FMDetect(std::shared_ptr<SampleSource<float>> src) : SampleProc(src)
{
    this->SetHistory(1);
}


template <>
FMDetect<std::complex<float>>::FMDetect(std::shared_ptr<SampleSource<std::complex<float>>> src) : SampleProc(src)
{
    this->SetHistory(1);
}

template <typename T>
void FMDetect<T>::Work(void* input, void* output, int count, size_t sampleid)
{ 
}

template <>
void FMDetect<float>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<float>*>(input);
    auto out = static_cast<std::vector<float>*>(output);
    float prev, current; 
    prev = in->at(0);
    out->at(0) = 0.0f;
    for (size_t i = 1; i < count; i++)
    {        
        current = in->at(i);
        std::complex<float> t = std::complex(prev*current,0.0f);
        out->at(i) = std::arg(t);        
        prev = current;
    } 
}

template <>
void FMDetect<std::complex<float>>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<std::complex<float>>*>(input);
    auto out = static_cast<std::vector<float>*>(output);    
    std::complex<float> prev, current; 
    prev = in->at(0);
    out->at(0) = 0.0f;
    for (size_t i = 1; i < count; i++)
    {        
        current = in->at(i);        
        out->at(i) = std::arg(std::conj(prev)*current);        
        prev = current;
    }     
}

template class FMDetect<std::complex<float>>;
template class FMDetect<float>;