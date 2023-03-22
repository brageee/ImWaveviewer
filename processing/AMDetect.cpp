#include "AMDetect.h"

template <typename T>
AMDetect<T>::AMDetect(std::shared_ptr<SampleSource<T>> src) 
{
    
}

template <>
AMDetect<float>::AMDetect(std::shared_ptr<SampleSource<float>> src) : SampleProc(src)
{

}


template <>
AMDetect<std::complex<float>>::AMDetect(std::shared_ptr<SampleSource<std::complex<float>>> src) : SampleProc(src)
{
    
}

template <typename T>
void AMDetect<T>::Work(void* input, void* output, int count, size_t sampleid)
{ 
}

template <>
void AMDetect<float>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<float>*>(input);
    auto out = static_cast<std::vector<float>*>(output);
    for (size_t i = 0; i < count; i++)
    {
        out->at(i) = in->at(i)*in->at(i);        
    } 
}

template <>
void AMDetect<std::complex<float>>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<std::complex<float>>*>(input);
    auto out = static_cast<std::vector<float>*>(output);    
    for (size_t i = 0; i < count; i++)
    {
        float treal = in->at(i).real();
        float timag = in->at(i).imag();
        out->at(i) = treal*treal+timag*timag;
    }     
}

template class AMDetect<std::complex<float>>;
template class AMDetect<float>;