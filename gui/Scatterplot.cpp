#include "Scatterplot.h"

namespace gui
{

    Scatterplot::Scatterplot()
    {
        samplesPerRow = 8192;
    }

    Scatterplot::~Scatterplot()
    {
        
    }

    void Scatterplot::Update(std::shared_ptr<AbstractSampleSource> src, int &currentSampleIndex, Markers &markers, bool &updatePlots)
    {
        numSamples = src->Count();    
        isReal = src->RealSignal();    
        /*ShowControls(currentSampleIndex, markers, updatePlots);
        ImGui::SameLine();
        ImGui::BeginChild("ChildR", ImVec2(-1, -1));                                  
        */        
        
        //Update plot data
        if(updatePlots)
        {
            if(!isReal)         
            {
                std::shared_ptr<SampleSource<std::complex<float>>> concrete = std::dynamic_pointer_cast<SampleSource<std::complex<float>>>(src);
                UpdateData(concrete, currentSampleIndex);
                updatePlots = false;
            } else
            {
                std::shared_ptr<SampleSource<float>> concrete = std::dynamic_pointer_cast<SampleSource<float>>(src);
                UpdateData(concrete, currentSampleIndex);
                updatePlots = false;
            }
            updateDecimation = true;
        }
        if(updateDecimation)
        {
            if(!isReal) UpdateDecimationComplex();
            else UpdateDecimationReal();
            updateDecimation = false;
        }

        static ImGuiTableFlags flags = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable;// | ImGuiTableFlags_RowBg;// | ImGuiTableFlags_NoBordersInBody;
        if (ImGui::BeginTable("", 2, flags))
        {
            ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthFixed, ImGui::GetWindowContentRegionWidth()* 0.2f);
            ImGui::TableSetupColumn("Plot", ImGuiTableColumnFlags_NoHide);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ShowControls(currentSampleIndex, markers, updatePlots);                
            ImGui::TableNextColumn();
            PlotData(currentSampleIndex, markers, updatePlots);
            ImGui::EndTable();
        }
        
    }

    void Scatterplot::PlotData(int &currentSampleIndex, Markers &markers, bool &updatePlots)
    {
        if(setAxisFit)
            ImPlot::SetNextAxesToFit(); 
        //Plot scatterplot   
        ImGui::BeginChild("PlotData");
        if(ImPlot::BeginSubplots("##scatter", numRows, numCols, ImVec2(-1,0.9f*ImGui::GetWindowHeight()), flags))
        { 
            for (size_t i = 0; i < numRows * numCols; i++)
            {
                //Set line style
                ImVec4 color = ImVec4(0.409f, 0.510f, 0.835f, 1.000f);
                ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, pointSizeDouble, color, IMPLOT_AUTO, color);
                if (ImPlot::BeginPlot("")) {
                    if(scale) 
                        ImPlot::SetupAxesLimits(xMin,xMax,yMin,yMax, ImPlotCond_Always);                      
                    if(showSubplotIndex) ImPlot::PlotScatter(std::to_string(i).c_str(), samplesWindowMatrixReal[i].data(), samplesWindowMatrixImag[i].data(),samplesWindowMatrixReal[i].size());
                    else ImPlot::PlotScatter("", samplesWindowMatrixReal[i].data(), samplesWindowMatrixImag[i].data(),samplesWindowMatrixReal[i].size());
                    ImPlot::EndPlot();
                }
            }
            ImPlot::EndSubplots();
        }         
        setAxisFit = false;
        if(numSamples>0)
        {
            ImGui::SetNextItemWidth(-1);
            if(ImGui::SliderInt("##sampleSlider", &currentSampleIndex, 0, numSamples-samplesPerRow))
            {
                setAxisFit = true;
                updatePlots = true;
            }
                
        }                                          
        ImGui::EndChild();
    }

    void Scatterplot::UpdateDecimationReal()
    {
        samplesWindowMatrixReal.resize(decimation);
        samplesWindowMatrixImag.resize(decimation);
        for (size_t i = 0; i < decimation; i++)
        {
            samplesWindowMatrixReal[i].clear();
            samplesWindowMatrixImag[i].clear();
        }
        for (size_t i = 0; i < samplesPerRow/decimation; i++)
        {
            for (size_t j = 0; j < decimation; j++)
            {
                samplesWindowMatrixReal[j].push_back(samplesWindowReal[i*decimation+j]);
                samplesWindowMatrixImag[j].push_back(0);
            }                
        }
    }

    void Scatterplot::UpdateDecimationComplex()
    {
        samplesWindowMatrixReal.resize(decimation);
        samplesWindowMatrixImag.resize(decimation);
        for (size_t i = 0; i < decimation; i++)
        {
            samplesWindowMatrixReal[i].clear();
            samplesWindowMatrixImag[i].clear();
        }
        for (size_t i = 0; i < samplesPerRow/decimation; i++)
        {
            for (size_t j = 0; j < decimation; j++)
            {
                samplesWindowMatrixReal[j].push_back(samplesWindowComplex[i*decimation+j].real());
                samplesWindowMatrixImag[j].push_back(samplesWindowComplex[i*decimation+j].imag());
            }                
        }
    }

    void Scatterplot::UpdateData(std::shared_ptr<SampleSource<float>> src, int &currentSampleIndex)
    {                       
        //Need code below in case plot is initialized with SPR>numSamples and
        //the code in Helper is not called        
        if(currentSampleIndex+samplesPerRow >= numSamples)
            currentSampleIndex = numSamples-samplesPerRow;
        if(currentSampleIndex < 0)
        {
            currentSampleIndex = 0;
            samplesPerRow = numSamples;
        }
        auto samples = src->GetSamples(currentSampleIndex, samplesPerRow);
        samplesPerRow  = samples.get()->size();
        vf samplesWindowOrg(samples.get()->begin(), samples.get()->end());           
        samplingRate = src->Rate(); 
        float f = static_cast<float>(freqShift * piOver180 / samplingRate);
        float p = static_cast<float>(phaseAngle * M_PI/180.0f);
        processing::filter::FrequencyShiftTheta(samplesWindowOrg, samplesWindowReal, f, p);           
    } //End Update float


    void Scatterplot::UpdateData(std::shared_ptr<SampleSource<std::complex<float>>> src, int &currentSampleIndex)
    {
        //Need code below in case plot is initialized with SPR>numSamples and
        //the code in Helper is not called        
        if(currentSampleIndex+samplesPerRow >= numSamples)
            currentSampleIndex = numSamples-samplesPerRow;
        if(currentSampleIndex < 0)
        {
            currentSampleIndex = 0;
            samplesPerRow = numSamples;
        }
        auto samples = src->GetSamples(currentSampleIndex, samplesPerRow);
        samplesPerRow  = samples.get()->size();
        vcf samplesWindowOrg(samples.get()->begin(), samples.get()->end());           
        samplingRate = src->Rate(); 
        //float f = static_cast<float>(freqShift * piOver180 / samplingRate);
        float f = static_cast<float>(2*float(M_PI)*freqShift / samplingRate);
        float p = static_cast<float>(phaseAngle * M_PI/180.0f);
        processing::filter::FrequencyShiftTheta(samplesWindowOrg, samplesWindowComplex, f, p);
    } //End Update complex<float>

    void Scatterplot::ShowControls(int &currentSampleIndex, Markers &markers, bool &updatePlots)
    {
        ImGui::BeginChild("ControlChild");//, ImVec2(ImGui::GetWindowContentRegionWidth() * 0.2f, -1));
        ImGui::Text("Display parameters");
        ImGui::Text("Start sample");                
        if(gui::InputSampleIndex(currentSampleIndex, samplesPerRow, numSamples))
            updatePlots = true;
        ImGui::Text("Samples per row");        
        if(gui::InputSamplesPerRow(currentSampleIndex, samplesPerRow, numSamples))        
            updatePlots = true;                             
        ImGui::Separator();
        ImGui::Text("Scatter view");
        ImGui::Text("Decimation");
        if(ImGui::InputInt("##decimation", &decimation))
        {
            //updatePlots = true;            
            updateDecimation = true;
        }
        ImGui::Text("Num rows");
        int prevNumRows = numRows;
        if(ImGui::InputInt("##rows", &numRows))
        {            
            if(numRows*numCols>decimation)
                numRows = prevNumRows;
            //updatePlots = true;
        }
        ImGui::Text("Num cols");
        int prevnumCols = numCols;
        if(ImGui::InputInt("##cols", &numCols))
        {            
            if(numRows*numCols>decimation)
                numCols = prevnumCols;
        }
        while(numCols*numRows > decimation)
        {
            numCols--;
            numRows--;
        }
        if(numCols<1)
            numCols = 1;
        if(numRows<1)
            numRows = 1;
        ImGui::Checkbox("Scaling", &scale);
        if(scale)
        {
            ImGui::SameLine();
            if(ImGui::InputDouble("##scale", &scaleDouble))
            {
                xMin = -scaleDouble;
                xMax = scaleDouble;
                yMin = -scaleDouble;
                yMax = scaleDouble;
            }            
            setAxisFit = false;
        } else
            setAxisFit = true;
        ImGui::Text("Point size");
        if(ImGui::InputInt("##point", &pointSizeInt))
        {
            if(pointSizeInt<1)
                pointSizeInt = 1;
            pointSizeDouble = static_cast<double>(pointSizeInt);
        }
        ImGui::Checkbox("Subplot index", &showSubplotIndex);
        ImGui::Separator();
        ImGui::Text("Frequency [Hz]");
        ImGui::SameLine();
        if(ImGui::InputDouble("##freqshift", &freqShift))
            updatePlots = true;
        ImGui::Text("Phase angle [deg]");
        ImGui::SameLine();
        if(ImGui::InputDouble("##phaseangle", &phaseAngle))
            updatePlots = true;
        ImGui::Separator();
        ImGui::Text("Markers");
        ImGui::Checkbox("X markers", &markers.displayXMarkers);
        if(ImGui::Button("Reset x markers"))
        {
            markers.XmarkerTD0 = axis.X.Min+10;
            markers.XmarkerTD1 = axis.X.Max-20;                    
        }
        ImGui::Checkbox("Y markers", &markers.displayYMarkers);                
        if(ImGui::Button("Reset y markers"))
        {
            markers.YmarkerTD0 = axis.Y.Min+10;
            markers.YmarkerTD1 = axis.Y.Max-10;                    
        }
        ImGui::EndChild();
    }

}