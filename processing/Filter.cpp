#include "Filter.h"
using namespace std;

namespace processing::filter
{
    /**
    *   Frequecy shifter
    *   
    */
    template <typename T>
    void FrequencyShift(vector<T> input, vector<T> &output, float tshift, float samplingRate)
    {

    }

    void FrequencyShift(vector<float> input, vector<float> &output, float tshift, float samplingRate)
    {
        float shift = (samplingRate == 1) ? tshift : tshift/samplingRate;
        float theta = 0.0f;
        float thetaInc = 2*float(M_PI)*shift;
        output.resize(input.size());
        for (size_t i = 0; i < input.size(); i++)
        {
            output[i] = input[i] * sin(theta);
            theta += thetaInc;
        }        
    }

    void FrequencyShift(vcf input, vcf &output, float tshift, float samplingRate)
    {
        float shift = (samplingRate == 1) ? tshift : tshift/samplingRate;
        float theta = 0.0f;
        float thetaInc = 2*float(M_PI)*shift;
        output.resize(input.size());
        for (size_t i = 0; i < input.size(); i++)
        {
            output[i] = input[i] * polar(1.0f,theta);
            theta += thetaInc;
        }        
    }

    template <typename T>
    void FrequencyShift(vector<T> input, vector<T> &output, float inputFreq, float outputFreq, float samplingRate)
    {
        
    }
    
    void FrequencyShift(vector<float> input, vector<float> &output, float inputFreq, float outputFreq, float samplingRate)
    {
        float tshift = inputFreq-outputFreq;
        float shift = (samplingRate == 1) ? tshift : tshift/samplingRate;
        float theta = 0.;
        float thetaInc = 2*float(M_PI)*shift;
        output.resize(input.size());
        for (size_t i = 0; i < input.size(); i++)
        {
            output[i] = input[i] * sin(theta);
            theta += thetaInc;
        }
    }

    void FrequencyShift(vcf input, vcf &output, float inputFreq, float outputFreq, float samplingRate)
    {
        float tshift = inputFreq-outputFreq;
        float shift = (samplingRate == 1) ? tshift : tshift/samplingRate;
        float theta = 0.;
        float thetaInc = 2*float(M_PI)*shift;
        output.resize(input.size());
        for (size_t i = 0; i < input.size(); i++)
        {
            output[i] = input[i] * polar(1.0f,theta);
            theta += thetaInc;
        }        
    }

    template <typename T>
    void FrequencyShift(vector<T> input, vector<T> &output, float &initPhase, float inputFreq, float outputFreq, float samplingRate)
    {
        
    }

    void FrequencyShift(vector<float> input, vector<float> &output, float &initPhase, float inputFreq, float outputFreq, float samplingRate)
    {
        float tshift = inputFreq-outputFreq;
        float shift = (samplingRate == 1) ? tshift : tshift/samplingRate;
        float theta = initPhase;
        float thetaInc = 2*float(M_PI)*shift;
        output.resize(input.size());
        for (size_t i = 0; i < input.size(); i++)
        {
            output[i] = input[i] * sin(theta);
            theta += thetaInc;
        }     
        initPhase = theta;   
    }

    void FrequencyShift(vcf input, vcf &output, float &initPhase, float inputFreq, float outputFreq, float samplingRate)
    {
        float tshift = inputFreq-outputFreq;
        float shift = (samplingRate == 1) ? tshift : tshift/samplingRate;
        float theta = initPhase;
        float thetaInc = 2*float(M_PI)*shift;
        output.resize(input.size());
        for (size_t i = 0; i < input.size(); i++)
        {
            output[i] = input[i] * polar(1.0f,theta);
            theta += thetaInc;
        }     
        initPhase = theta;   
    }

    template <typename T>
    void FrequencyShiftTheta(vector<T> input, vector<T> &output, float thetaInc, float &initPhase)
    {

    }

    void FrequencyShiftTheta(vector<float> input, vector<float> &output, float thetaInc, float &initPhase)
    {        
        float theta = initPhase;
        //float thetaInc = 2*float(M_PI)*shift;
        output.resize(input.size());
        for (size_t i = 0; i < input.size(); i++)
        {
            output[i] = input[i] * sin(theta);
            theta += thetaInc;
        }     
        initPhase = theta;   
    }

    void FrequencyShiftTheta(vcf input, vcf &output, float thetaInc, float &initPhase)
    {        
        float theta = initPhase;
        //float thetaInc = 2*float(M_PI)*shift;
        output.resize(input.size());
        for (size_t i = 0; i < input.size(); i++)
        {
            output[i] = input[i] * polar(1.0f,theta);
            theta += thetaInc;
        }     
        initPhase = theta;   
    }

    /**
     *   Arbitrary resampler
     */

    void ArbitraryResampler(vector<float> input, vector<float> &output, float resamplingRate, float As)
    {
        msresamp_rrrf q = msresamp_rrrf_create(resamplingRate, As);        
        unsigned int nx = input.size();
        unsigned int ny;
        unsigned int nyAlloc = int(2*nx*resamplingRate);
        //liquid_float_complex *x = (liquid_float_complex*) malloc(nx*sizeof(liquid_float_complex));
        float x[nx];
        float y[nyAlloc];        
        memcpy(&x[0], &input[0], nx*sizeof(float));
        
        msresamp_rrrf_execute(q, x, nx, y, &ny);
        output.resize(ny);
        memcpy(&output[0], &y[0], ny*sizeof(float));
        //free(x);
        //free(y);
        msresamp_rrrf_destroy(q);
    }

    void ArbitraryResampler(vector<complex<float>> input, vector<complex<float>> &output, float resamplingRate, float As)
    {
        msresamp_crcf q = msresamp_crcf_create(resamplingRate, As);        
        unsigned int nx = input.size();
        unsigned int ny;
        unsigned int nyAlloc = int(2*nx*resamplingRate);
        //liquid_float_complex *x = (liquid_float_complex*) malloc(nx*sizeof(liquid_float_complex));
        //liquid_float_complex x[nx];
        liquid_float_complex* x = (liquid_float_complex*) malloc(nx*sizeof(complex<float>));
        liquid_float_complex* y = (liquid_float_complex*) malloc(nyAlloc*sizeof(complex<float>));
        //liquid_float_complex y[nyAlloc];        
        memcpy(&x[0], &input[0], nx*sizeof(complex<float>));
        
        msresamp_crcf_execute(q, x, nx, y, &ny);
        output.resize(ny);
        memcpy(&output[0], &y[0], ny*sizeof(complex<float>));
        free(x);
        free(y);
        msresamp_crcf_destroy(q);
    }

    /*
    FIR filter
    */

    template <typename T>
    void FIRFilt(vector<T> x, vector<T> &y, int  hLen, float cutOff, float As=60.0f)
    {

    }

    void FIRFilt(vector<float> x, vector<float> &y, int  hLen, float cutOff, float As)
    {
        firfilt_rrrf q = firfilt_rrrf_create_kaiser(hLen, cutOff, As, 0.0f);
        //firfilt_rrrf_set_scale(q, 2.0f*cutOff);
        
        y.resize(x.size());
        for (size_t i = 0; i < x.size(); i++)
        {
            firfilt_rrrf_push(q, x[i]);
            firfilt_rrrf_execute(q, &y[i]);
        } 
        firfilt_rrrf_destroy(q);
    }

    void FIRFilt(vector<complex<float>> x, vector<complex<float>> &y, int  hLen, float cutOff, float As)
    {
        firfilt_crcf q = firfilt_crcf_create_kaiser(hLen, cutOff, As, 0.0f);
        //firfilt_rrrf_set_scale(q, 2.0f*cutOff);
        
        y.resize(x.size());
        for (size_t i = 0; i < x.size(); i++)
        {
            firfilt_crcf_push(q, x[i]);
            firfilt_crcf_execute(q, &y[i]);
        } 
        firfilt_crcf_destroy(q);
    }

    /*void FIRFiltRFFF(vector<float> x, vector<float> &y, int hLen, float cutOff, float As=60.0f)
    {
        firfilt_rrrf q = firfilt_rrrf_create_kaiser(hLen, cutOff, As, 0.0f);
        //firfilt_rrrf_set_scale(q, 2.0f*cutOff);
        
        y.resize(x.size());
        for (size_t i = 0; i < x.size(); i++)
        {
            firfilt_rrrf_push(q, x[i]);
            firfilt_rrrf_execute(q, &y[i]);
        }                
        //firfilt_rrrf_execute_block(q, x, x.size(), &y);        
    }*/

    template <typename T>
    void Interpolate(vector<T> &input, vector<T> &output, int interpolationFactor)
    {

    }

    void Interpolate(vector<float> &input, vector<float> &output, int interpolationFactor)
    {
        //Upsample Signal   (output i TRANSFORMATION_POWER times longer)
        unsigned int    m        = 3;       // filter delay (semi length)
        float           As       = 60.0f;   // filter stop-band attenuation
        
        // derived values
        unsigned int sampleWithDelay = input.size() + 2*m;   // total input samples for interpolation with zeroes (w/ delay)
        unsigned int numOutputSamples    = interpolationFactor * sampleWithDelay; // total output samples

        // create interpolator from prototype
        firinterp_rrrf firintrp = firinterp_rrrf_create_kaiser(interpolationFactor,m,As);        

        // pad end of sequence with zeros
        input.resize(sampleWithDelay);

        // interpolate symbols
        output.resize(numOutputSamples);        
        for (int i=0; i<sampleWithDelay; i++)
            firinterp_rrrf_execute(firintrp, input[i], &output[interpolationFactor*i]);

        // destroy interpolator object
        firinterp_rrrf_destroy(firintrp);        
    }

    void Interpolate(vector<complex<float>> &input, vector<complex<float>> &output, int interpolationFactor)
    {
        //Upsample Signal   (output i TRANSFORMATION_POWER times longer)
        unsigned int    m        = 3;       // filter delay (semi length)
        float           As       = 60.0f;   // filter stop-band attenuation
        
        // derived values
        unsigned int sampleWithDelay = input.size() + 2*m;   // total input samples for interpolation with zeroes (w/ delay)
        unsigned int numOutputSamples    = interpolationFactor * sampleWithDelay; // total output samples

        // create interpolator from prototype
        firinterp_crcf firintrp = firinterp_crcf_create_kaiser(interpolationFactor,m,As);        

        // pad end of sequence with zeros        
        input.resize(sampleWithDelay);

        // interpolate symbols
        output.resize(numOutputSamples);
        for (int i=0; i<sampleWithDelay; i++)
            firinterp_crcf_execute(firintrp, input[i], &output[interpolationFactor*i]);

        // destroy interpolator object
        firinterp_crcf_destroy(firintrp);        
    }

    //template <typename T>
    template <class T>
    MovingAverage<T>::MovingAverage(unsigned int winSize)
    {
        buffer.resize(winSize);
        accumulated = 0;
        wSize = winSize;
        index = 0;
    }

    template <class T>
    MovingAverage<T>::~MovingAverage()
    {

    }

    template <class T>
    void MovingAverage<T>::push(T sample)
    {                
        accumulated -= buffer[index];
        accumulated += sample;
        buffer[index] = sample;
        index++;
        if(index == wSize)
            index = 0;
    }
    template <class T>
    T MovingAverage<T>::mean()
    {
        return accumulated/T(wSize);
    }
    template <class T>
    void MovingAverage<T>::reset(unsigned int w)
    {                
        wSize = w;
        buffer.clear();
        buffer.resize(wSize);
        index = 0;
        accumulated = 0;
    }         
    template class MovingAverage<int>;
    template class MovingAverage<float>;   

    int compare(const void* a, const void* b)
    {
        return *(float*) a > *(float*) b ? 1 : -1;
    }

    template <class T>
    Median<T>::Median(unsigned int winSize)
    {
        buffer.resize(winSize);
        sorted.resize(winSize);
        wSize = winSize;
        index = 0;
    }

    template <class T>
    Median<T>::~Median()
    {

    }

    template <class T>
    void Median<T>::push(T sample)
    {                
        buffer[index] = sample;
        index++;
        if(index == wSize)
            index = 0;
    }
    template <class T>
    T Median<T>::median()
    {
        memcpy(&sorted[0], &buffer[0], wSize*sizeof(T));
        qsort(sorted.data(), wSize, sizeof(T), compare);
        return sorted[wSize/2];
    }
    template <class T>
    void Median<T>::reset(unsigned int w)
    {                
        wSize = w;
        buffer.clear();
        buffer.resize(wSize);
        sorted.resize(wSize);
        index = 0;
    } 

    template class Median<int>;
    template class Median<float>;   

}