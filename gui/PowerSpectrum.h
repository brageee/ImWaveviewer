#include <vector>
#include <complex>
#include <memory>
#include "Plot.h"
#include "imgui.h"
#include "implot.h"
#include "Structs.h"
#include "Transformations.h"
#include "Windowing.h"
#include "SampleSource.h"
#include "Helpers.h"

using namespace std;

typedef vector<std::complex<float>> vcf;
typedef vector<float> vf;
typedef vector<double> vd;

#pragma once

namespace gui
{
    class PowerSpectrum : Plot
    {
        public:
            PowerSpectrum();
            // Destructor.
            ~PowerSpectrum();  
            void Update(std::shared_ptr<AbstractSampleSource> src, int &currentSampleIndex, Markers &markers, bool &updatePlots);
            int GetSamplesPerRow() { return samplesPerRow; }

        private:
            void PlotData(int &currentSampleIndex, Markers &markers, bool &updatePlots);
            void ShowControls(int &currentSampleIndex, Markers &markers, bool &updatePlots);
            virtual void UpdateData(std::shared_ptr<SampleSource<std::complex<float>>> src, int &currentSampleIndex) ;
            virtual void UpdateData(std::shared_ptr<SampleSource<float>> src, int &currentSampleIndex);

            //Time Domain variables and parameters
            int fftSizesList[9] = {64,128,256,512,1024,2048,4096,8192,16384};
            int fftSize = 1024;    
            int fftSizeIndex = 4;
            int powerTransformList[9] = {1,2,4,8,-1,-2,-3, -4, -5}; //-1 = AM, -2 = FM, -3 = AutoCorr, -4 = DelayAndCorrelate, -5 = DelayAndIntegrate
            int transformationPower = 1;
            int powerTransformIndex = 0;
            int delay = 1;
            int upsampleInt = 1;
            bool upsampleBool = false;
            double samplingRate = 0;
            int numSamples;
            vf binIndices;    
            vf powerWindow;      
            bool setAxisFit = true; 
            bool setAxisFitAfterUpdate = true;
            ImPlotRect axis;
            vd periodicXVals;
            int numPeriodicXVals = 0;
            bool updatePeriodicXVals = false;
            
            //bool recalcSpectrum = true; 
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
            int windowLength = 1024;
            bool windowBool = false;

    };
}