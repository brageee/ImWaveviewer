#include "DDC.h"

template <typename T>
DDCProc<T>::DDCProc(std::shared_ptr<SampleSource<T>> src, float c, float _r, float _cutoff, float as)
{
    
}

template <>
DDCProc<float>::DDCProc(std::shared_ptr<SampleSource<float>> src, float c, float _r, float _cutoff, float _as) : SampleProc(src), center(c), outputRate(_r), cutoff(_cutoff), As(_as)
{
    inputRate = this->src->Rate();
    normShift = center/inputRate;
    prevPhase = 0.0f;
    resampleRate = outputRate / inputRate; 
    maxSamplesFromSrc = static_cast<int>(this->src->Count());
    maxNumSamples = maxSamplesFromSrc * resampleRate;  
    InitReal() ;
}


template <>
DDCProc<std::complex<float>>::DDCProc(std::shared_ptr<SampleSource<std::complex<float>>> src,  float c, float _r, float _cutoff, float _as) : SampleProc(src), center(c), outputRate(_r), cutoff(_cutoff), As(_as)
{
    inputRate = this->src->Rate();
    normShift = center/inputRate;
    prevPhase = 0.0f;
    resampleRate = outputRate / inputRate;
    maxSamplesFromSrc = static_cast<int>(this->src->Count());
    maxNumSamples = maxSamplesFromSrc * resampleRate;
    InitComplex();
}

template <typename T>
void DDCProc<T>::InitReal()
{
    qr = msresamp_rrrf_create(resampleRate, As); 
    unsigned int h_len = estimate_req_filter_len(0.05f, As);
    float h[h_len];
    liquid_firdes_kaiser(h_len, cutoff/inputRate, As, 0.0f, h); 
    realFilter = firfilt_rrrf_create(h, h_len);
}

template <typename T>
void DDCProc<T>::InitComplex()
{
    qc = msresamp_crcf_create(resampleRate, As); 
    unsigned int h_len = estimate_req_filter_len(0.05f, As);
    complexFilter = firfilt_crcf_create_kaiser(h_len, cutoff/inputRate, As, 0.0f);
}

template <typename T>
void DDCProc<T>::Reconfigure(float c, float _r, float _cutoff, float as)
{
    
}

template <>
void DDCProc<float>::Reconfigure(float c, float _r, float _cutoff, float as)
{
    center = c; 
    outputRate = _r;
    cutoff = _cutoff,
    As = as;
    normShift = center/inputRate;
    prevPhase = 0.0f;
    resampleRate = outputRate / inputRate;
    firfilt_rrrf_destroy(realFilter);
    msresamp_rrrf_destroy(qr);
    InitReal();
}

template <>
void DDCProc<std::complex<float>>::Reconfigure(float c, float _r, float _cutoff, float as)
{
    center = c; 
    outputRate = _r;
    cutoff = _cutoff,
    As = as;
    normShift = center/inputRate;
    prevPhase = 0.0f;
    resampleRate = outputRate / inputRate;
    firfilt_crcf_destroy(complexFilter);
    msresamp_crcf_destroy(qc);
    InitComplex();
}

template <typename T>
DDCProc<T>::~DDCProc() 
{
    //msresamp_rrrf_destroy(qr);
    //msresamp_crcf_destroy(qc);
}

template <typename T>
std::unique_ptr<std::vector<T>> DDCProc<T>::GetSamples(size_t start, size_t length)
{
    // TODO: base this on the actual history required
    //auto history = std::min(start, (size_t)256);
    size_t history = 0;
    auto samples = this->src->GetSamples(start - history, (length + history)/resampleRate);
    if (samples == nullptr)
    	return nullptr;

    auto temp = std::make_unique<std::vector<T>>(history + length);
    auto dest = std::make_unique<std::vector<T>>(length);
    //QMutexLocker ml(&mutex);
    Work(samples.get(), temp.get(), (length + history)/resampleRate, start);        
    for (size_t i = history; i < history+length; i++)
    {
        dest.get()->at(i-history) = temp.get()->at(i);
    }
    return dest;
}



template <typename T>
void DDCProc<T>::Work(void* input, void* output, int count, size_t sampleid)
{ 
}

template <>
void DDCProc<float>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<float>*>(input);
    auto out = static_cast<std::vector<float>*>(output);
    float resampled[1];
    unsigned int numResampled;
    float filtered[count];
    float shifted;
    float theta = prevPhase;
    float thetaInc = 2*float(M_PI)*normShift;
    out->resize(count);
    int numResampledTot = 0;
    for (size_t i = 0; i < count; i++)
    {
        shifted = in->at(i) * sin(theta);
        theta += thetaInc;
        firfilt_rrrf_push(realFilter, shifted);
        firfilt_rrrf_execute(realFilter, &filtered[i]);
        /*msresamp_rrrf_execute(qr, filtered, 1, resampled, &numResampled);
        if(numResampled>0)
        {
            numResampledTot++;
            out->at(i) = resampled[0];
        }*/
    }
    unsigned int numResampledSamples;
    msresamp_rrrf_execute(qr, filtered, count, out->data(), &numResampledSamples);
    out->resize(numResampledSamples);
    prevPhase = theta;
    //out->resize(numResampledTot);
}

template <>
void DDCProc<std::complex<float>>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<std::complex<float>>*>(input);
    auto out = static_cast<std::vector<std::complex<float>>*>(output);
    std::complex<float> resampled[count];
    unsigned int numResampled;
    std::complex<float> filtered[count];
    std::complex<float> shifted;
    float theta = prevPhase;
    float thetaInc = 2*float(M_PI)*normShift;
    out->resize(count);
    int numResampledTot = 0;
    for (size_t i = 0; i < count; i++)
    {
        shifted = in->at(i) * std::polar(1.0f, theta);
        theta += thetaInc;
        firfilt_crcf_push(complexFilter, shifted);
        firfilt_crcf_execute(complexFilter, &filtered[i]);
        /*msresamp_crcf_execute(qc, filtered, 1, resampled, &numResampled);
        if(numResampled>0)
        {
            numResampledTot++;
            out->at(i) = resampled[0];
        }
        */
    }
    //out->resize(numResampledTot);
    unsigned int numResampledSamples;
    msresamp_crcf_execute(qc, filtered, count, out->data(), &numResampledSamples);
    out->resize(numResampledSamples);
    prevPhase = theta;
    
}

template class DDCProc<std::complex<float>>;
template class DDCProc<float>;