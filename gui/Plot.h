
#include <complex>
#include "AbstractSampleSource.h"
#include "SampleSource.h"
#include "Structs.h"

#pragma once

namespace gui
{
    class Plot
    {
        public:
            Plot() {};
            virtual ~Plot() {};
            virtual void Update(std::shared_ptr<AbstractSampleSource> src, int &currentSampleIndex, Markers &markers, bool &updatePlots) = 0;
            virtual int GetSamplesPerRow() { return samplesPerRow; }

        protected:
            virtual void UpdateData(std::shared_ptr<SampleSource<std::complex<float>>> src, int &currentSampleIndex) = 0;
            virtual void UpdateData(std::shared_ptr<SampleSource<float>> src, int &currentSampleIndex) = 0;
            virtual void ShowControls(int &currentSampleIndex, Markers &markers, bool &updatePlots) = 0;
            virtual void PlotData(int &currentSampleIndex, Markers &markers, bool &updatePlots) = 0;
            int samplesPerRow;
    };
}