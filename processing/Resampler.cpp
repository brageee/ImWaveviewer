#include "Resampler.h"

template <typename T>
Resampler<T>::Resampler(std::shared_ptr<SampleSource<T>> src, float _or, float ir, float _as) 
{
    
}

template <>
Resampler<float>::Resampler(std::shared_ptr<SampleSource<float>> src, float _or, float ir, float _as) : SampleProc(src), outputRate(_or), inputRate(ir), As(_as)
{
    resampleRate = outputRate / inputRate;
    qr = msresamp_rrrf_create(resampleRate, As); 
    qc = msresamp_crcf_create(resampleRate, As);  
    maxSamplesFromSrc = static_cast<int>(this->src->Count());
    maxNumSamples = maxSamplesFromSrc * resampleRate;
    
}


template <>
Resampler<std::complex<float>>::Resampler(std::shared_ptr<SampleSource<std::complex<float>>> src, float _or, float ir, float _as) : SampleProc(src),  outputRate(_or), inputRate(ir),  As(_as)
{
    resampleRate = outputRate / inputRate;
    qr = msresamp_rrrf_create(resampleRate, As); 
    qc = msresamp_crcf_create(resampleRate, As); 
    maxSamplesFromSrc = static_cast<int>(this->src->Count());
    maxNumSamples = maxSamplesFromSrc * resampleRate;
}

template <typename T>
Resampler<T>::~Resampler() 
{
    msresamp_rrrf_destroy(qr);
    msresamp_crcf_destroy(qc);
}

template <typename T>
void Resampler<T>::SetOutputRate(float r)
{
    outputRate = r;
    resampleRate = outputRate / inputRate;
    maxNumSamples = maxSamplesFromSrc * resampleRate;
    msresamp_rrrf_destroy(qr);
    msresamp_crcf_destroy(qc);
    qr = msresamp_rrrf_create(resampleRate, As);
    qc = msresamp_crcf_create(resampleRate, As);
} 

template <typename T>
std::unique_ptr<std::vector<T>> Resampler<T>::GetSamples(size_t start, size_t length)
{
    // TODO: base this on the actual history required    
    //size_t history = 0;
    int resampLen = static_cast<int>(length)/resampleRate+5;
    int outputLen = static_cast<int>(length);
    int srcStart = static_cast<int>(start)/resampleRate;
    if(srcStart+resampLen +5 > maxSamplesFromSrc)
    {
        resampLen = maxSamplesFromSrc-srcStart-5;//static_cast<int>(start);
        outputLen = resampLen*resampleRate;
    }
    if(outputLen <= 0)
    {
        auto temp = std::make_unique<std::vector<T>>(length,0);        
        return temp;
    }
    auto samples = this->src->GetSamples(srcStart, resampLen);
    if (samples == nullptr)
    	return nullptr;

    auto temp = std::make_unique<std::vector<T>>(outputLen);
    Work(samples.get(), temp.get(), resampLen, 0);    
    int samplesFromWork = temp.get()->size();
    if(samplesFromWork < outputLen)
        outputLen = samplesFromWork;
    auto dest = std::make_unique<std::vector<T>>(outputLen);
    //QMutexLocker ml(&mutex);
        
    for (size_t i = 0; i < outputLen; i++)
    {
        dest.get()->at(i) = temp.get()->at(i);
    }
    return dest;
}



template <typename T>
void Resampler<T>::Work(void* input, void* output, int count, size_t sampleid)
{ 
}

template <>
void Resampler<float>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<float>*>(input);
    auto out = static_cast<std::vector<float>*>(output);
    unsigned int nyAlloc = int(2*count*resampleRate);
    //liquid_float_complex *x = (liquid_float_complex*) malloc(nx*sizeof(liquid_float_complex));
    //float x[nx];
    float y[nyAlloc];
    unsigned int numResampledSamples;
    msresamp_rrrf_execute(qr, in->data(), count, y, &numResampledSamples);
    out->resize(numResampledSamples);
    for (size_t i = 0; i < numResampledSamples; i++)
    {
        out->at(i) = y[i];
    }
}

template <>
void Resampler<std::complex<float>>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<std::complex<float>>*>(input);
    auto out = static_cast<std::vector<std::complex<float>>*>(output);
    unsigned int nyAlloc = int(2*count*resampleRate);
    //liquid_float_complex *x = (liquid_float_complex*) malloc(nx*sizeof(liquid_float_complex));
    //float x[nx];
    std::complex<float> y[nyAlloc];
    unsigned int numResampledSamples;
    msresamp_crcf_execute(qc, in->data(), count, y, &numResampledSamples);
    out->resize(numResampledSamples);
    for (size_t i = 0; i < numResampledSamples; i++)
    {
        out->at(i) = y[i];
    }
}

template class Resampler<std::complex<float>>;
template class Resampler<float>;