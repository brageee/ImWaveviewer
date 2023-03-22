#include "Transformations.h"

using namespace processing::windowing;
using namespace std;

namespace processing::transforms
{
    /**
     * Performs complex 1d fft on complex float input and returns std::vector<float> of 
     * power spectrum 
    */
    void fftCFCF(std::vector<std::complex<float>> &input, std::vector<std::complex<float>> &output, unsigned int fftSize)
    {
        //Setup fftw3 plan
        fftwf_plan plan;
        unsigned int numSamples = input.size();        
        fftwf_complex* fftIn = (fftwf_complex*) fftwf_malloc(fftSize * sizeof(fftwf_complex));
        fftwf_complex* fftOut = (fftwf_complex*) fftwf_malloc(fftSize * sizeof(fftwf_complex));        
        plan = fftwf_plan_dft_1d(fftSize, fftIn, fftOut, FFTW_FORWARD, FFTW_ESTIMATE);
        //Resize output             
        output.resize(fftSize);       
        //Copy input to fftIn        
        for (size_t i = 0; i < fftSize; i++)
        {
            /* code */
            fftIn[i][0] = input[i].real();
            fftIn[i][1] = input[i].imag();
        }
        //Execute fft
        fftwf_execute(plan);
        //Copy to output
        for (size_t i = 0; i < fftSize; i++)
        {                        
            output[i] = std::complex<float>(fftOut[i][0],fftOut[i][1]);        
        }
        fftwf_free(fftIn);
        fftwf_free(fftOut);
        fftwf_destroy_plan(plan);
        fftwf_cleanup();
    }

    void fftCFCF(std::vector<std::complex<float>> &input, std::vector<std::complex<float>> &output, unsigned int fftSize, fftwf_plan plan, fftwf_complex* fftIn, fftwf_complex* fftOut)
    {        
        //Resize output             
        output.resize(fftSize);       
        //Copy input to fftIn        
        for (size_t i = 0; i < fftSize; i++)
        {
            /* code */
            fftIn[i][0] = input[i].real();
            fftIn[i][1] = input[i].imag();
        }
        //Execute fft
        fftwf_execute(plan);
        //Copy to output
        for (size_t i = 0; i < fftSize; i++)
        {                        
            output[i] = std::complex<float>(fftOut[i][0],fftOut[i][1]);        
        }
        
    }

    /**
     * Performs complex 1d fft on complex float input and returns std::vector<float> of 
     * power spectrum 
    */
    void ifftCFCF(std::vector<std::complex<float>> &input, std::vector<std::complex<float>> &output, unsigned int fftSize)
    {
        //Setup fftw3 plan
        fftwf_plan plan;
        unsigned int numSamples = input.size();
        fftwf_complex* fftIn = (fftwf_complex*) fftwf_malloc(fftSize * sizeof(fftwf_complex));
        fftwf_complex* fftOut = (fftwf_complex*) fftwf_malloc(fftSize * sizeof(fftwf_complex));        
        plan = fftwf_plan_dft_1d(fftSize, fftIn, fftOut, FFTW_BACKWARD, FFTW_ESTIMATE);
        //Resize output             
        output.resize(numSamples);       
        //Copy input to fftIn        
        for (size_t i = 0; i < numSamples; i++)
        {
            /* code */
            fftIn[i][0] = input[i].real();
            fftIn[i][1] = input[i].imag();
        }
        //Execute fft
        fftwf_execute(plan);
        //Copy to output
        for (size_t i = 0; i < fftSize; i++)
        {                     
            //std::cout << "i " << i << std::endl;
            output[i] = std::complex<float>(fftOut[i][0]/fftSize,fftOut[i][1]/fftSize);     
        }
        fftwf_free(fftIn);
        fftwf_free(fftOut);
        fftwf_destroy_plan(plan);
        fftwf_cleanup();        
    }

    void ifftCFCF(std::vector<std::complex<float>> &input, std::vector<std::complex<float>> &output, unsigned int fftSize, fftwf_plan plan, fftwf_complex* fftIn, fftwf_complex* fftOut)
    {
        unsigned int numSamples = input.size();
        //Resize output             
        output.resize(numSamples);       
        //Copy input to fftIn        
        for (size_t i = 0; i < numSamples; i++)
        {
            /* code */
            fftIn[i][0] = input[i].real();
            fftIn[i][1] = input[i].imag();
        }
        //Execute fft
        fftwf_execute(plan);
        //Copy to output
        for (size_t i = 0; i < fftSize; i++)
        {                     
            //std::cout << "i " << i << std::endl;
            output[i] = std::complex<float>(fftOut[i][0]/fftSize,fftOut[i][1]/fftSize);     
        }
              
    }

    /**
     * Performs MATLAB type fftshift of input vector
     */
    template <typename T1, typename T2>
    void FFTShift(T1 &input, T1 &output)
    {
        int size = input.size();
        output.resize(size);
        int halfSize = size/2;
        memcpy(&output[0], &input[halfSize], (size-halfSize)*sizeof(T2));
        memcpy(&output[(size-halfSize)], &input[0], halfSize*sizeof(T2));
    }

    /**
     * Normalize signal/vector power to 1
     */
    /*template <typename T1, typename T2>
    void Normalize(T1 &input)
    {
        int size = input.size();
        float sumPower = 0.0f;
        for (size_t i = 0; i < size; i++)
        {
            sumPower += input[i].real()*input[i].real()+input[i].imag()*input[i].imag();
        }
        sumPower = 1.0f*sumPower/size;
        for (size_t i = 0; i < size; i++)
        {
            input[i] = input[i]/sqrt(sumPower*size);
        }        
    }*/
    void Normalize(std::vector<float> &input)
    {
        int size = input.size();
        float sumPower = 0.0f;
        for (size_t i = 0; i < size; i++)
        {
            sumPower += input[i]*input[i];
        }
        sumPower = 1.0f*sumPower/size;
        for (size_t i = 0; i < size; i++)
        {
            input[i] = input[i]/sqrt(sumPower*size);
        }        
    }

    void Normalize(std::vector<float> &input, float power)
    {
        int size = input.size();
        float sumPower = 0.0f;
        for (size_t i = 0; i < size; i++)
        {
            sumPower += input[i]*input[i];
        }
        sumPower = sumPower/(static_cast<float>(size)*power);
        for (size_t i = 0; i < size; i++)
        {
            input[i] = input[i]/sqrt(sumPower*size);
        }        
    }

    void Normalize(std::vector<std::complex<float>> &input)
    {
        int size = input.size();
        float sumPower = 0.0f;
        for (size_t i = 0; i < size; i++)
        {
            sumPower += input[i].real()*input[i].real()+input[i].imag()*input[i].imag();
        }
        sumPower = 1.0f*sumPower/size;
        for (size_t i = 0; i < size; i++)
        {
            input[i] = input[i]/sqrt(sumPower*size);
        }        
    }

    void Normalize(std::vector<std::complex<float>> &input, float power)
    {
        int size = input.size();
        float sumPower = 0.0f;
        for (size_t i = 0; i < size; i++)
        {
            sumPower += input[i].real()*input[i].real()+input[i].imag()*input[i].imag();
        }
        sumPower = sumPower/(static_cast<float>(size)*power);
        for (size_t i = 0; i < size; i++)
        {
            input[i] = input[i]/sqrt(sumPower*size);
        }        
    }

    /**
     * Performs complex 1d fft on complex float input and returns std::vector<float> of 
     * power spectrum 
    */
    void PowerSpectrumCFF(std::vector<std::complex<float>> &input, std::vector<float> &fftMagSq, unsigned int fftSize)
    {
        //Setup fftw3 plan
        fftwf_plan plan;
        unsigned int numSamples = input.size();
        fftwf_complex* fftIn = (fftwf_complex*) fftwf_malloc(numSamples * sizeof(fftwf_complex));
        fftwf_complex* fftOut = (fftwf_complex*) fftwf_malloc(fftSize * sizeof(fftwf_complex));        
        plan = fftwf_plan_dft_1d(fftSize, fftIn, fftOut, FFTW_FORWARD, FFTW_ESTIMATE);
        //Resize output             
        std::vector<float> fftMagSqT;
        fftMagSqT.resize(fftSize);   
        //Copy input to fftIn        
        for (size_t i = 0; i < numSamples; i++)
        {
            /* code */
            fftIn[i][0] = input[i].real();
            fftIn[i][1] = input[i].imag();
        }
        //Execute fft
        fftwf_execute(plan);
        //Copy to output
        for (size_t i = 0; i < fftSize; i++)
        {                        
            fftMagSqT[i] = fftOut[i][0]*fftOut[i][0]/fftSize+fftOut[i][1]*fftOut[i][1]/fftSize;        
        }
        FFTShift<std::vector<float>, float>(fftMagSqT, fftMagSq);
        fftwf_free(fftIn);
        fftwf_free(fftOut);
        fftwf_destroy_plan(plan);
        fftwf_cleanup();
    }    

    void PowerSpectrumLogCFF(std::vector<std::complex<float>> &input, std::vector<float> &fftMagSq, unsigned int fftSize, int power)
    {
        //Setup fftw3 plan
        fftwf_plan plan;
        unsigned int numSamples = input.size();
        fftwf_complex* fftIn = (fftwf_complex*) fftwf_malloc(fftSize * power * sizeof(fftwf_complex));
        fftwf_complex* fftOut = (fftwf_complex*) fftwf_malloc(fftSize * power * sizeof(fftwf_complex));        
        plan = fftwf_plan_dft_1d(fftSize * power, fftIn, fftOut, FFTW_FORWARD, FFTW_ESTIMATE);
        
        vector<complex<float>> interpolated;
        if(power>1)
            processing::filter::Interpolate(input, interpolated, power);
        else
            interpolated = input;

        //Complex multiply upsamples array with itself "power" times
        int powerToGo = power;
        
        while(powerToGo > 1)
        {
            float realSum = 0.0;

            for(int i = 0; i < interpolated.size(); i++)
            {
                realSum += fabs(interpolated[i].real());
            }

            float realAvg = realSum / interpolated.size();
            float scalingFactor = 1.0 / realAvg;

            for(int i = 0; i < interpolated.size(); i++)
            {
                interpolated[i] = interpolated[i] * scalingFactor;
                interpolated[i] = interpolated[i] * interpolated[i];
            }
            powerToGo = (int)(powerToGo/2);
        }

        //Copy input to fftIn        
        for (size_t i = 0; i < fftSize * power; i++)
        {
            /* code */
            fftIn[i][0] = interpolated[i].real();
            fftIn[i][1] = interpolated[i].imag();
        }

        //Execute fft
        fftwf_execute(plan);
        //Copy to output
        //Resize output             
        std::vector<float> fftMagSqT;
        fftMagSqT.resize(fftSize * power);  
        for (size_t i = 0; i < fftSize * power; i++)
        {                        
            fftMagSqT[i] = 10*log10(fftOut[i][0]*fftOut[i][0]+fftOut[i][1]*fftOut[i][1]);        
        }
        fftMagSq = fftMagSqT;
        FFTShift<std::vector<float>, float>(fftMagSqT, fftMagSq);

        fftwf_free(fftIn);
        fftwf_free(fftOut);
        fftwf_destroy_plan(plan);
        fftwf_cleanup();
    }

    void PowerSpectrumWindowLog(std::vector<float> &input, std::vector<float> &output, unsigned int windowLen, unsigned int fftSize, int upsample, int power, WindowType window)
    {
         //Setup fftw3 plan
        fftwf_plan plan;
        unsigned int numSamples = input.size();
        fftwf_complex* fftIn = (fftwf_complex*) fftwf_malloc(fftSize * upsample * sizeof(fftwf_complex));
        fftwf_complex* fftOut = (fftwf_complex*) fftwf_malloc(fftSize * upsample * sizeof(fftwf_complex));        
        plan = fftwf_plan_dft_1d(fftSize * upsample, fftIn, fftOut, FFTW_FORWARD, FFTW_ESTIMATE);

        //Upsample if selected
        std::vector<float> interpolated;
        if(upsample>1)
            processing::filter::Interpolate(input, interpolated, upsample);
        else
            interpolated = input;

        //Raise to power
        int powerToGo = power;        
        while(powerToGo > 1)
        {            
            for(int i = 0; i < interpolated.size(); i++)
            {                
                interpolated[i] = interpolated[i] * interpolated[i];
            }
            powerToGo = (int)(powerToGo/2);
        }

        //Normalize
        float scalingFactor = 1.0f / static_cast<float>(upsample);
        if(upsample > 1 && power > 1)
            scalingFactor = 1.0f / pow(upsample, power);
        for (size_t i = 0; i < interpolated.size(); i++)
        {
            interpolated[i] = interpolated[i] * scalingFactor;
        }
        

        //Setup windowing taps        
        std::vector<float> windowTaps(windowLen * upsample);
        SetupWindow(window, windowLen * upsample, windowTaps);        

        //Allocate FFT mag sq vector
        std::vector<float> fftMagSqT;
        fftMagSqT.resize(fftSize * upsample);  

        int offset =  windowLen * upsample;
        int numFFTs = interpolated.size()/offset;
        if(numFFTs == 0) numFFTs = 1;
        int remainingSamples = interpolated.size();
        for (int j = 0; j < numFFTs; j++)
        {
            //Apply window
            std::vector<float> windowOutput(fftSize * upsample);
            int localWindowLen = std::min(remainingSamples, static_cast<int>(offset));
            remainingSamples -= localWindowLen;
            for (int i = 0; i < localWindowLen; i++)
            {                
                windowOutput[i] = interpolated[j*offset+i]*windowTaps[i];                
            }
            for (int i = localWindowLen; i < fftSize * upsample; i++)
            {
                windowOutput[i] = 0.0f;                
            }            
            
            //Copy to fft input
            for (size_t i = 0; i < fftSize * upsample; i++)
            {
                fftIn[i][0] = windowOutput[i];
                fftIn[i][1] = 0;
            }
            
            //Execute fft
            fftwf_execute(plan);
            
            //Copy to output            
            for (int i = 0; i < fftSize * upsample; i++)
            {                        
                fftMagSqT[i] += (fftOut[i][0]*fftOut[i][0]+fftOut[i][1]*fftOut[i][1]);        
            }
        }
        for (int i = 0; i < fftSize * upsample; i++)
        {            
            float temp  =  10*log10(fftMagSqT[i]/numFFTs); 
            fftMagSqT[i] = temp;
        }               

        FFTShift<std::vector<float>, float>(fftMagSqT, output);

        fftwf_free(fftIn);
        fftwf_free(fftOut);
        fftwf_destroy_plan(plan);
        fftwf_cleanup();

    }

    void PowerSpectrumWindowLog(std::vector<std::complex<float>> &input, std::vector<float> &output, unsigned int windowLen, unsigned int fftSize, int upsample, int power, WindowType window)
    {
         //Setup fftw3 plan
        fftwf_plan plan;
        unsigned int numSamples = input.size();
        fftwf_complex* fftIn = (fftwf_complex*) fftwf_malloc(fftSize * upsample * sizeof(fftwf_complex));
        fftwf_complex* fftOut = (fftwf_complex*) fftwf_malloc(fftSize * upsample * sizeof(fftwf_complex));        
        plan = fftwf_plan_dft_1d(fftSize * upsample, fftIn, fftOut, FFTW_FORWARD, FFTW_ESTIMATE);

        //Upsample if selected
        vcf interpolated;
        if(upsample>1)
            processing::filter::Interpolate(input, interpolated, upsample);
        else
            interpolated = input;

        //Raise to power
        int powerToGo = power;        
        while(powerToGo > 1)
        {            
            for(int i = 0; i < interpolated.size(); i++)
            {                
                interpolated[i] = interpolated[i] * interpolated[i];
            }
            powerToGo = (int)(powerToGo/2);
        }

        //Normalize
        float scalingFactor = 1.0f / static_cast<float>(upsample);
        if(upsample > 1 && power > 1)
            scalingFactor = 1.0f / pow(upsample, power);
        for (size_t i = 0; i < interpolated.size(); i++)
        {
            interpolated[i] = interpolated[i] * scalingFactor;
        }
        
        //Setup windowing taps        
        std::vector<float> windowTaps(windowLen * upsample);
        SetupWindow(window, windowLen * upsample, windowTaps);        

        //Allocate FFT mag sq vector
        std::vector<float> fftMagSqT;
        fftMagSqT.resize(fftSize * upsample);  

        int offset =  windowLen * upsample;
        int numFFTs = interpolated.size()/offset;
        if(numFFTs == 0) numFFTs = 1;
        int remainingSamples = interpolated.size();
        for (int j = 0; j < numFFTs; j++)
        {
            //Apply window
            vcf windowOutput(fftSize * upsample);
            int localWindowLen = std::min(remainingSamples, static_cast<int>(offset));
            remainingSamples -= localWindowLen;
            for (int i = 0; i < localWindowLen; i++)
            {                
                windowOutput[i] = interpolated[j*offset+i]*windowTaps[i];                
            }
            for (int i = localWindowLen; i < fftSize * upsample; i++)
            {
                windowOutput[i] = std::complex<float>(0.0, 0.0);                
            }            
            
            //Copy to fft input
            for (size_t i = 0; i < fftSize * upsample; i++)
            {
                fftIn[i][0] = windowOutput[i].real();
                fftIn[i][1] = windowOutput[i].imag();
            }
            
            //Execute fft
            fftwf_execute(plan);
            
            //Copy to output            
            for (int i = 0; i < fftSize * upsample; i++)
            {                        
                fftMagSqT[i] += (fftOut[i][0]*fftOut[i][0]+fftOut[i][1]*fftOut[i][1]);        
            }
        }
        for (int i = 0; i < fftSize * upsample; i++)
        {            
            float temp  =  10*log10(fftMagSqT[i]/numFFTs); 
            fftMagSqT[i] = temp;
        }                

        FFTShift<std::vector<float>, float>(fftMagSqT, output);

        fftwf_free(fftIn);
        fftwf_free(fftOut);
        fftwf_destroy_plan(plan);
        fftwf_cleanup();

    }

    void PowerSpectrumWindowLog(std::vector<float> &input, std::vector<float> &output, unsigned int windowLen, unsigned int fftSize, int upsample, int power, WindowType window, fftwf_plan plan, fftwf_complex* fftIn, fftwf_complex* fftOut)
    {
        //Upsample if selected
        std::vector<float> interpolated;
        if(upsample>1)
            processing::filter::Interpolate(input, interpolated, upsample);
        else
            interpolated = input;

        //Raise to power
        int powerToGo = power;        
        while(powerToGo > 1)
        {            
            for(int i = 0; i < interpolated.size(); i++)
            {                
                interpolated[i] = interpolated[i] * interpolated[i];
            }
            powerToGo = (int)(powerToGo/2);
        }

        //Normalize
        float scalingFactor = 1.0f / static_cast<float>(upsample);
        if(upsample > 1 && power > 1)
            scalingFactor = 1.0f / pow(upsample, power);
        for (size_t i = 0; i < interpolated.size(); i++)
        {
            interpolated[i] = interpolated[i] * scalingFactor;
        }
        

        //Setup windowing taps        
        std::vector<float> windowTaps(windowLen * upsample);
        SetupWindow(window, windowLen * upsample, windowTaps);        

        //Allocate FFT mag sq vector
        std::vector<float> fftMagSqT;
        fftMagSqT.resize(fftSize * upsample);  

        int offset =  windowLen * upsample;
        int numFFTs = interpolated.size()/offset;
        if(numFFTs == 0) numFFTs = 1;
        int remainingSamples = interpolated.size();
        for (int j = 0; j < numFFTs; j++)
        {
            //Apply window
            std::vector<float> windowOutput(fftSize * upsample);
            int localWindowLen = std::min(remainingSamples, static_cast<int>(offset));
            remainingSamples -= localWindowLen;
            for (int i = 0; i < localWindowLen; i++)
            {                
                windowOutput[i] = interpolated[j*offset+i]*windowTaps[i];                
            }
            for (int i = localWindowLen; i < fftSize * upsample; i++)
            {
                windowOutput[i] = 0.0f;                
            }            
            
            //Copy to fft input
            for (size_t i = 0; i < fftSize * upsample; i++)
            {
                fftIn[i][0] = windowOutput[i];
                fftIn[i][1] = 0;
            }
            
            //Execute fft
            fftwf_execute(plan);
            
            //Copy to output            
            for (int i = 0; i < fftSize * upsample; i++)
            {                        
                fftMagSqT[i] += (fftOut[i][0]*fftOut[i][0]+fftOut[i][1]*fftOut[i][1]);        
            }
        }
        for (int i = 0; i < fftSize * upsample; i++)
        {            
            float temp  =  10*log10(fftMagSqT[i]/numFFTs); 
            fftMagSqT[i] = temp;
        }               

        FFTShift<std::vector<float>, float>(fftMagSqT, output);
        
    }

    void PowerSpectrumWindowLog(std::vector<std::complex<float>> &input, std::vector<float> &output, unsigned int windowLen, unsigned int fftSize, int upsample, int power, WindowType window, fftwf_plan plan, fftwf_complex* fftIn, fftwf_complex* fftOut)
    {
        //Upsample if selected
        vcf interpolated;
        if(upsample>1)
            processing::filter::Interpolate(input, interpolated, upsample);
        else
            interpolated = input;

        //Raise to power
        int powerToGo = power;        
        while(powerToGo > 1)
        {            
            for(int i = 0; i < interpolated.size(); i++)
            {                
                interpolated[i] = interpolated[i] * interpolated[i];
            }
            powerToGo = (int)(powerToGo/2);
        }

        //Normalize
        float scalingFactor = 1.0f / static_cast<float>(upsample);
        if(upsample > 1 && power > 1)
            scalingFactor = 1.0f / pow(upsample, power);
        for (size_t i = 0; i < interpolated.size(); i++)
        {
            interpolated[i] = interpolated[i] * scalingFactor;
        }
        
        //Setup windowing taps        
        std::vector<float> windowTaps(windowLen * upsample);
        SetupWindow(window, windowLen * upsample, windowTaps);        

        //Allocate FFT mag sq vector
        std::vector<float> fftMagSqT;
        fftMagSqT.resize(fftSize * upsample);  

        int offset =  windowLen * upsample;
        int numFFTs = interpolated.size()/offset;
        if(numFFTs == 0) numFFTs = 1;
        int remainingSamples = interpolated.size();
        for (int j = 0; j < numFFTs; j++)
        {
            //Apply window
            vcf windowOutput(fftSize * upsample);
            int localWindowLen = std::min(remainingSamples, static_cast<int>(offset));
            remainingSamples -= localWindowLen;
            for (int i = 0; i < localWindowLen; i++)
            {                
                windowOutput[i] = interpolated[j*offset+i]*windowTaps[i];                
            }
            for (int i = localWindowLen; i < fftSize * upsample; i++)
            {
                windowOutput[i] = std::complex<float>(0.0, 0.0);                
            }            
            
            //Copy to fft input
            for (size_t i = 0; i < fftSize * upsample; i++)
            {
                fftIn[i][0] = windowOutput[i].real();
                fftIn[i][1] = windowOutput[i].imag();
            }
            
            //Execute fft
            fftwf_execute(plan);
            
            //Copy to output            
            for (int i = 0; i < fftSize * upsample; i++)
            {                        
                fftMagSqT[i] += (fftOut[i][0]*fftOut[i][0]+fftOut[i][1]*fftOut[i][1]);        
            }
        }
        for (int i = 0; i < fftSize * upsample; i++)
        {            
            float temp  =  10*log10(fftMagSqT[i]/numFFTs); 
            fftMagSqT[i] = temp;
        }                 

        FFTShift<std::vector<float>, float>(fftMagSqT, output);
    }

    void PowerSpectrumWindowLog(std::vector<float> &input, std::vector<float> &output, unsigned int windowLen, unsigned int fftSize, int upsample, int power, WindowType window, fftwf_plan plan, fftwf_complex* fftIn, fftwf_complex* fftOut, std::vector<float> &windowTaps)
    {
        //Upsample if selected
        std::vector<float> interpolated;
        if(upsample>1)
            processing::filter::Interpolate(input, interpolated, upsample);
        else
            interpolated = input;

        //Raise to power
        int powerToGo = power;        
        while(powerToGo > 1)
        {            
            for(int i = 0; i < interpolated.size(); i++)
            {                
                interpolated[i] = interpolated[i] * interpolated[i];
            }
            powerToGo = (int)(powerToGo/2);
        }

        //Normalize
        float scalingFactor = 1.0f / static_cast<float>(upsample);
        if(upsample > 1 && power > 1)
            scalingFactor = 1.0f / pow(upsample, power);
        for (size_t i = 0; i < interpolated.size(); i++)
        {
            interpolated[i] = interpolated[i] * scalingFactor;
        }   

        //Allocate FFT mag sq vector
        std::vector<float> fftMagSqT;
        fftMagSqT.resize(fftSize * upsample);  

        int offset =  windowLen * upsample;
        int numFFTs = interpolated.size()/offset;
        if(numFFTs == 0) numFFTs = 1;
        int remainingSamples = interpolated.size();
        for (int j = 0; j < numFFTs; j++)
        {
            //Apply window
            std::vector<float> windowOutput(fftSize * upsample);
            int localWindowLen = std::min(remainingSamples, static_cast<int>(offset));
            remainingSamples -= localWindowLen;
            for (int i = 0; i < localWindowLen; i++)
            {                
                windowOutput[i] = interpolated[j*offset+i]*windowTaps[i];                
            }
            for (int i = localWindowLen; i < fftSize * upsample; i++)
            {
                windowOutput[i] = 0.0f;                
            }            
            
            //Copy to fft input
            for (size_t i = 0; i < fftSize * upsample; i++)
            {
                fftIn[i][0] = windowOutput[i];
                fftIn[i][1] = 0;
            }
            
            //Execute fft
            fftwf_execute(plan);
            
            //Copy to output            
            for (int i = 0; i < fftSize * upsample; i++)
            {                        
                fftMagSqT[i] += (fftOut[i][0]*fftOut[i][0]+fftOut[i][1]*fftOut[i][1]);        
            }
        }
        for (int i = 0; i < fftSize * upsample; i++)
        {            
            float temp  =  10*log10(fftMagSqT[i]/numFFTs); 
            fftMagSqT[i] = temp;
        }               

        FFTShift<std::vector<float>, float>(fftMagSqT, output);
        
    }

    void PowerSpectrumWindowLog(std::vector<std::complex<float>> &input, std::vector<float> &output, unsigned int windowLen, unsigned int fftSize, int upsample, int power, WindowType window, fftwf_plan plan, fftwf_complex* fftIn, fftwf_complex* fftOut, std::vector<float> &windowTaps)
    {
        //Upsample if selected
        vcf interpolated;
        if(upsample>1)
            processing::filter::Interpolate(input, interpolated, upsample);
        else
            interpolated = input;

        //Raise to power
        int powerToGo = power;        
        while(powerToGo > 1)
        {            
            for(int i = 0; i < interpolated.size(); i++)
            {                
                interpolated[i] = interpolated[i] * interpolated[i];
            }
            powerToGo = (int)(powerToGo/2);
        }

        //Normalize
        float scalingFactor = 1.0f / static_cast<float>(upsample);
        if(upsample > 1 && power > 1)
            scalingFactor = 1.0f / pow(upsample, power);
        for (size_t i = 0; i < interpolated.size(); i++)
        {
            interpolated[i] = interpolated[i] * scalingFactor;
        }       

        //Allocate FFT mag sq vector
        std::vector<float> fftMagSqT;
        fftMagSqT.resize(fftSize * upsample);  

        int offset =  windowLen * upsample;
        int numFFTs = interpolated.size()/offset;
        if(numFFTs == 0) numFFTs = 1;
        int remainingSamples = interpolated.size();
        for (int j = 0; j < numFFTs; j++)
        {
            //Apply window
            vcf windowOutput(fftSize * upsample);
            int localWindowLen = std::min(remainingSamples, static_cast<int>(offset));
            remainingSamples -= localWindowLen;
            for (int i = 0; i < localWindowLen; i++)
            {                
                windowOutput[i] = interpolated[j*offset+i]*windowTaps[i];                
            }
            for (int i = localWindowLen; i < fftSize * upsample; i++)
            {
                windowOutput[i] = std::complex<float>(0.0, 0.0);                
            }            
            
            //Copy to fft input
            for (size_t i = 0; i < fftSize * upsample; i++)
            {
                fftIn[i][0] = windowOutput[i].real();
                fftIn[i][1] = windowOutput[i].imag();
            }
            
            //Execute fft
            fftwf_execute(plan);
            
            //Copy to output            
            for (int i = 0; i < fftSize * upsample; i++)
            {                        
                fftMagSqT[i] += (fftOut[i][0]*fftOut[i][0]+fftOut[i][1]*fftOut[i][1]);        
            }
        }
        for (int i = 0; i < fftSize * upsample; i++)
        {            
            float temp  =  10*log10(fftMagSqT[i]/numFFTs); 
            fftMagSqT[i] = temp;
        }                 

        FFTShift<std::vector<float>, float>(fftMagSqT, output);
    }

    void SpectrogramFFT(std::vector<float> &input, std::vector<float> &output, unsigned int windowLen, unsigned int fftSize, WindowType window, fftwf_plan plan, fftwf_complex* fftIn, fftwf_complex* fftOut, std::vector<float> &windowTaps)
    {              
        //Allocate FFT mag sq vector
        std::vector<float> fftMagSqT(fftSize);
        
        //for (int j = 0; j < numFFTs; j++)
        //{
        //Apply window
        vcf windowOutput(fftSize);                
        for (int i = 0; i < windowLen; i++)
        {       
            float re = input[i]*windowTaps[i];
            float im = 0;                  
            windowOutput[i] = std::complex<float>(re, im);                
        }
        for (int i = windowLen; i < fftSize; i++)
        {
            windowOutput[i] = std::polar(0.0f, 0.0f);                
        }            
        
        //Copy to fft input
        for (size_t i = 0; i < fftSize; i++)
        {
            fftIn[i][0] = windowOutput[i].real();
            fftIn[i][1] = windowOutput[i].imag();
        }
        
        //Execute fft
        fftwf_execute(plan);
        
        //Copy to output            
        for (int i = 0; i < fftSize; i++)
        {                        
            fftMagSqT[i] = 10*log10((fftOut[i][0]*fftOut[i][0]+fftOut[i][1]*fftOut[i][1]));        
        }                         

        FFTShift<std::vector<float>, float>(fftMagSqT, output);
    }

    void SpectrogramFFT(std::vector<std::complex<float>> &input, std::vector<float> &output, unsigned int windowLen, unsigned int fftSize, WindowType window, fftwf_plan plan, fftwf_complex* fftIn, fftwf_complex* fftOut, std::vector<float> &windowTaps)
    {              
        //Allocate FFT mag sq vector
        std::vector<float> fftMagSqT(fftSize);
        
        //for (int j = 0; j < numFFTs; j++)
        //{
        //Apply window
        vcf windowOutput(fftSize);                
        for (int i = 0; i < windowLen; i++)
        {                            
            windowOutput[i] = input[i]*windowTaps[i];                
        }
        for (int i = windowLen; i < fftSize; i++)
        {
            windowOutput[i] = std::polar(0.0f, 0.0f);                
        }            
        
        //Copy to fft input
        for (size_t i = 0; i < fftSize; i++)
        {
            fftIn[i][0] = windowOutput[i].real();
            fftIn[i][1] = windowOutput[i].imag();
        }
        
        //Execute fft
        fftwf_execute(plan);
        
        //Copy to output            
        for (int i = 0; i < fftSize; i++)
        {                        
            fftMagSqT[i] = 10*log10f((fftOut[i][0]*fftOut[i][0]+fftOut[i][1]*fftOut[i][1]));        
        }                         

        FFTShift<std::vector<float>, float>(fftMagSqT, output);
    }


    void PowerSpectrumLogWithLiquid(std::vector<float> &input, std::vector<float> &output, unsigned int windowLen, unsigned int fftSize, int power, liquid_window_type window)
    {        
        vector<float> interpolated;
        if(power>1)
            processing::filter::Interpolate(input, interpolated, power);
        else
            interpolated = input;

        //Complex multiply upsamples array with itself "power" times
        int powerToGo = power;
        
        while(powerToGo > 1)
        {
            float realSum = 0.0;

            for(int i = 0; i < interpolated.size(); i++)
            {
                realSum += fabs(interpolated[i]);
            }

            float realAvg = realSum / interpolated.size();
            float scalingFactor = 1.0 / realAvg;

            for(int i = 0; i < interpolated.size(); i++)
            {
                interpolated[i] = interpolated[i] * scalingFactor;
                interpolated[i] = interpolated[i] * interpolated[i];
            }
            powerToGo = (int)(powerToGo/2);
        }

        //std::complex<float>* spinput = (std::complex<float>*) malloc(interpolated.size()*sizeof(complex<float>));
        //memcpy(&spinput[0], &interpolated[0],interpolated.size()*sizeof(complex<float>));        

        spgramf q = spgramf_create(fftSize*power, window, windowLen, windowLen);
        spgramf_write(q, interpolated.data(), interpolated.size());

        float * psd = (float *) malloc(fftSize*power*sizeof(float));
        
        spgramf_get_psd(q, psd);
        spgramf_destroy(q);

        output.resize(fftSize*power);
        for (int i = 0; i < fftSize*power; i++)
        {
            float t = psd[i];
            output[i] = t;//10*log10(psd[i]);
        }        
        //memcpy(&output[0], &psd[0], fftSize*sizeof(float));
        
        free(psd);
        //free(spinput);
    }

    void PowerSpectrumLogWithLiquid(std::vector<std::complex<float>> &input, std::vector<float> &output, unsigned int windowLen, unsigned int fftSize, int power, liquid_window_type window)
    {        
        vector<complex<float>> interpolated;
        if(power>1)
            processing::filter::Interpolate(input, interpolated, power);
        else
            interpolated = input;

        //Complex multiply upsamples array with itself "power" times
        int powerToGo = power;
        
        while(powerToGo > 1)
        {
            float realSum = 0.0;

            for(int i = 0; i < interpolated.size(); i++)
            {
                realSum += fabs(interpolated[i].real());
            }

            float realAvg = realSum / interpolated.size();
            float scalingFactor = 1.0 / realAvg;

            for(int i = 0; i < interpolated.size(); i++)
            {
                interpolated[i] = interpolated[i] * scalingFactor;
                interpolated[i] = interpolated[i] * interpolated[i];
            }
            powerToGo = (int)(powerToGo/2);
        }

        //std::complex<float>* spinput = (std::complex<float>*) malloc(interpolated.size()*sizeof(complex<float>));
        //memcpy(&spinput[0], &interpolated[0],interpolated.size()*sizeof(complex<float>));        

        spgramcf q = spgramcf_create(fftSize*power, window, windowLen, windowLen);
        spgramcf_write(q, interpolated.data(), interpolated.size());

        float * psd = (float *) malloc(fftSize*power*sizeof(float));
        
        spgramcf_get_psd(q, psd);
        spgramcf_destroy(q);

        output.resize(fftSize*power);
        for (int i = 0; i < fftSize*power; i++)
        {
            float t = psd[i];
            output[i] = t;//10*log10(psd[i]);
        }        
        //memcpy(&output[0], &psd[0], fftSize*sizeof(float));
        
        free(psd);
        //free(spinput);
    }

    void CalculateWaterfallLiquid(vcf input, vector<float> &output, unsigned int nfft, liquid_window_type window, unsigned int windowLength, unsigned int delay, unsigned int time)
    {
        spwaterfallcf periodogram = spwaterfallcf_create(nfft, window, windowLength, delay, time);
        spwaterfallcf_print(periodogram);
        unsigned int numSamples = input.size();
        spwaterfallcf_write(periodogram, input.data(), numSamples);
        //spwaterfallcf_export(periodogram, "waterfalltest");
        //spwaterfallf_set_rate(<#spwaterfallf _q#>, <#float _rate#>)
        const float * psd = (float *) malloc(nfft * 2 * time * sizeof(float));
        psd = spwaterfallcf_get_psd(periodogram);
        output.resize(nfft*2*time);
        for (int i = 0; i < nfft*2*time; i++)
        {
            output[i] = psd[i];
        }        
    }

    /**
     * Performs autocorrelation through FFT and real vector
    */

    void autoCorrFFT(std::vector<float> input, std::vector<float> &output, unsigned int fftSize)
    {
        //Setup fftw3 plans (forward and backward)
        fftwf_plan planf, planb;        
        fftwf_complex* x = (fftwf_complex*) fftwf_malloc(fftSize * sizeof(fftwf_complex));
        fftwf_complex* X = (fftwf_complex*) fftwf_malloc(fftSize * sizeof(fftwf_complex));        
        planf = fftwf_plan_dft_1d(fftSize, x, X, FFTW_FORWARD, FFTW_ESTIMATE);
        fftwf_complex* Rx = (fftwf_complex*) fftwf_malloc(fftSize * sizeof(fftwf_complex));
        fftwf_complex* rx = (fftwf_complex*) fftwf_malloc(fftSize * sizeof(fftwf_complex));
        planb = fftwf_plan_dft_1d(fftSize, Rx, rx, FFTW_BACKWARD, FFTW_ESTIMATE);

        //Set up local variables
        unsigned int numSamples = input.size();        
        unsigned int numFFTs = ceil(double(numSamples)/double(fftSize));
        output.clear();
        output.resize(fftSize/2); 

        std::vector<float> windowTaps(fftSize);
        SetupWindow(WINDOW_HAMMING, fftSize, windowTaps);
        
        //For each block of data do autocorrelation
        for (int j = 0; j < numFFTs; j++)
        {
            //Copy data to fft input            
            unsigned int localWindowLen = (numSamples<fftSize) ? numSamples : fftSize;
            numSamples -= fftSize;            
            for (int i = 0; i < localWindowLen; i++)
            {                                
                x[i][0] = input[j*fftSize+i]*windowTaps[i];
                x[i][1] = 0;
            }
            for (int i = localWindowLen; i < fftSize; i++)
            {
                x[i][0] = 0;
                x[i][1] = 0;
            }
            
            //Execute fft
            fftwf_execute(planf);

            //Do autoCorr X(w)*conj(X(w))           
            for (size_t i = 0; i < fftSize; i++)
            {                        
                Rx[i][0] = X[i][0]*X[i][0]+X[i][1]*X[i][1];        
                Rx[i][1] = 0;        
            }
            fftwf_execute(planb);

            //Copy output of FFT into output vector
            for (size_t i = 0; i < fftSize/2; i++)
            {                       
                output[i] += (rx[i][0]*rx[i][0]+rx[i][1]*rx[i][1])/fftSize;//rx[i][0]*rx[i][0]+rx[i][1]*rx[i][1];
            }
        }
               
        for (size_t i = 0; i < fftSize/2; i++)
        {            
            output[i] = 10*log10(output[i]/numFFTs); //not sure if normalization is correct            
        }

        fftwf_free(x);
        fftwf_free(X);
        fftwf_free(Rx);
        fftwf_free(rx);
        fftwf_destroy_plan(planf);
        fftwf_destroy_plan(planb);
        fftwf_cleanup();
    }


    void autoCorrFFT(std::vector<std::complex<float>> input, std::vector<float> &output, unsigned int fftSize)
    {        
        //Setup fftw3 plans (forward and backward)
        fftwf_plan planf, planb;        
        fftwf_complex* x = (fftwf_complex*) fftwf_malloc(fftSize * sizeof(fftwf_complex));
        fftwf_complex* X = (fftwf_complex*) fftwf_malloc(fftSize * sizeof(fftwf_complex));        
        planf = fftwf_plan_dft_1d(fftSize, x, X, FFTW_FORWARD, FFTW_ESTIMATE);
        fftwf_complex* Rx = (fftwf_complex*) fftwf_malloc(fftSize * sizeof(fftwf_complex));
        fftwf_complex* rx = (fftwf_complex*) fftwf_malloc(fftSize * sizeof(fftwf_complex));
        planb = fftwf_plan_dft_1d(fftSize, Rx, rx, FFTW_BACKWARD, FFTW_ESTIMATE);

        //Set up local variables
        unsigned int numSamples = input.size();        
        unsigned int numFFTs = ceil(double(numSamples)/double(fftSize));   
        output.clear();
        output.resize(fftSize/2);      

        std::vector<float> windowTaps(fftSize);
        SetupWindow(WINDOW_HAMMING, fftSize, windowTaps);
        
        //For each block of data do autocorrelation
        for (int j = 0; j < numFFTs; j++)
        {
             //Copy data to fft input            
            unsigned int localWindowLen = (numSamples<fftSize) ? numSamples : fftSize;
            numSamples -= fftSize;
            std::complex<float> temp;
            for (int i = 0; i < localWindowLen; i++)
            {                        
                temp = input[j*fftSize+i] * windowTaps[i];        
                x[i][0] = temp.real();
                x[i][1] = temp.imag();
            }
            for (int i = localWindowLen; i < fftSize; i++)
            {
                x[i][0] = 0;
                x[i][1] = 0;
            }
            
            //Execute fft
            fftwf_execute(planf);

            //Do autoCorr X(w)*conj(X(w))           
            for (size_t i = 0; i < fftSize; i++)
            {                        
                Rx[i][0] = X[i][0]*X[i][0]+X[i][1]*X[i][1];        
                Rx[i][1] = 0;        
            }
            fftwf_execute(planb);

            //Copy output of FFT into output vector
            for (size_t i = 0; i < fftSize/2; i++)
            {                       
                output[i] += (rx[i][0]*rx[i][0]+rx[i][1]*rx[i][1])/fftSize;//rx[i][0]*rx[i][0]+rx[i][1]*rx[i][1];
            }
        }
               
        for (size_t i = 0; i < fftSize/2; i++)
        {
            output[i] = 10*log10(output[i]/numFFTs); //not sure if normalization is correct
        }

        fftwf_free(x);
        fftwf_free(X);
        fftwf_free(Rx);
        fftwf_free(rx);
        fftwf_destroy_plan(planf);
        fftwf_destroy_plan(planb);
        fftwf_cleanup();
    }

    void autoCorrTD(std::vector<float> input, std::vector<float> &output, int delay)
    {
        int inputSize = input.size();
        output.resize(inputSize);
        int delayIndex;
        for (size_t i = 0; i < inputSize; i++)
        {
            delayIndex = i + delay;
            if(delayIndex >= inputSize)
                delayIndex -= inputSize;
            output[i] = input[i]*input[delayIndex];
        }        
    }

    void autoCorrTD(std::vector<std::complex<float>> input, std::vector<std::complex<float>> &output, int delay)
    {
        int inputSize = input.size();
        output.resize(inputSize);
        int delayIndex;
        for (size_t i = 0; i < inputSize; i++)
        {
            delayIndex = i + delay;
            if(delayIndex >= inputSize)
                delayIndex -= inputSize;
            output[i] = input[i]*conj(input[delayIndex]);
        }
        
    }

    /**
     * Integrate with filter
    */
   void Integrate(std::vector<float> input, std::vector<float> &output, int fftSize, int delay)
   {
    int symbolPeriod = fftSize + delay;
    int numBlocks = input.size()/symbolPeriod;
    if(numBlocks == 0) numBlocks = 1;
    vector<float> integratedBlock(symbolPeriod);
    int remainingSamples = input.size();
    for (size_t i = 0; i < numBlocks; i++)
    {
        size_t tempLen = min(remainingSamples, symbolPeriod);
        remainingSamples -= tempLen;
        for (size_t j = 0; j < tempLen; j++)
        {
            integratedBlock[j] += input[i*symbolPeriod + j];
        }        
    }
    vector<float> integratedBlockMag(symbolPeriod);
    for (size_t i = 0; i < symbolPeriod; i++)
    {        
        integratedBlockMag[i] = integratedBlock[i]*integratedBlock[i]/static_cast<float>(numBlocks);
    }
    vector<float> integratedBlockMagFiltered(symbolPeriod);
    int filterLen = delay;
    if(filterLen < 3)
        filterLen = 3;
    vector<float> lowpassFilter(filterLen);
    lowpassFilter.assign(filterLen, 1.0f/static_cast<float>(filterLen));
    for (size_t i = 0; i < symbolPeriod; i++)
    {
        for (size_t j = 0; j < filterLen; j++)
        {
            integratedBlockMagFiltered[i] += integratedBlockMag[(i+j)%symbolPeriod] * lowpassFilter[j];
        }
        integratedBlockMagFiltered[i] = integratedBlockMagFiltered[i]/static_cast<float>(filterLen);
    }
    output = integratedBlockMagFiltered;    
   }

   void Integrate(std::vector<std::complex<float>> input, std::vector<float> &output, int fftSize, int delay)
   {
    int symbolPeriod = fftSize + delay;
    int numBlocks = input.size()/symbolPeriod;
    if(numBlocks == 0) numBlocks = 1;
    vcf integratedBlock(symbolPeriod);
    int remainingSamples = input.size();
    for (size_t i = 0; i < numBlocks; i++)
    {
        size_t tempLen = min(remainingSamples, symbolPeriod);
        remainingSamples -= tempLen;
        for (size_t j = 0; j < tempLen; j++)
        {
            integratedBlock[j] += input[i*symbolPeriod + j];
        }        
    }
    vector<float> integratedBlockMag(symbolPeriod);
    for (size_t i = 0; i < symbolPeriod; i++)
    {
        float re = integratedBlock[i].real()/static_cast<float>(numBlocks);
        float imag = integratedBlock[i].imag()/static_cast<float>(numBlocks);
        integratedBlockMag[i] = re*re + imag*imag;
    }
    vector<float> integratedBlockMagFiltered(symbolPeriod);
    int filterLen = delay;
    if(filterLen < 3)
        filterLen = 3;
    vector<float> lowpassFilter(filterLen);
    lowpassFilter.assign(filterLen, 1.0f/static_cast<float>(filterLen));
    for (size_t i = 0; i < symbolPeriod; i++)
    {
        for (size_t j = 0; j < filterLen; j++)
        {
            integratedBlockMagFiltered[i] += integratedBlockMag[(i+j)%symbolPeriod] * lowpassFilter[j];
        }
        integratedBlockMagFiltered[i] = integratedBlockMagFiltered[i]/static_cast<float>(filterLen);
    }
    output = integratedBlockMagFiltered;    
   }

    /**
     * Returns real of complex
     * 
    */

    void RealDetect(std::vector<std::complex<float>> input, std::vector<float> &output)
    {
        output.resize(input.size());
        for (size_t i = 0; i < input.size(); i++)
        {
            output[i] = input[i].real();
        }        
    }

    /**
     * Returns real of complex
     * 
    */

    void ImagDetect(std::vector<std::complex<float>> input, std::vector<float> &output)
    {
        output.resize(input.size());
        for (size_t i = 0; i < input.size(); i++)
        {
            output[i] = input[i].imag();
        }        
    }

    /**
     * Performs AM detect on complex float input
     * 
    */

    void AMDetect(std::vector<float> input, std::vector<float> &output)
    {
        output.resize(input.size());
        for (size_t i = 0; i < input.size(); i++)
        {
            output[i] = input[i]*input[i];
        }        
    }

    void AMDetect(std::vector<std::complex<float>> input, std::vector<float> &output)
    {
        output.resize(input.size());
        for (size_t i = 0; i < input.size(); i++)
        {
            output[i] = input[i].real()*input[i].real()+input[i].imag()*input[i].imag();
        }        
    }

    /**
     * Performs AM detect on complex float input
     * 
    */

    void AMDetectLog(std::vector<float> input, std::vector<float> &output)
    {
        output.resize(input.size());
        for (size_t i = 0; i < input.size(); i++)
        {
            output[i] = 10*log10(input[i]*input[i]);
        }        
    }

    void AMDetectLog(std::vector<std::complex<float>> input, std::vector<float> &output)
    {
        output.resize(input.size());
        for (size_t i = 0; i < input.size(); i++)
        {
            output[i] = 10*log10(input[i].real()*input[i].real()+input[i].imag()*input[i].imag());
        }        
    }

    /**
     * Performs FM detect on complex float input
     * 
    */ 
    void FMDetect(std::vector<float> input, std::vector<float> &output, float scale)
    {
        output.resize(input.size());
        float prev, current;           
        for (size_t i = 1; i < input.size(); i++)
        {            
            prev = input[i-1];
            current = input[i];
            std::complex<float> t = std::complex(prev*current,0.0f);
            output[i] = arg(t) * scale/(2*M_PI);            
        } 
    }

    void FMDetect(std::vector<std::complex<float>> &input, std::vector<float> &output, float scale)
    {
        output.resize(input.size());           
        for (size_t i = 1; i < input.size(); i++)
        {            
            std::complex<float> prev;
            std::complex<float> current;
            prev = input[i-1];
            current = input[i];
            output[i] = arg(conj(prev)*current) * scale/(2*M_PI);            
        }               
    }

    /**
     * Performs phase detect on complex float input
     * Counter-clockwise is positive phase trajectory
     * 
    */     

    void PhaseDetect(std::vector<float> input, std::vector<float> &output, bool deg)
    {
        float scale = 1.0f;
        if(deg)
            scale = 180.0f/M_PI;
        output.resize(input.size());           
        for (size_t i = 0; i < input.size(); i++)
        {                        
            output[i] = -arg(input[i])*scale;// * scale/(2*M_PI);            
        }               
    }

    void PhaseDetect(std::vector<std::complex<float>> input, std::vector<float> &output, bool deg)
    {
        float scale = 1.0f;
        if(deg)
            scale = 180.0f/M_PI;
        output.resize(input.size());           
        for (size_t i = 0; i < input.size(); i++)
        {                        
            output[i] = -arg(input[i])*scale;// * scale/(2*M_PI);            
        }               
    }
}