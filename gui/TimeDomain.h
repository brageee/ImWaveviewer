#pragma once

#include <vector>
#include <complex>
#include <memory>
#include <string.h>

#include "Plot.h"
#include "imgui.h"
#include "implot.h"

#include "Structs.h"
#include "Transformations.h"
#include "SampleSource.h"
#include "AbstractSampleSource.h"
#include "SampleProc.h"
#include "Filter.h"


typedef std::vector<std::complex<float>> vcf;
typedef std::vector<float> vf;
typedef std::vector<double> vd;

namespace gui
{

    class TimeDomain : Plot
    {
        public:
            TimeDomain();
            // Destructor.
            ~TimeDomain();  
            void Update(std::shared_ptr<AbstractSampleSource> src, int &currentSampleIndex, Markers &markers, bool &updatePlots);
            int GetSamplesPerRow() { return samplesPerRow; }

        private:
            void PlotData(int &currentSampleIndex, Markers &markers, bool &updatePlots);
            void ShowControls(int &currentSampleIndex, Markers &markers, bool &updatePlots);
            virtual void UpdateData(std::shared_ptr<SampleSource<std::complex<float>>> src, int &currentSampleIndex) ;
            virtual void UpdateData(std::shared_ptr<SampleSource<float>> src, int &currentSampleIndex);

            //Time Domain variables and parameters
            vf signalReal;
            vf signalImag;
            bool displayReal = true;
            bool deriveReal = false;
            bool displayImag = false;
            bool displayAM = false;
            bool displayFM = false;
            bool displayPhase = false;
            int numSamples;
            bool isReal;
            ImPlotRect axis;
            vector<bool> setAxisFit {false, false, false, false, false};
            vector<bool> resizeY {true, true, true, true, true};            

            int traceController = 0; //0 real, 1 imag, 2 am, 3 fm, 4 phase
            bool traceResize = true;
            vector<complex<float>> dataWindowCF;         
            vector<float> dataWindowF;     
            vd displayIndices;   
            vd dataReal;
            vd dataImag;
            vd dataAM;
            vd dataFM;
            vd dataPhase;
            vd periodicXVals;
            vd periodicYVals;            
            int numPeriodicXVals = 0;
            int numPeriodicYVals = 0;
            bool updatePeriodicXVals = false;
            bool updatePeriodicYVals = false;
            float degScale = 1.0f;
            float yRealMin = 0.0f;
            float yRealMax = 0.0f;            
            float yImagMin = 0.0f;
            float yImagMax = 0.0f;
            float yAMMin = 0.0f;
            float yAMMax = 0.0f;
            float yFMMin = 0.0f;
            float yFMMax = 0.0f;
            float yPhaseMin = 0.0f;
            float yPhaseMax = 0.0f;            
            vector<int> filterLenMA {1, 1, 1, 1, 1};
            vector<int> filterLenMedian {1, 1, 1, 1, 1};
            int filterLenTemp = 1;

            processing::filter::MovingAverage<float> realMA{1};
            processing::filter::MovingAverage<float> imagMA{1};
            processing::filter::MovingAverage<float> amMA{1};
            processing::filter::MovingAverage<float> fmMA{1};
            processing::filter::MovingAverage<float> phaseMA{1};

            processing::filter::Median<float> realMedian{1};
            processing::filter::Median<float> imagMedian{1};
            processing::filter::Median<float> amMedian{1};
            processing::filter::Median<float> fmMedian{1};
            processing::filter::Median<float> phaseMedian{1};
            
    };
}

