#include <vector>
#include <complex>
#include <fftw3.h>
#include <math.h>
#include <iostream>

#include "Filter.h"
#include "Windowing.h"

#pragma once

using namespace processing::windowing;
using namespace std;

namespace processing::transforms
{
    /**
     * Performs complex 1d fft on complex float input and returns std::vector<float> of 
     * power spectrum 
    */
    void fftCFCF(std::vector<std::complex<float>> &input, std::vector<std::complex<float>> &output, unsigned int fftSize);

    void fftCFCF(std::vector<std::complex<float>> &input, std::vector<std::complex<float>> &output, unsigned int fftSize, fftwf_plan plan, fftwf_complex* fftIn, fftwf_complex* fftOut);
    /**
     * Performs complex 1d fft on complex float input and returns std::vector<float> of 
     * power spectrum 
    */
    void ifftCFCF(std::vector<std::complex<float>> &input, std::vector<std::complex<float>> &output, unsigned int fftSize);

    void ifftCFCF(std::vector<std::complex<float>> &input, std::vector<std::complex<float>> &output, unsigned int fftSize, fftwf_plan plan, fftwf_complex* fftIn, fftwf_complex* fftOut);

    /**
     * Performs MATLAB type fftshift of input vector
     */
    template <typename T1, typename T2>
    void FFTShift(T1 &input, T1 &output);

    /**
     * Normalize signal/vector power to 1
     */
    //template <typename T1, typename T2>    
    void Normalize(std::vector<float> &input);
    void Normalize(std::vector<float> &input, float power);
    void Normalize(std::vector<std::complex<float>> &input);
    void Normalize(std::vector<std::complex<float>> &input, float power);

    /**
     * Performs complex 1d fft on complex float input and returns std::vector<float> of 
     * power spectrum 
    */
    void PowerSpectrumCFF(std::vector<std::complex<float>> &input, std::vector<float> &fftMagSq, unsigned int fftSize);
    
    void PowerSpectrumLogCFF(std::vector<std::complex<float>> &input, std::vector<float> &fftMagSq, unsigned int fftSize, int power=1);

    void PowerSpectrumWindowLog(std::vector<float> &input, std::vector<float> &output, unsigned int windowLen, unsigned int fftSize, int upsample, int power, WindowType window);
    
    void PowerSpectrumWindowLog(std::vector<std::complex<float>> &input, std::vector<float> &output, unsigned int windowLen, unsigned int fftSize, int upsample, int power, WindowType window);
    
    void PowerSpectrumWindowLog(std::vector<float> &input, std::vector<float> &output, unsigned int windowLen, unsigned int fftSize, int upsample, int power, WindowType window, fftwf_plan plan, fftwf_complex* fftIn, fftwf_complex* fftOut);
    
    void PowerSpectrumWindowLog(std::vector<std::complex<float>> &input, std::vector<float> &output, unsigned int windowLen, unsigned int fftSize, int upsample, int power, WindowType window, fftwf_plan plan, fftwf_complex* fftIn, fftwf_complex* fftOut);

    void PowerSpectrumWindowLog(std::vector<float> &input, std::vector<float> &output, unsigned int windowLen, unsigned int fftSize, int upsample, int power, WindowType window, fftwf_plan plan, fftwf_complex* fftIn, fftwf_complex* fftOut, std::vector<float> &windowTaps);

    void PowerSpectrumWindowLog(std::vector<std::complex<float>> &input, std::vector<float> &output, unsigned int windowLen, unsigned int fftSize, int upsample, int power, WindowType window, fftwf_plan plan, fftwf_complex* fftIn, fftwf_complex* fftOut, std::vector<float> &windowTaps);    

    void SpectrogramFFT(std::vector<float> &input, std::vector<float> &output, unsigned int windowLen, unsigned int fftSize, WindowType window, fftwf_plan plan, fftwf_complex* fftIn, fftwf_complex* fftOut, std::vector<float> &windowTaps);

    void SpectrogramFFT(std::vector<std::complex<float>> &input, std::vector<float> &output, unsigned int windowLen, unsigned int fftSize, WindowType window, fftwf_plan plan, fftwf_complex* fftIn, fftwf_complex* fftOut, std::vector<float> &windowTaps);

    /**
     * Performs autocorrelation through FFT and real vector
    */

    void autoCorrFFT(std::vector<float> input, std::vector<float> &output, unsigned int fftSize);

    void autoCorrFFT(std::vector<std::complex<float>> input, std::vector<float> &output, unsigned int fftSize);

    void autoCorrTD(std::vector<float> input, std::vector<float> &output, int delay);

    void autoCorrTD(std::vector<std::complex<float>> input, std::vector<std::complex<float>> &output, int delay);

    void Integrate(std::vector<float> input, std::vector<float> &output, int fftSize, int delay);

    void Integrate(std::vector<std::complex<float>> input, std::vector<float> &output, int fftSize, int delay);


    void RealDetect(std::vector<std::complex<float>> input, std::vector<float> &output);

    /**
     * Performs AM detect on complex float input
     * 
    */

    void AMDetect(std::vector<float> input, std::vector<float> &output);

    void AMDetect(std::vector<std::complex<float>> input, std::vector<float> &output);

    /**
     * Performs AM detect on complex float input
     * 
    */
    void AMDetectLog(std::vector<float> input, std::vector<float> &output);

    void AMDetectLog(std::vector<std::complex<float>> input, std::vector<float> &output);

    /**
     * Performs FM detect on complex float input
     * 
    */ 

    void FMDetect(std::vector<float> input, std::vector<float> &output, float scale=1);

    void FMDetect(std::vector<std::complex<float>> &input, std::vector<float> &output, float scale=1);            

    /**
     * Performs phase detect on complex float input
     * 
    */    

    void PhaseDetect(std::vector<float> input, std::vector<float> &output, bool deg=false);

    void PhaseDetect(std::vector<std::complex<float>> input, std::vector<float> &output, bool deg=false);
        
}