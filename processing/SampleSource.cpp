#include "SampleSource.h"

template<typename T>
std::type_index SampleSource<T>::SampleType()
{
    return typeid(std::complex<float>);
}

/*
template<typename T>
double SampleSource<T>::GetFrequency()
{
    return frequency;
}
*/

template class SampleSource<std::complex<float>>;
template class SampleSource<float>;
