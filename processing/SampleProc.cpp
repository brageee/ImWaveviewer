#include <string.h>
#include "SampleProc.h"

template <typename Tin, typename Tout>
SampleProc<Tin, Tout>::SampleProc(std::shared_ptr<SampleSource<Tin>> src) : src(src)
{
	
}

template <typename Tin, typename Tout>
std::unique_ptr<std::vector<Tout>> SampleProc<Tin, Tout>::GetSamples(size_t start, size_t length)
{
    // TODO: base this on the actual history required  
    size_t localStart, localHistory;
    localHistory = history;
    if(start < history)
    {
        localStart = start;
        localHistory = 0;
    } else
        localStart = start-history;
    
    if(src->Count() < length+history)
        localHistory = length;

    auto samples = src->GetSamples(localStart, length + localHistory);
    if (samples == nullptr)
    	return nullptr;

    auto temp = std::make_unique<std::vector<Tout>>(localHistory + length);
    auto dest = std::make_unique<std::vector<Tout>>(length);
    //QMutexLocker ml(&mutex);
    Work(samples.get(), temp.get(), localHistory + length, start);            
    for (size_t i = localHistory; i < localHistory+length; i++)
    {
        dest.get()->at(i-localHistory) = temp.get()->at(i);
    }        
    return dest;
}

template class SampleProc<std::complex<float>, std::complex<float>>;
template class SampleProc<std::complex<float>, float>;
template class SampleProc<float, float>;
