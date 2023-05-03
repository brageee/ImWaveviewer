#include <vector>
#include <complex>
#include <memory>

#include "Plot.h"
#include "imgui.h"
#include "implot.h"
#include "Helpers.h"
#include "Structs.h"
#include "SampleSource.h"
#include "Filter.h"


typedef std::vector<std::complex<float>> vcf;
typedef std::vector<float> vf;

#pragma once

namespace gui
{
    class Scatterplot : Plot
    {
        public:
            Scatterplot();
            // Destructor.
            ~Scatterplot();  
            void Update(std::shared_ptr<AbstractSampleSource> src, int &currentSampleIndex, Markers &markers, bool &updatePlots);
            int GetSamplesPerRow() { return samplesPerRow; }

        private:
            void PlotData(int &currentSampleIndex, Markers &markers, bool &updatePlots);
            void ShowControls(int &currentSampleIndex, Markers &markers, bool &updatePlots);
            virtual void UpdateData(std::shared_ptr<SampleSource<std::complex<float>>> src, int &currentSampleIndex) ;
            virtual void UpdateData(std::shared_ptr<SampleSource<float>> src, int &currentSampleIndex);
            void UpdateDecimationComplex();
            void UpdateDecimationReal();

            //Scatterplot variables and parameters
            std::string currentSampleIndexStr = ""; // to facility math expressions
            std::string samplesPerRowStr = ""; // to facility math expressions
            size_t numSamples;
            int numRows = 1;
            int numCols = 1;
            int decimation = 1;
            bool updateDecimation = true;
            bool showSubplotIndex = false;
            ImPlotSubplotFlags flags = 0;//ImPlotSubplotFlags_LinkRows | ImPlotSubplotFlags_LinkCols; 
            bool setAxisFit = true; 
            ImPlotRect axis;
            double samplingRate = 0.0f;
            vcf samplesWindowComplex;
            vf samplesWindowReal;
            bool isReal = false;
            std::vector<vf> samplesWindowMatrixReal;
            std::vector<vf> samplesWindowMatrixImag;
            int pointSizeInt = 1;
            double pointSizeDouble = 1.0f;
            double scaleDouble = 1.0f;
            double xMin = -1.0f;
            double xMax = 1.0f;
            double yMin = -1.0f;
            double yMax = 1.0f;
            bool scale = false;
            double freqShift = 0.0f;
            double phaseAngle = 0.0f;
            float piOver180 = M_PI/180.0f;
            
    };
}