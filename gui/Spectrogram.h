
#include "Plot.h"
#include "imgui.h"
#include "implot.h"
#include <vector>
#include <fftw3.h>
#include "Structs.h"
#include "Transformations.h"
#include "Windowing.h"
#include "SampleSource.h"
#include "Helpers.h"
#include "Utils.h"
#include "chrono"

typedef std::vector<std::complex<float>> vcf;
typedef std::vector<float> vf;
typedef std::vector<double> vd;
/*
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
*/

#pragma once

namespace gui
{
    class Spectrogram : Plot
    {
        public:
            Spectrogram();
            ~Spectrogram();  
            void Update(std::shared_ptr<AbstractSampleSource> src, int &currentSampleIndex, Markers &markers, bool &updatePlots);
            int GetSamplesPerRow() { return samplesPerRow; }

        private:
            void PlotData(int &currentSampleIndex, Markers &markers, bool &updatePlots);
            void ShowControls(int &currentSampleIndex, Markers &markers, bool &updatePlots);
            virtual void UpdateData(std::shared_ptr<SampleSource<std::complex<float>>> src, int &currentSampleIndex) ;
            virtual void UpdateData(std::shared_ptr<SampleSource<float>> src, int &currentSampleIndex);

            vf spectrogram;
            std::vector<processing::utils::ExamplePoint> spectrogram2D;
            vf miniPowerWindow; 
            vector<int> fftSizesList;// = {64,128,256,512,1024,2048,4096,8192,16384};
            int fftSize = 256;    
            int fftSizeIndex = 2;
            int powerTransformList[7] = {1,2,4,8,-1,-2,-3}; //-1 = AM, -2 = FM, -3 = AutoCorr
            int transformationPower = 1;
            int powerTransformIndex = 0;
            double samplingRate = 0;
            int numSamples;
            vf binIndices;   
            float yMin;
            float yMax;
            vf miniBinIndices;
            int samplesPerRowPlot = 1024; 
            int samplesPerRowPlotMax = 1024;
            bool updateDownsampling = true;
            bool initYMinMax = true;
            int windowShift = 8;
            double yMinPSD = -60;
            double yMaxPSD = -50;  
            int xAxisMin = 0;
            int xAxisMax = 8192;
            float realFactor = 1;            
            bool setXAxis = true;
            bool setYAxis = true;
            ImPlotRect axis;
            vector<float> powerWindow;                  
            vector<WindowType> windowTypeList = {
                WINDOW_HAMMING,          // Hamming
                WINDOW_HANN,             // Hann
                WINDOW_BLACKMANHARRIS,   // Blackman-harris (4-term)
                WINDOW_BLACKMANHARRIS7,  // Blackman-harris (7-term)
                WINDOW_KAISER,           // Kaiser (beta factor unspecified)
                WINDOW_FLATTOP,          // flat top (includes negative values)
                WINDOW_TRIANGULAR,       // triangular
                WINDOW_RCOSTAPER        // raised-cosine taper (taper size unspecified)
                //WINDOW_KBD               // Kaiser-Bessel derived window (beta factor unspecified)
                };
            int windowTypeIndex= 0;
            WindowType windowType = windowTypeList[windowTypeIndex];
            int windowLength = 256;
            ImPlotColormap map = ImPlotColormap_Greys;
            vd periodicXVals;
            int numPeriodicXVals = 0;
            bool updatePeriodicXVals = false;

            vector<fftwf_plan> fftwForwardPlans; //fftw plans            
            
            fftwf_complex* fftForwardIn = (fftwf_complex*) fftwf_malloc(16384 * sizeof(fftwf_complex)); //room for 16384*8
            fftwf_complex* fftForwardOut = (fftwf_complex*) fftwf_malloc(16384 * sizeof(fftwf_complex));//room for 16384*8            
    };
}