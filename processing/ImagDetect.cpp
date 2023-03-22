#include "ImagDetect.h"

template <typename T>
ImagDetect<T>::ImagDetect(std::shared_ptr<SampleSource<T>> src) 
{
    
}


template <>
ImagDetect<std::complex<float>>::ImagDetect(std::shared_ptr<SampleSource<std::complex<float>>> src) : SampleProc(src)
{
    
}

template <typename T>
void ImagDetect<T>::Work(void* input, void* output, int count, size_t sampleid)
{ 
}

template <>
void ImagDetect<std::complex<float>>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<std::complex<float>>*>(input);
    auto out = static_cast<std::vector<float>*>(output);    
    for (size_t i = 1; i < count; i++)
    {         
        out->at(i) = in->at(i).imag();
    }     
}

template class ImagDetect<std::complex<float>>;