#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <cmath>
#include <algorithm>
#include <complex>
#include <vector>
#include <math.h>
#include <iostream>
#include <string.h>

#include <liquid/liquid.h>

using namespace std;
typedef vector<complex<float>> vcf;

namespace processing::filter
{
    /**
    *   Frequecy shifter
    *   
    */

    void FrequencyShift(vector<float> input, vector<float> &output, float tshift, float samplingRate);

    void FrequencyShift(vcf input, vcf &output, float tshift, float samplingRate);
    
    void FrequencyShift(vector<float> input, vector<float> &output, float inputFreq, float outputFreq, float samplingRate);

    void FrequencyShift(vcf input, vcf &output, float inputFreq, float outputFreq, float samplingRate);

    void FrequencyShift(vector<float> input, vector<float> &output, float &initPhase, float inputFreq, float outputFreq, float samplingRate);

    void FrequencyShift(vcf input, vcf &output, float &initPhase, float inputFreq, float outputFreq, float samplingRate);

    void FrequencyShiftTheta(vector<float> input, vector<float> &output, float thetaInc, float &initPhase);

    void FrequencyShiftTheta(vcf input, vcf &output, float thetaInc, float &initPhase);

    /**
     *   Arbitrary resampler
     */

    void ArbitraryResampler(vector<float> input, vector<float> &output, float resamplingRate, float As=60.0f);
    
    void ArbitraryResampler(vector<complex<float>> input, vector<complex<float>> &output, float resamplingRate, float As=60.0f);

    /*
    FIR filter
    */

    void FIRFilt(vector<float> x, vector<float> &y, int  hLen, float cutOff, float As=60.0f);

    void FIRFilt(vector<complex<float>> x, vector<complex<float>> &y, int  hLen, float cutOff, float As=60.0f);

    void Interpolate(vector<float> &input, vector<float> &output, int interpolationFactor);

    void Interpolate(vector<complex<float>> &input, vector<complex<float>> &output, int interpolationFactor);

    template <class T>
    class MovingAverage
    {
        public:
            MovingAverage(unsigned int winSize);
            ~MovingAverage();
            void push(T sample);
            T mean();
            void reset(unsigned int w);            
        private:
            std::vector<T> buffer; //buffer to store sample values;
            T accumulated; //the accumulated value of all samples in buffer
            unsigned int wSize; //window size
            unsigned int index; //index in buffer. Creates a circular buffer
    };

    int compare(const void* a, const void* b);

    template <class T>
    class Median
    {
        public:
            Median(unsigned int winSize);
            ~Median();
            void push(T sample);
            T median();
            void reset(unsigned int w);            
        private:
            //int compare(const void* a, const void* b);
            std::vector<T> buffer; //buffer to store sample values;
            std::vector<T> sorted; //buffer sorted
            unsigned int wSize; //window size
            unsigned int index; //index in buffer. Creates a circular buffer
    };

}