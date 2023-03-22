#pragma once

#include "SampleProc.h"
#include <iostream>
#include <complex>
 
#include <fftw3.h> //provides fft plan
#include "Filter.h"
#include "Transformations.h"
#include "Structs.h"
#include "Utils.h"
#include "spline.h"

using namespace processing::utils;
template<typename T>
class OFDMDemod : public SampleProc<T, T>
{
public:
    OFDMDemod(std::shared_ptr<SampleSource<T>> src, OFDMDemodVars vars);    
    ~OFDMDemod();  
    std::unique_ptr<std::vector<T>> GetSamples(size_t start, size_t length);
    void Reconfigure(OFDMDemodVars vars);
    void Work(void *input, void *output, int count, size_t sampleid);
private:
    int GetSymbolStart(size_t start);
    float GetCarrierOffset(int numOFDMSymbols);
    float EstimateCarrierOffset(std::vector<std::complex<float>> input);
    void InitProc();

    int fftSize;
    int cpLen;
    int ofdmSymLen;
    int sampleReference;
    int ofdmSymStartRef;
    bool cpBasedShift;
    bool initFreqShift;
    float initialFreqOffset = 0.0f; //radPerSample
    int numSymsToEstimateOffset;
    float sampleRate;
    int numPilotCarriers;
    std::vector<int> pilotCarrierIndices;
    bool pilotCarrierEq;
    std::vector<float> pilotPhaseError;
    std::vector<DataPoint> interPoints;
    
    fftwf_plan plan; //fftw plans
    fftwf_complex* fftIn;// = (fftwf_complex*) fftwf_malloc(16384 * sizeof(fftwf_complex)); //room for 16384*8
    fftwf_complex* fftOut;// = (fftwf_complex*) fftwf_malloc(16384 * sizeof(fftwf_complex));//room for 16384*8
};
