#include "PowerSpectrum.h"

namespace gui
{

    PowerSpectrum::PowerSpectrum()
    {
        samplesPerRow = 8192;   
    }

    PowerSpectrum::~PowerSpectrum()
    {
        
    }

    void PowerSpectrum::Update(std::shared_ptr<AbstractSampleSource> src, int &currentSampleIndex, Markers &markers, bool &updatePlots)
    {
        numSamples = static_cast<int>(src->Count());

        //ImGui::SameLine();
        //ImGui::BeginChild("ChildR", ImVec2(-1, -1));                                  
        
        if(currentSampleIndex+samplesPerRow>numSamples)             
        {
            currentSampleIndex = numSamples-samplesPerRow;                                                            
        }

        //Update plot data
        if(updatePlots)
        {
            if(!src->RealSignal())         
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
            if(markers.xPeriodic) updatePeriodicXVals = true;
        }
        if(updatePeriodicXVals)
        {
            numPeriodicXVals = static_cast<int>((binIndices[binIndices.size()-1]-binIndices[0])/markers.xPeriod)+1;
            periodicXVals.resize(numPeriodicXVals);
            double initVal = markers.xPeriodicReference;
            while(initVal-markers.xPeriod > binIndices[0])
                initVal -= markers.xPeriod;
            while(initVal < binIndices[0])
                initVal += markers.xPeriod;
            for (size_t i = 0; i < numPeriodicXVals; i++)
            {
                periodicXVals[i] = initVal+i*markers.xPeriod;
            }
            updatePeriodicXVals = false;
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

    void PowerSpectrum::PlotData(int &currentSampleIndex, Markers &markers, bool &updatePlots)
    {
        if(setAxisFit)
            ImPlot::SetNextAxesToFit();  
        ImGui::BeginChild("PlotData");                 
        if(ImPlot::BeginPlot("##psdplot", ImVec2(-1,0.9f*ImGui::GetWindowHeight())))
        { 

            /*if(setAxisFit)
            {
                ImPlot::SetupAxisLimits(ImAxis_X1, binIndices[0], binIndices[binIndices.size()-1], ImPlotCond_Always);//, ImPlotCond_Once);                                
              
            } */     
            if(transformationPower>-3)                                          
                ImPlot::SetupAxes("Frequency [Hz]","Power"); 
            else    
                ImPlot::SetupAxes("Samples","Power"); 
            //ImPlot::SetupAxisFormat(ImAxis_X1, "%.0f");
            if(markers.displayXMarkers)
            {
                ImPlot::DragLineX(markers.XmarkerTD0ID,&markers.XmarkerTD0,ImVec4(1,0,0,0.5f),0.5f,ImPlotDragToolFlags_None);
                ImPlot::TagX(markers.XmarkerTD0, ImVec4(1,0,0,1), "M1");
                ImPlot::DragLineX(markers.XmarkerTD1ID,&markers.XmarkerTD1,ImVec4(1,0,0,0.5f),0.5f,ImPlotDragToolFlags_None);
                ImPlot::TagX(markers.XmarkerTD1, ImVec4(1,0,0,1), "M2");
            }   
            if(markers.xPeriodic)
            {
                ImPlot::SetNextLineStyle(ImVec4(0.3f, 0.7f, 0.2f, 0.700f), 1.0f);  
                ImPlot::PlotInfLines("##rv", periodicXVals.data(), numPeriodicXVals);
            }
            if(markers.displayYMarkers)
            {
                ImPlot::DragLineY(markers.YmarkerTD0ID,&markers.YmarkerTD0,ImVec4(1,0,0,0.5f),0.5f,ImPlotDragToolFlags_None);
                ImPlot::TagY(markers.YmarkerTD0, ImVec4(1,0,0,1), "M1");
                ImPlot::DragLineY(markers.YmarkerTD1ID,&markers.YmarkerTD1,ImVec4(1,0,0,0.5f),0.5f,ImPlotDragToolFlags_None);
                ImPlot::TagY(markers.YmarkerTD1, ImVec4(1,0,0,1), "M2");
            }                  
            ImPlot::SetNextLineStyle(ImVec4(0.409f, 0.510f, 0.835f, 1.000f), 1.0f);     
            ImPlot::PlotLine("", binIndices.data(), powerWindow.data(), powerWindow.size());//,ImPlotLineFlags_None, 0, sizeof(float));                                                
            axis = ImPlot::GetPlotLimits();                                
            ImPlot::EndPlot();
        }         
        setAxisFit = false;
        if(numSamples>0)
        {
            ImGui::SetNextItemWidth(-1);
            if(ImGui::SliderInt("##sampleSlider", &currentSampleIndex, 0, numSamples))
            {
                setAxisFit = true;
                updatePlots = true;
            }
                
        }                                          
        ImGui::EndChild();
    }

    void PowerSpectrum::UpdateData(std::shared_ptr<SampleSource<float>> src, int &currentSampleIndex)
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
        
        //Update plots
        auto samples = src->GetSamples(currentSampleIndex, samplesPerRow);
        samplesPerRow = samples.get()->size();
        samplingRate = src->Rate();
        vf samplesWindow(samples.get()->begin(), samples.get()->end());            
        if(transformationPower>0)
        {
            processing::transforms::PowerSpectrumWindowLog(samplesWindow, powerWindow, windowLength, fftSize, upsampleInt, transformationPower, windowType);
            binIndices.resize(powerWindow.size());   
            float hzPerBin = upsampleInt*samplingRate/powerWindow.size();                    
            for (int i = 0; i < powerWindow.size()/2; i++)
            {
                binIndices[i] = i*hzPerBin+hzPerBin/2;
            }
            vf powerWindowTemp(powerWindow.begin()+powerWindow.size()/2, powerWindow.end());
            powerWindow = powerWindowTemp;
        } else if (transformationPower == -1 || transformationPower == -2)
        {
            vf transformedData;
            if(transformationPower == -1)
                processing::transforms::AMDetect(samplesWindow, transformedData);
            else if(transformationPower == -2)
                processing::transforms::FMDetect(samplesWindow, transformedData);                                                                        
            processing::transforms::PowerSpectrumWindowLog(transformedData, powerWindow, windowLength, fftSize, upsampleInt, 1, windowType);                
            binIndices.resize(powerWindow.size()/2);   
            float hzPerBin = upsampleInt*samplingRate/powerWindow.size();                                
            for (int i = 0; i < powerWindow.size()/2; i++)
            {
                binIndices[i] = i*hzPerBin+hzPerBin;
            }                
            vf powerWindowTemp(powerWindow.begin()+powerWindow.size()/2, powerWindow.end());
            powerWindow = powerWindowTemp;
        } else if(transformationPower == -3)
        {
            processing::transforms::autoCorrFFT(samplesWindow, powerWindow, fftSize);
            binIndices.resize(powerWindow.size());                   
            for (int i = 0; i < powerWindow.size(); i++)
            {
                binIndices[i] = i;
            }
        }else if (transformationPower == -4)
        {
            vf transformedData, powerWindowTemp;
            processing::transforms::autoCorrTD(samplesWindow, transformedData, delay);
            processing::transforms::autoCorrFFT(transformedData, powerWindowTemp, fftSize);
            binIndices.resize(powerWindowTemp.size());  
            powerWindow.resize(powerWindowTemp.size());
            //Perform moving average
            for (int i = 0; i < powerWindow.size(); i++)
            {
                binIndices[i] = i;
                powerWindow[i] = powerWindowTemp[i];
                if(i>2 && i < powerWindow.size()-2)
                {
                    float acc = 0.0f;
                    for (size_t j = 0; j < 4; j++)
                    {
                        acc += powerWindowTemp[i-2+j];
                    }
                    powerWindow[i] = acc/4.0f;
                }
            }
        }else if (transformationPower == -5)
        {
            vf transformedData;
            processing::transforms::autoCorrTD(samplesWindow, transformedData, fftSize);
            processing::transforms::Integrate(transformedData, powerWindow, fftSize, delay);
            binIndices.resize(powerWindow.size());                   
            for (int i = 0; i < powerWindow.size(); i++)
            {
                binIndices[i] = i;
            }
        }
        if(setAxisFitAfterUpdate) setAxisFit = true;
    } //End UpdateData float


    void PowerSpectrum::UpdateData(std::shared_ptr<SampleSource<std::complex<float>>> src, int &currentSampleIndex)
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
        samplesPerRow = samples.get()->size();
        vcf samplesWindow(samples.get()->begin(), samples.get()->end());           
        samplingRate = src->Rate(); 
        if(transformationPower>0)
        {
            processing::transforms::PowerSpectrumWindowLog(samplesWindow, powerWindow, windowLength, fftSize, upsampleInt, transformationPower, windowType);
            binIndices.resize(powerWindow.size());   
            float hzPerBin = upsampleInt*samplingRate/powerWindow.size();                    
            for (int i = 0; i < powerWindow.size(); i++)
            {
                binIndices[i] = -upsampleInt*samplingRate/2+i*hzPerBin+hzPerBin/2;
            }
        } else if(transformationPower == -1 || transformationPower == -2)
        {
            vf transformedData;
            if(transformationPower == -1)
                processing::transforms::AMDetect(samplesWindow, transformedData);
            else if(transformationPower == -2)
                processing::transforms::FMDetect(samplesWindow, transformedData);                                                     
            processing::transforms::PowerSpectrumWindowLog(transformedData, powerWindow, windowLength, fftSize, upsampleInt, 1, windowType);                
            binIndices.resize(powerWindow.size()/2);   
            float hzPerBin = upsampleInt*samplingRate/powerWindow.size();                                
            for (int i = 0; i < powerWindow.size()/2; i++)
            {
                binIndices[i] = i*hzPerBin+hzPerBin;
            }
            vf powerWindowTemp(powerWindow.begin()+powerWindow.size()/2, powerWindow.end());
            powerWindow = powerWindowTemp;
        } else if (transformationPower == -3)
        {
            processing::transforms::autoCorrFFT(samplesWindow, powerWindow, fftSize);
            binIndices.resize(powerWindow.size());                   
            for (int i = 0; i < powerWindow.size(); i++)
            {
                binIndices[i] = i;
            }
        } else if (transformationPower == -4)
        {
            vcf transformedData;
            vf powerWindowTemp;
            processing::transforms::autoCorrTD(samplesWindow, transformedData, delay);
            processing::transforms::autoCorrFFT(transformedData, powerWindowTemp, fftSize);
            binIndices.resize(powerWindowTemp.size());  
            powerWindow.resize(powerWindowTemp.size());
            //Perform moving average
            for (int i = 0; i < powerWindow.size(); i++)
            {
                binIndices[i] = i;
                powerWindow[i] = powerWindowTemp[i];
                if(i>2 && i < powerWindow.size()-2)
                {
                    float acc = 0.0f;
                    for (size_t j = 0; j < 4; j++)
                    {
                        acc += powerWindowTemp[i-2+j];
                    }
                    powerWindow[i] = acc/4.0f;
                }
            }
        } else if (transformationPower == -5)
        {
            vcf transformedData;
            processing::transforms::autoCorrTD(samplesWindow, transformedData, fftSize);
            processing::transforms::Integrate(transformedData, powerWindow, fftSize, delay);
            processing::transforms::Normalize(powerWindow);
            binIndices.resize(powerWindow.size());                   
            for (int i = 0; i < powerWindow.size(); i++)
            {
                binIndices[i] = i;
            }
        }
        if(setAxisFitAfterUpdate) setAxisFit = true;
    } //End UpdateData complex<float>

    void PowerSpectrum::ShowControls(int &currentSampleIndex, Markers &markers, bool &updatePlots)
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
        ImGui::Text("Spectral parameters");
        ImGui::Text("FFT length");
        if (ImGui::Combo("##fftsize", &fftSizeIndex, "64\0""128\0""256\0""512\0""1024\0""2048\0""4096\0""8192\0""16384\0")) {                    
            fftSize = fftSizesList[fftSizeIndex];    
            windowLength = fftSize;                
            setAxisFitAfterUpdate = true;
            updatePlots = true;
        }
        ImGui::Text("Window type");
        if (ImGui::Combo("##windowtypePSD", &windowTypeIndex, "Hamming\0""Hann\0""BlackmanHarris4\0""BlackmanHarris7\0""Kaiser\0""Flattop\0""Triangular\0""RCosTaper\0")) {                    
            windowType = windowTypeList[windowTypeIndex];                    
            setAxisFitAfterUpdate = true;
            updatePlots = true;
        }        
        ImGui::Text("Transformation parameters");
        ImGui::Text("Type");
        if (ImGui::Combo("##power", &powerTransformIndex, "1\0""2\0""4\0""8\0""AM\0""FM\0""AutoCorr\0""DelayAndCorr\0""DelayAndIntegrate\0")) {                    
            transformationPower = powerTransformList[powerTransformIndex];
            setAxisFitAfterUpdate = true;
            updatePlots = true;
            if(transformationPower > 0)
            {
                upsampleInt = transformationPower;
                windowLength = fftSize;
            }
            else if(transformationPower == -1 || transformationPower == -2)
            {
                windowLength = fftSize;
                upsampleInt = 2;
            }
        }
        ImGui::Checkbox("Upsample", &upsampleBool);
        ImGui::SameLine();
        ImGui::BeginDisabled(!upsampleBool);
        int prevUpsampleInt = upsampleInt;
        ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth()*0.4f);        
        ImGui::InputInt("##upsampleInt", &upsampleInt);
        if(prevUpsampleInt != upsampleInt)
        {
            setAxisFitAfterUpdate = true;
            updatePlots = true;
        }            
        if(upsampleInt<1)
            upsampleInt = 1;
        ImGui::EndDisabled();
        ImGui::Checkbox("Win len", &windowBool);
        ImGui::SameLine();
        ImGui::BeginDisabled(!windowBool);
        int prevWindowLength = windowLength;
        ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth()*0.4f);
        ImGui::InputInt("##winlen", &windowLength);
        if(prevWindowLength != windowLength)
            updatePlots = true;
        ImGui::EndDisabled();
        if(windowLength > fftSize)
            windowLength = fftSize;
        if(windowLength<=0)
            windowLength = fftSize;
        if(transformationPower < -3)
        {            
            ImGui::Text("Delay");
            if(ImGui::InputInt("##delay", &delay))
            {
                updatePlots = true;
                setAxisFitAfterUpdate = true;
            }
        }
        if(delay < 0)
            delay = 0;
        //if(delay > fftSize)
        //    delay = fftSize;
        ImGui::Separator();
        ImGui::Text("Markers");
        ImGui::Checkbox("X markers", &markers.displayXMarkers);
        if(ImGui::Checkbox("Periodic", &markers.xPeriodic))
            updatePeriodicXVals = true;
        if(!markers.xPeriodic)
            ImGui::BeginDisabled();
        ImGui::Text("Period"); ImGui::SameLine();
        if(ImGui::InputFloat("##xperiod", &markers.xPeriod))
            updatePeriodicXVals = true;
        ImGui::Text("Reference"); ImGui::SameLine();
        if(ImGui::InputDouble("##xreference", &markers.xPeriodicReference))
            updatePeriodicXVals = true;
        if(!markers.xPeriodic)
            ImGui::EndDisabled();  
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