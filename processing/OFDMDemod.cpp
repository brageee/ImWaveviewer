#include "OFDMDemod.h"

template <typename T>
OFDMDemod<T>::OFDMDemod(std::shared_ptr<SampleSource<T>> src, OFDMDemodVars vars)
{
   
}

template <>
OFDMDemod<std::complex<float>>::OFDMDemod(std::shared_ptr<SampleSource<std::complex<float>>> src, OFDMDemodVars vars) : SampleProc(src)
{
    fftSize = vars.fftSize;
    cpLen = vars.cpLen;
    ofdmSymLen = fftSize+cpLen;
    sampleReference = vars.sampleReference;
    ofdmSymStartRef = sampleReference+vars.symStart;
    cpBasedShift = vars.cpBasedShift;
    initFreqShift = vars.initFreqShift;    
    numSymsToEstimateOffset = vars.numSymsToEstimateOffset; 
    numPilotCarriers = vars.pilotCarrierIndices.size(); 
    pilotCarrierEq = vars.pilotCarrierEq;
    pilotCarrierIndices = vars.pilotCarrierIndices;  
    pilotPhaseError.resize(numPilotCarriers);
    interPoints.resize(numPilotCarriers);
    sampleRate = static_cast<float>(src->Rate());
    InitProc();
}

template <typename T>
void OFDMDemod<T>::InitProc()
{
    fftIn = (fftwf_complex*) fftwf_malloc(fftSize * sizeof(fftwf_complex)); 
    fftOut = (fftwf_complex*) fftwf_malloc(fftSize * sizeof(fftwf_complex)); 
    plan = fftwf_plan_dft_1d(fftSize, fftIn, fftOut, FFTW_FORWARD, FFTW_ESTIMATE);    

    //Estimate freq offset (can only correct up to carrier-spacing/2)
    if(initFreqShift)
        initialFreqOffset = GetCarrierOffset(numSymsToEstimateOffset);   
}

template <typename T>
void OFDMDemod<T>::Reconfigure(OFDMDemodVars vars)
{

}

template <>
void OFDMDemod<std::complex<float>>::Reconfigure(OFDMDemodVars vars)
{
    fftSize = vars.fftSize;
    cpLen = vars.cpLen;
    ofdmSymLen = fftSize+cpLen;
    sampleReference = vars.sampleReference;
    ofdmSymStartRef = sampleReference+vars.symStart;
    cpBasedShift = vars.cpBasedShift;
    initFreqShift = vars.initFreqShift;    
    numSymsToEstimateOffset = vars.numSymsToEstimateOffset; 
    pilotCarrierIndices = vars.pilotCarrierIndices;  
    numPilotCarriers = pilotCarrierIndices.size();
    pilotCarrierEq = vars.pilotCarrierEq;    
    pilotPhaseError.resize(numPilotCarriers);
    interPoints.resize(numPilotCarriers);

    fftwf_free(fftIn);
    fftwf_free(fftOut);
    fftwf_destroy_plan(plan);

    InitProc();
}

template <typename T>
OFDMDemod<T>::~OFDMDemod() 
{
    
}

template <typename T>
std::unique_ptr<std::vector<T>> OFDMDemod<T>::GetSamples(size_t start, size_t length)
{
    // TODO: base this on the actual history required
    //auto history = std::min(start, (size_t)256);
    size_t history = 0;
    //Find closest OFDM symbol start
    int numSamplesToReturn = length;
    int numSymbols; //number of OFDM symbols we in demod (length)/ofdmSymLen+1
    int symStart; //OFDM symbol start that we use when calling GetSamples from src
    int offset; //difference between start and symStart,
    numSymbols = numSamplesToReturn/fftSize+2; //+2 so we handle offsets from OFDM symbol start          

    //Find closest OFDM symbol to start
    int sampleDiff = static_cast<int>(start) - ofdmSymStartRef;
    int symbolDiff = abs(sampleDiff) / ofdmSymLen;
    if(sampleDiff>0)
        symStart = ofdmSymStartRef+symbolDiff*ofdmSymLen;
    else
        symStart = ofdmSymStartRef-symbolDiff*ofdmSymLen;
    if(symStart < 0)
        std::cout << "Error in ofdm demod" << std::endl;    
    offset = abs(sampleDiff)%ofdmSymLen;    
    if(offset > ofdmSymLen)
        std::cout << "Error in ofdm demod" << std::endl;
    if(offset < 0)
    {
        std::cout << "Error in ofdm demod" << std::endl;        
    }      

    while(this->src->Count() < symStart + numSymbols*ofdmSymLen)
    {
        numSymbols--;
        numSamplesToReturn -= fftSize;
        //symStart -= ofdmSymLen;
    }  
    if(numSymbols == 0 || numSamplesToReturn < 0)
    {
        auto temp = std::make_unique<std::vector<T>>(length,0);        
        return temp;
    }
        
    //Get samples on cut
    auto samples = this->src->GetSamples(symStart, numSymbols*ofdmSymLen);
    if (samples == nullptr)
    	return nullptr;          
           
    auto temp = std::make_unique<std::vector<T>>(numSymbols*fftSize);
    //std::cout << "Calling work" << std::endl;
    Work(samples.get(), temp.get(), (numSymbols-1)*ofdmSymLen, 0);

    //Out of bounds checks
    if(offset>=fftSize)
        offset = 0;//requested start is a CP
    //while(numSamplesToReturn < (numSymbols-1)*fftSize)        
    //    numSamplesToReturn -= fftSize;
    //if(numSamplesToReturn < 0)
    //    std::cout << "Error in ofdm demod" << std::endl;
    auto dest = std::make_unique<std::vector<T>>(numSamplesToReturn);
    //QMutexLocker ml(&mutex);
    //OFDMDemod Work operates on complete OFDM symbols                
    for (size_t i = offset; i < numSamplesToReturn+offset; i++)
    {        
        dest.get()->at(i-offset) = temp.get()->at(i);
    }
    //std::cout << "Done" << std::endl;
    return dest;
}

template <typename T>
float OFDMDemod<T>::GetCarrierOffset(int numOFDMSymbols)
{    
    int numGetSamples = numOFDMSymbols*ofdmSymLen;
    while(numGetSamples+sampleReference>this->src->Count())
        numGetSamples -= ofdmSymLen;   
    auto samples = this->src->GetSamples(sampleReference, numGetSamples);
    vcf samplesWindow(samples.get()->begin(), samples.get()->end());
    vcf transformedData;
    processing::transforms::autoCorrTD(samplesWindow, transformedData, fftSize);
    float freqError = EstimateCarrierOffset(transformedData);
    return freqError;
}

template <typename T>
float OFDMDemod<T>::EstimateCarrierOffset(std::vector<std::complex<float>> input)
{
    int numBlocks = input.size()/ofdmSymLen;
    if(numBlocks == 0) numBlocks = 1;
    vcf integratedBlock(ofdmSymLen);
    int remainingSamples = input.size();
    for (size_t i = 0; i < numBlocks; i++)
    {
        size_t tempLen = min(remainingSamples, ofdmSymLen);
        remainingSamples -= tempLen;
        for (size_t j = 0; j < tempLen; j++)
        {
            integratedBlock[j] += input[i*ofdmSymLen + j];
        }        
    }
    std::vector<float> integratedBlockMag(ofdmSymLen);
    for (size_t i = 0; i < ofdmSymLen; i++)
    {
        float re = integratedBlock[i].real()/static_cast<float>(numBlocks);
        float imag = integratedBlock[i].imag()/static_cast<float>(numBlocks);
        integratedBlockMag[i] = re*re + imag*imag;
    }
    std::vector<float> integratedBlockMagFiltered(ofdmSymLen);
    int filterLen = cpLen;
    if(filterLen < 3)
        filterLen = 3;
    vector<float> lowpassFilter(filterLen);
    lowpassFilter.assign(filterLen, 1.0f/static_cast<float>(filterLen));
    for (size_t i = 0; i < ofdmSymLen; i++)
    {
        for (size_t j = 0; j < filterLen; j++)
        {
            integratedBlockMagFiltered[i] += integratedBlockMag[(i+j)%ofdmSymLen] * lowpassFilter[j];
        }
        integratedBlockMagFiltered[i] = integratedBlockMagFiltered[i]/static_cast<float>(filterLen);
    }
    int maxIndex = 0;
    float maxValue = 0.0f;
    float freqError;
    for (size_t i = 0; i < ofdmSymLen; i++)
    {
        if(integratedBlockMagFiltered[i]>maxValue)
        {
            maxValue = integratedBlockMagFiltered[i];
            maxIndex = i;
        }
    }
    freqError = arg(integratedBlock[maxIndex])/(2.0f*M_PI*static_cast<float>(fftSize));
    std::cout << "Estimated freq offset " << sampleRate*freqError << " at index " << maxIndex << std::endl;
    return freqError;
}

template <typename T>
int OFDMDemod<T>::GetSymbolStart(size_t start)
{
    return 0;    
}



template <typename T>
void OFDMDemod<T>::Work(void* input, void* output, int count, size_t sampleid)
{ 
}

template <>
void OFDMDemod<float>::Work(void* input, void* output, int count, size_t sampleid)
{
    
}

template <>
void OFDMDemod<std::complex<float>>::Work(void* input, void* output, int count, size_t sampleid)
{
    auto in = static_cast<std::vector<std::complex<float>>*>(input);
    auto out = static_cast<std::vector<std::complex<float>>*>(output);
    int numSymbols = count/ofdmSymLen;
    float prevPhase = 0.0f;
    float prevPhasePerSym = 0.0f;
    float carrierFreqErrorAvg = 0.0f;    
    auto samplesIter = in->begin();    
    vcf eqBuffer;
    std::vector<int> pilotSymCount(numPilotCarriers);
    if(pilotCarrierEq)
    {
        eqBuffer.resize(numSymbols*fftSize);
        for (size_t i = 0; i < numPilotCarriers; i++)
        {
            pilotPhaseError[i] = 0.0f;
        }        
    }
    for (size_t i = 0; i < numSymbols; i++)
    {
        vcf symT(samplesIter, samplesIter+ofdmSymLen);
        
        //Shift symbol based on freq offset estimate from constructor
        vcf sym(ofdmSymLen);        
        processing::filter::FrequencyShiftTheta(symT, sym, 2*M_PI*initialFreqOffset, prevPhase);  

        //Shift each symbol based on its CP      
        vcf shifted;
        if(cpBasedShift)
        {
            std::complex<float> cfoAcc {0,0};
            for (size_t j = 0; j < cpLen; j++)
            {
                cfoAcc += sym[j] * conj(sym[fftSize+j]);
            }
            std::complex<float> cfoAvg {cfoAcc.real()/cpLen,cfoAcc.imag()/cpLen};
            float radPerSample = arg(cfoAcc)/static_cast<float>(fftSize);
            float carrierFreqError = radPerSample / (2*M_PI);
            carrierFreqErrorAvg += carrierFreqError;            
            processing::filter::FrequencyShiftTheta(sym, shifted, radPerSample, prevPhasePerSym);            
        } else
            shifted = sym;
        
        //Remove CP and do demod
        vcf td(shifted.begin()+cpLen, shifted.end());
        vcf fd, fftShifted(fftSize);
        processing::transforms::fftCFCF(td, fd, fftSize, plan, fftIn, fftOut);  

        //FFT shift
        if(!pilotCarrierEq) 
        {
            //Normalize
            processing::transforms::Normalize(fd, fftSize);
            for (size_t j = 0; j < fftSize/2; j++)
            {
                out->at(i*fftSize+j) = fd[fftSize/2+j];
                out->at(i*fftSize+fftSize/2+j) = fd[j];
            }
        } else
        {
            processing::transforms::Normalize(fd, fftSize);
            for (size_t j = 0; j < fftSize/2; j++)
            {
                eqBuffer[i*fftSize+j] = fd[fftSize/2+j];
                eqBuffer[i*fftSize+fftSize/2+j] = fd[j];
            }
            //Find all pilot carrier errors
            std::vector<double> X(numPilotCarriers), Y(numPilotCarriers);
            for (size_t j = 0; j < numPilotCarriers; j++)
            {
                //Assume BPSK
                std::complex<float> pilot = eqBuffer[i*fftSize+pilotCarrierIndices[j]];                
                float arg = std::arg(pilot);                               
                if(arg>M_PI/2)
                    arg -= M_PI;
                if(arg<-M_PI/2)
                    arg += M_PI;                    
                
                //Dont use pilots right around -pi/2 or pi/2
                if(arg < 1.5f && arg > -1.5f)
                {
                    pilotPhaseError[j] += -arg;
                    pilotSymCount[j]++;
                }                    

            }                                                
        }    

        //Update iterator        
        samplesIter += ofdmSymLen;
    }        
    //std::cout << "CP freq error " << sampleRate*carrierFreqErrorAvg/static_cast<float>(numSymbols) << std::endl;
    //Block carrier recovery
    if(pilotCarrierEq)
    {
        std::vector<double> X, Y;
        bool refSet = false;
        float ref, diff;
        int slope = 0;
        bool through = false;
        for (size_t i = 0; i < numPilotCarriers; i++)
        {
            pilotPhaseError[i] /= numSymbols;
            if(pilotSymCount[i]>numSymbols/4)
            {
                //unwrap phase trajectory
                if(!refSet)
                {
                    ref = pilotPhaseError[i];
                    refSet = true;
                } else
                {
                    float t = pilotPhaseError[i];
                    if(((ref < 0 && t > 0) || (ref > 0 && t < 0)) && (abs(ref) > M_PI/4 && abs(t) > M_PI/4))
                    {
                        through = true;
                        diff = (M_PI/2-abs(ref))+(M_PI/2-abs(t));
                    } else
                        through = false;
                    if(!through)
                    {
                        slope = (ref < t) ? 1 : -1;
                        ref += slope*abs(ref-t);
                    } else
                    {
                        ref += slope*diff;
                    }
                }
                X.push_back(pilotCarrierIndices[i]);
                Y.push_back(ref);
            }
            
        }
        tk::spline s(X,Y);
        for (size_t i = 0; i < numSymbols; i++)
        {
            for (size_t j = 0; j < fftSize; j++)
            {
                std::complex<float> sampleToBeEq = eqBuffer[i*fftSize + j];
                double phaseCorr = s(j);
                out->at(i*fftSize+j) = sampleToBeEq * std::polar(1.0f, static_cast<float>(phaseCorr));
            }            
        }                
    }
}

template class OFDMDemod<std::complex<float>>;