#include "RealDetect.h"

template <typename T>
RealDetect<T>::RealDetect(std::shared_ptr<SampleSource<T>> src) 
{
    
}

template <>
RealDetect<float>::RealDetect(std::shared_ptr<SampleSource<float>> src) : SampleProc(src)
{

}


template <>
RealDetect<std::complex<float>>::RealDetect(std::shared_ptr<SampleSource<std::complex<float>>> src) : SampleProc(src)
{
    
}

template <typename T>
void RealDetect<T>::Work(void* input, void* output, int count, size_t sampleid)
{ 
}

template <>
void RealDetect<float>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<float>*>(input);
    auto out = static_cast<std::vector<float>*>(output);
    for (size_t i = 1; i < count; i++)
    {        
        out->at(i) = in->at(i);        
    } 
}

template <>
void RealDetect<std::complex<float>>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<std::complex<float>>*>(input);
    auto out = static_cast<std::vector<float>*>(output);    
    for (size_t i = 1; i < count; i++)
    {         
        out->at(i) = in->at(i).real();
    }     
}

template class RealDetect<std::complex<float>>;
template class RealDetect<float>;