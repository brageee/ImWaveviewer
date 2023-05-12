#include "PhaseDetect.h"

template <typename T>
PhaseDetect<T>::PhaseDetect(std::shared_ptr<SampleSource<T>> src) 
{
    
}

template <>
PhaseDetect<float>::PhaseDetect(std::shared_ptr<SampleSource<float>> src) : SampleProc(src)
{

}


template <>
PhaseDetect<std::complex<float>>::PhaseDetect(std::shared_ptr<SampleSource<std::complex<float>>> src) : SampleProc(src)
{
    
}

template <typename T>
void PhaseDetect<T>::Work(void* input, void* output, int count, size_t sampleid)
{ 
}

template <>
void PhaseDetect<float>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<float>*>(input);
    auto out = static_cast<std::vector<float>*>(output);    
    for (size_t i = 0; i < count; i++)
    {                
        std::complex<float> t = std::complex(in->at(i),0.0f);
        out->at(i) = -std::arg(t);                
    } 
}

template <>
void PhaseDetect<std::complex<float>>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<std::complex<float>>*>(input);
    auto out = static_cast<std::vector<float>*>(output);        
    for (size_t i = 0; i < count; i++)
    {                
        out->at(i) = -std::arg(in->at(i));                
    }     
}

template class PhaseDetect<std::complex<float>>;
template class PhaseDetect<float>;