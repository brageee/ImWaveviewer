#include "TakeSkip.h"

template <typename T>
TakeSkip<T>::TakeSkip(std::shared_ptr<SampleSource<T>> src, TakeSkipVars vars)
{       
}

template <>
TakeSkip<float>::TakeSkip(std::shared_ptr<SampleSource<float>> src, TakeSkipVars vars) : SampleProc(src)
{    
    maxSamplesFromSrc = static_cast<int>(this->src->Count());    
    Reconfigure(vars);
}


template <>
TakeSkip<std::complex<float>>::TakeSkip(std::shared_ptr<SampleSource<std::complex<float>>> src, TakeSkipVars vars) : SampleProc(src)
{    
    maxSamplesFromSrc = static_cast<int>(this->src->Count());    
    Reconfigure(vars);
}

template <typename T>
void TakeSkip<T>::CalculateResampleRate() 
{    
    float takes = 0;
    float skips = 0;
    float inserts = 0;
    for (size_t i = 0; i < takeSkipPattern.size(); i++)
    {
        if(takeSkipPattern[i] == 0)
            skips++;
        else if(takeSkipPattern[i] == 1)
            takes++;
        else if(takeSkipPattern[i] == 2)
            inserts++;
    }
    resampleRate = (takes+inserts)/(skips+takes);      
    samplesReqFromSrc = takes+skips; 
    samplesProduced = takes+inserts; 
}

template <typename T>
void TakeSkip<T>::Reconfigure(TakeSkipVars vars) 
{    
    takeSkipPattern = vars.pattern;
    patternLength = takeSkipPattern.size();
    sampleReference = vars.sampleReference;
    CalculateResampleRate();
    maxNumSamples = maxSamplesFromSrc * resampleRate;
}

template <typename T>
TakeSkip<T>::~TakeSkip() 
{    
}

template <typename T>
std::unique_ptr<std::vector<T>> TakeSkip<T>::GetSamples(size_t start, size_t length)
{       
    //Calculate srcStart that corresponds to closest take/skip command from
    //start
    int numSamplesToReturn = length;
    int offset; //difference between start and srcStart
    int numSymbols; //number of full takeSkip patterns 
    int srcStart; //start we use when calling GetSamples from source          
    int sampleDiff = static_cast<int>(start) - sampleReference;
    int symbolDiff = abs(sampleDiff) / samplesReqFromSrc;

    numSymbols = (numSamplesToReturn/resampleRate)/samplesReqFromSrc+2; //+2 so we handle offsets from take skip pattern start    
    
    if(sampleDiff>0)
        srcStart = sampleReference+symbolDiff*samplesReqFromSrc;
    else
        srcStart = sampleReference-symbolDiff*samplesReqFromSrc;
    if(srcStart < 0)
        std::cout << "Error in take skip" << std::endl;    
    offset = abs(sampleDiff)%samplesReqFromSrc;    
    if(offset > patternLength)
        std::cout << "Error in take skip" << std::endl;
    if(offset < 0)
    {
        std::cout << "Error in take skip" << std::endl;        
    } 
    
    while(this->src->Count() < srcStart + numSymbols*samplesReqFromSrc)
    {
        numSymbols--;
        numSamplesToReturn -= samplesReqFromSrc;
        //symStart -= ofdmSymLen;
    }

    if(numSymbols == 0 || numSamplesToReturn < 0)
    {
        auto temp = std::make_unique<std::vector<T>>(length,0);        
        return temp;
    }

    auto samples = this->src->GetSamples(srcStart, numSymbols*samplesReqFromSrc);
    if (samples == nullptr)
    	return nullptr;

    auto temp = std::make_unique<std::vector<T>>(numSymbols*samplesProduced);
    Work(samples.get(), temp.get(), numSymbols*samplesReqFromSrc, 0);    
    //Out of bounds checks
    if(offset>=samplesProduced)
        offset = 0;
    
    auto dest = std::make_unique<std::vector<T>>(numSamplesToReturn);
    
    for (size_t i = offset; i < numSamplesToReturn+offset; i++)
    {        
        dest.get()->at(i-offset) = temp.get()->at(i);
    }
    //std::cout << "Done" << std::endl;
    return dest;
}



template <typename T>
void TakeSkip<T>::Work(void* input, void* output, int count, size_t sampleid)
{ 
}

template <>
void TakeSkip<float>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<float>*>(input);
    auto out = static_cast<std::vector<float>*>(output);
        
    out->clear();
    int patternIdx = 0;
    int sampleIdx = 0;      
    while (sampleIdx < count)
    {
        if(takeSkipPattern[patternIdx] == 0) //skip
        {
            sampleIdx++;            
        }
        else if(takeSkipPattern[patternIdx] == 1) //take
        {
            out->push_back(in->at(sampleIdx));            
            sampleIdx++;
        }            
        else if (takeSkipPattern[patternIdx] == 2) //insert
        {
            out->push_back(0.0f);            
        }
        patternIdx++;
        if(patternIdx == patternLength)
            patternIdx = 0;
    }   
    int a  =0; 
}

template <>
void TakeSkip<std::complex<float>>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<std::complex<float>>*>(input);
    auto out = static_cast<std::vector<std::complex<float>>*>(output);
    out->clear();
    int patternIdx = 0;
    int sampleIdx = 0;    
    int sampleOutCnter = 0;
    while (sampleIdx < count)
    {
        if(takeSkipPattern[patternIdx] == 0) //skip
        {
            sampleIdx++;            
        }
        else if(takeSkipPattern[patternIdx] == 1) //take
        {
            out->push_back(in->at(sampleIdx));
            sampleOutCnter++;
            sampleIdx++;
        }            
        else if (takeSkipPattern[patternIdx] == 2) //insert
        {
            out->push_back(std::complex(0.0f,0.0f));
            sampleOutCnter++;
        }
        patternIdx++;
        if(patternIdx == patternLength)
            patternIdx = 0;
    }
}

template class TakeSkip<std::complex<float>>;
template class TakeSkip<float>;