#include "FilterProc.h"

template <typename T>
FilterProc<T>::FilterProc(std::shared_ptr<SampleSource<T>> src, FilterVars vars)
{
    
}

template <>
FilterProc<float>::FilterProc(std::shared_ptr<SampleSource<float>> src, FilterVars vars) : SampleProc(src)
{
    sampleRate = static_cast<float>(this->src->Rate());
    centerFreq = vars.centerFreq;
    cutoff = vars.cutoff;
    As = vars.attenuation;
    if(vars.filterType == 0)
        InitRealFilterFromBw();
    else
        InitRealFilterFromTaps(vars.realTaps);
}


template <>
FilterProc<std::complex<float>>::FilterProc(std::shared_ptr<SampleSource<std::complex<float>>> src,  FilterVars vars) : SampleProc(src)
{
    sampleRate = static_cast<float>(this->src->Rate());   
    centerFreq = vars.centerFreq;
    cutoff = vars.cutoff;
    As = vars.attenuation;
    if(vars.filterType == 0)
        InitComplexFilterFromBw();
    else if(vars.filterType == 1)
        InitComplexFilterFromTaps(vars.realTaps);
    else
        InitComplexFilterFromTaps(vars.complexTaps);   
}

template <typename T>
void FilterProc<T>::InitRealFilterFromBw() 
{
    unsigned int h_len = estimate_req_filter_len(0.05f, As);
    float h[h_len];
    liquid_firdes_kaiser(h_len, cutoff/sampleRate, As, 0.0f, h);    
    for (size_t i = 0; i < h_len; i++)
    {
        float argument  = ((float)i - 0.5f*(float)h_len) * centerFreq / sampleRate;        
        h[i] *= cosf(2*M_PI*argument);        
    }    
    realFilter = firfilt_rrrf_create(h, h_len);
    this->SetHistory(h_len);   
}

template <typename T>
void FilterProc<T>::InitRealFilterFromTaps(std::vector<float> taps) 
{            
    realFilter = firfilt_rrrf_create(taps.data(), taps.size());
    this->SetHistory(taps.size());   
}

template <typename T>
void FilterProc<T>::InitComplexFilterFromBw() 
{
    unsigned int h_len = estimate_req_filter_len(0.05f, As);
    float h[h_len];    
    liquid_firdes_kaiser(h_len, cutoff/sampleRate, As, 0.0f, h);
    std::vector<std::complex<float>> hc(h_len);
    for (size_t i = 0; i < h_len; i++)
    {
        float argument  = ((float)i - 0.5f*(float)h_len) * centerFreq / sampleRate;
        hc[i] = h[i] * std::polar(1.0f, static_cast<float>(2*M_PI*argument));
        //h[i] *= cosf(2*M_PI*argument);        
    }    
    complexFilter = firfilt_cccf_create(hc.data(), h_len); 
    complexTaps = true;
    this->SetHistory(h_len);   
}


template <typename T>
void FilterProc<T>::InitComplexFilterFromTaps(std::vector<float> taps) 
{            
    complexFilterRealTaps = firfilt_crcf_create(taps.data(), taps.size());
    complexTaps = false;
    this->SetHistory(taps.size());   
}

template <typename T>
void FilterProc<T>::InitComplexFilterFromTaps(std::vector<std::complex<float>> taps) 
{            
    complexFilter = firfilt_cccf_create(taps.data(), taps.size());   
    complexTaps = true; 
    this->SetHistory(taps.size());   
}

template <typename T>
FilterProc<T>::~FilterProc() 
{
    
}

template <>
FilterProc<std::complex<float>>::~FilterProc() 
{
    
}

template <>
FilterProc<float>::~FilterProc() 
{
    
}

template <typename T>
void FilterProc<T>::Reconfigure(FilterVars vars)
{
    
}

template <>
void FilterProc<float>::Reconfigure(FilterVars vars)
{
    centerFreq = vars.centerFreq;
    cutoff = vars.cutoff;
    As = vars.attenuation;
    firfilt_rrrf_destroy(realFilter);
    if(vars.filterType == 0)
        InitRealFilterFromBw();
    else
        InitRealFilterFromTaps(vars.realTaps);
        
}

template <>
void FilterProc<std::complex<float>>::Reconfigure(FilterVars vars)
{
    centerFreq = vars.centerFreq;
    cutoff = vars.cutoff;
    As = vars.attenuation;
    if(vars.filterType == 0)
    {
        firfilt_cccf_destroy(complexFilter);
        InitComplexFilterFromBw();        
    }        
    else if(vars.filterType == 1)
    {
        firfilt_crcf_destroy(complexFilterRealTaps);
        InitComplexFilterFromTaps(vars.realTaps);        
    }        
    else
    {
        complexTaps = true;
        InitComplexFilterFromTaps(vars.complexTaps);          
    }
}


template <typename T>
void FilterProc<T>::Work(void* input, void* output, int count, size_t sampleid)
{ 
}

template <>
void FilterProc<float>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<float>*>(input);
    auto out = static_cast<std::vector<float>*>(output);    
    out->resize(count);
    firfilt_rrrf_execute_block(realFilter, in->data(), count, out->data());    
}

template <>
void FilterProc<std::complex<float>>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<std::complex<float>>*>(input);
    auto out = static_cast<std::vector<std::complex<float>>*>(output);
    out->resize(count);
    if(complexTaps)
        firfilt_cccf_execute_block(complexFilter, in->data(), count, out->data());    
    else
        firfilt_crcf_execute_block(complexFilterRealTaps, in->data(), count, out->data());    
}

template class FilterProc<std::complex<float>>;
template class FilterProc<float>;