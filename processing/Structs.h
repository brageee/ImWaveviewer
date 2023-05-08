
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <complex>
#include <vector>
#include <iostream>
#include <fstream>
#include "DataTypes.h"

struct FileInfo {
    std::string filename;
    std::string detectedFileType = "";
    int realOrComplex = 0;
    int sampleType = 2;
    int endianness = 0;
    bool supportedFormat;
    DataType datatype;
    double sampleRate = 1000.0;
    double frequency = 0.0;
    double timestampStart = 0.0;
};

struct Settings {
    std::string defaultPath = "";
    bool updatePath = true;
};

enum DerivationType {
        INPUT,
        REAL,
        IMAG,
        AM,
        FM,
        PHASE,
        FREQSHIFT,
        RESAMPLE,
        FILTER,
        DDC,
        OFDMDEMOD
};


struct DerivedPlot {    
    DataType datatype;
    std::vector<float> dataFloat;
    std::vector<std::complex<float>> dataComplexFloat;
    std::vector<float> dataReal;
    std::vector<float> dataImag;
    int numSamples = 0;
    std::vector<int> indices;
    std::string name;
    double sampleRate;
    double resamplingRatio = 1.0f;
    bool hasChildren = false;
    //std::ifstream fileHandler;
    //unique_ptr<DerivedPlot> parent = nullptr;
    //vector<unique_ptr<DerivedPlot>> children;
};

struct Markers {
    bool displayXMarkers = false;
    bool displayYMarkers = false;
    bool xPeriodic = false;
    bool yPeriodic = false;
    float xPeriod = 100;
    float yPeriod = 100;
    double xPeriodicReference = 0.0;
    double yPeriodicReference = 0.0;
    int XmarkerTD0ID = 1000;
    int XmarkerTD1ID = 1001;
    int YmarkerTD0ID = 1002;
    int YmarkerTD1ID = 1003;
    double XmarkerTD0 = 0;
    double XmarkerTD1 = 1;
    double YmarkerTD0 = 0;
    double YmarkerTD1 = 1;
    int controlTrace = 0;   //which TD trace to control with y markers and derive trace
    double yMarkerRangeMin = 0;
    double yMarkerRangeMax = 0;
};

struct FrequencyShiftStruct{
    double shift;    
};

struct FilterVars{
    double centerFreq = 0.0f;
    double cutoff = 100.0f;
    double attenuation = 60;
    std::vector<float> realTaps;
    char realTapsChar[10000];
    std::vector<std::complex<float>> complexTaps;
    char complexTapsChar[10000];
    bool flipTapsReal = false;
    bool flipTapsImag = false;
    int filterType = 0; //0 = taps are derived based on cf, cutoff, att, 1 = real taps provided, 2 = complex taps provided
};

struct OFDMDemodVars{
    int fftSize = 64;
    int cpLen = 16;
    int ofdmSymLen;
    int sampleReference;
    int symStart;    
    bool cpBasedShift = false;
    bool initFreqShift = true;
    std::vector<int> pilotCarrierIndices;   
    char pilotsChar[1000] ;
    bool pilotCarrierEq = false;
    int numSymsToEstimateOffset = 100;
};