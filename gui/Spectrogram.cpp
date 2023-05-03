#include "Spectrogram.h"

namespace gui
{

    Spectrogram::Spectrogram()
    {
        /*
        std::string filename = "/home/user/wisdom_fftwf.cfg";
        int res = fftwf_import_wisdom_from_filename(filename.c_str());
        if(res)
            std::cout << "Read wisdom succesfully" << std::endl;
        else
            std::cout << "Could not read wisdom file "  << std::endl;
        */
        fftSizesList = {64,128,256,512,1024,2048,4096,8192,16384};
        for (size_t i = 0; i < fftSizesList.size(); i++)
        {            
            fftwf_plan plan = fftwf_plan_dft_1d(fftSizesList[i], fftForwardIn, fftForwardOut, FFTW_FORWARD, FFTW_ESTIMATE);
            fftwForwardPlans.push_back(plan);
        }
        samplesPerRow = 8192;
    }

    Spectrogram::~Spectrogram()
    {
        for (auto plan : fftwForwardPlans)
        {
             fftwf_destroy_plan(plan);
        }
        fftwf_free(fftForwardIn);
        fftwf_free(fftForwardOut);
        fftwf_cleanup();
    }

    void Spectrogram::Update(std::shared_ptr<AbstractSampleSource> src, int &currentSampleIndex, Markers &markers, bool &updatePlots)
    {
        numSamples = static_cast<int>(src->Count());                

        static ImGuiTableFlags flags = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable;// | ImGuiTableFlags_RowBg;// | ImGuiTableFlags_NoBordersInBody;
        if (ImGui::BeginTable("", 2, flags))
        {
            ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthFixed, ImGui::GetWindowContentRegionWidth()* 0.2f);
            ImGui::TableSetupColumn("Plot", ImGuiTableColumnFlags_NoHide);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ShowControls(currentSampleIndex, markers, updatePlots); 
            //Update plot data
            if(updatePlots)
            {
                if(!src->RealSignal())         
                {
                    realFactor = 1;
                    std::shared_ptr<SampleSource<std::complex<float>>> concrete = std::dynamic_pointer_cast<SampleSource<std::complex<float>>>(src);
                    UpdateData(concrete, currentSampleIndex);
                    updatePlots = false;
                } else
                {
                    realFactor = 0.5;
                    std::shared_ptr<SampleSource<float>> concrete = std::dynamic_pointer_cast<SampleSource<float>>(src);
                    UpdateData(concrete, currentSampleIndex);
                    updatePlots = false;
                }
                if(markers.xPeriodic) updatePeriodicXVals = true;
            }
            if(updateDownsampling)
            {
                samplesPerRowPlot = samplesPerRowPlotMax;
                float samplesPerBin = (samplesPerRow)/samplesPerRowPlot;
                std::vector<processing::utils::ExamplePoint> spectrogramDownsampled;
                processing::utils::PointLttb::Downsample(spectrogram2D.begin(), spectrogram2D.size(), std::back_insert_iterator<std::vector<processing::utils::ExamplePoint>>(spectrogramDownsampled), fftSize*samplesPerRowPlot*realFactor);
                spectrogram.resize(fftSize*samplesPerRowPlot*realFactor);
                for (size_t i = 0; i < fftSize*samplesPerRowPlot*realFactor; i++)
                {
                    float y = spectrogramDownsampled[i].y;
                    spectrogram[i] = spectrogramDownsampled[i].y;
                }
            
                updateDownsampling = false;
            }
            if(updatePeriodicXVals)
            {
                numPeriodicXVals = static_cast<int>(samplesPerRow/markers.xPeriod)+1;
                periodicXVals.resize(numPeriodicXVals);
                int numPeriodDiff = static_cast<int>((markers.xPeriodicReference-currentSampleIndex)/markers.xPeriod);
                double initVal = markers.xPeriodicReference-numPeriodDiff*markers.xPeriod;
                while(initVal<0)
                    initVal +=markers.xPeriod;
                for (size_t i = 0; i < numPeriodicXVals; i++)
                {
                    periodicXVals[i] = initVal+i*markers.xPeriod;
                }
                updatePeriodicXVals = false;
            }

            xAxisMin = currentSampleIndex;
            xAxisMax = currentSampleIndex+samplesPerRow;                
            ImGui::TableNextColumn();
            PlotData(currentSampleIndex, markers, updatePlots);
            ImGui::EndTable();
        }
        
    }

    void Spectrogram::PlotData(int &currentSampleIndex, Markers &markers, bool &updatePlots)
    {
        ImPlot::PushColormap(map);                   
        static ImPlotHeatmapFlags hm_flags = 0;
        ImGui::BeginChild("PlotData");
        if(ImPlot::BeginPlot("##Heatmap1", ImVec2(-1,0.9f*ImGui::GetWindowHeight())))
        {                            
            if(setXAxis) 
            {
               ImPlot::SetupAxisLimits(ImAxis_X1,axis.X.Min,axis.X.Max,ImGuiCond_Always);
               setXAxis = false;
            }
            if(setYAxis)
            {
                ImPlot::SetupAxisLimits(ImAxis_Y1,axis.Y.Min,axis.Y.Max,ImGuiCond_Always);
                setYAxis = false;
            }           
            ImPlot::SetupAxisFormat(ImAxis_X1, "%.0f");
                
            ImPlotPoint p1 {static_cast<double>(xAxisMin),yMin};
            ImPlotPoint p2 {static_cast<double>(xAxisMax),yMax};
            ImPlot::PlotHeatmap("##spectrogram", spectrogram.data(), fftSize*realFactor, samplesPerRowPlot, yMinPSD, yMaxPSD, NULL,p1,p2,hm_flags);
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
            axis = ImPlot::GetPlotLimits();                     
            if(axis.X.Min<currentSampleIndex || axis.X.Max > currentSampleIndex+samplesPerRow)
            {
                updatePlots = true;
                currentSampleIndex = axis.X.Min;
                if(axis.X.Min<0)
                {
                    currentSampleIndex = 0;
                    axis.X.Min = 0;
                    setXAxis = true;
                }                   
            }                        
            if(axis.X.Max>=numSamples-windowLength)
            {
                axis.X.Max = numSamples-1-windowLength; 
                setXAxis = true;
            } 
            if(axis.Y.Min<yMin)
            {
                axis.Y.Min = yMin;
                setYAxis = true;
            }
            if(axis.Y.Max>yMax)
            {
                axis.Y.Max = yMax;
                setYAxis = true;
            }
            samplesPerRow = axis.X.Max-axis.X.Min;             
            ImPlot::EndPlot();
        }           
                
        ImGui::SetNextItemWidth(-1);
        if(ImGui::SliderInt("##sampleSlider", &currentSampleIndex, 0, numSamples-samplesPerRow-windowLength))
        {
            setXAxis = true;                       
            setYAxis = true;
            updatePlots = true;            
        }
        ImGui::EndChild();
        //usleep(1000);
    }

    void Spectrogram::UpdateData(std::shared_ptr<SampleSource<float>> src, int &currentSampleIndex)
    {
        int miniPSDFFTSize = 256;    
        auto samples = src->GetSamples(currentSampleIndex, miniPSDFFTSize);
        samplingRate = src->Rate();
        vf samplesWindow(samples.get()->begin(), samples.get()->end());            
        processing::transforms::PowerSpectrumWindowLog(samplesWindow, miniPowerWindow, miniPSDFFTSize, miniPSDFFTSize, 1, 1, WINDOW_HAMMING);
        miniBinIndices.resize(miniPSDFFTSize/2);   
        float hzPerBin = samplingRate/miniPSDFFTSize;                    
        for (int i = 0; i < miniPSDFFTSize/2; i++)
        {
            miniBinIndices[i] = i*hzPerBin+hzPerBin/2;
            if(initYMinMax)
            {
                if(miniPowerWindow[miniPSDFFTSize/2+i] > yMaxPSD)
                    yMaxPSD = miniPowerWindow[miniPSDFFTSize/2+i];
                if(miniPowerWindow[miniPSDFFTSize/2+i] < yMinPSD)
                    yMinPSD = miniPowerWindow[miniPSDFFTSize/2+i];
            }
        }    
        initYMinMax = false;               
        vf miniTemp(miniPowerWindow.begin()+miniPSDFFTSize/2,miniPowerWindow.end());
        miniPowerWindow = miniTemp;
        int numFFTs = samplesPerRow/windowShift;
        if(numFFTs < 1)
            numFFTs = 1;
        spectrogram.resize(numFFTs*fftSize/2);
        spectrogram2D.resize(numFFTs*fftSize/2);
        //Setup windowing taps        
        std::vector<float> windowTaps(windowLength);
        SetupWindow(windowType, windowLength, windowTaps); 
        for (int i = 0; i < numFFTs; i++)
        {            
            auto samples = src->GetSamples(currentSampleIndex+i*windowShift,windowLength);
            vcf samplesWindow(samples.get()->begin(), samples.get()->end());
            processing::transforms::SpectrogramFFT(samplesWindow, powerWindow, windowLength, fftSize, windowType, fftwForwardPlans[fftSizeIndex], fftForwardIn, fftForwardOut, windowTaps);
            int idx = 0;
            for (int j = 0; j < fftSize/2; j++)
            {                
                idx = (fftSize/2-1-j)*numFFTs+i;
                spectrogram[idx] = powerWindow[fftSize/2+j];                    
                spectrogram2D[idx].x = static_cast<float>(idx);
                spectrogram2D[idx].y = powerWindow[fftSize/2+j];                
            }            
        }
                    
        if(numFFTs>samplesPerRowPlotMax)       
        {
            updateDownsampling = true;   
        } else
        {
            samplesPerRowPlot = numFFTs;                       
        }

        //Setup y axis limits                                
        hzPerBin = samplingRate/fftSize;                    
        yMin = hzPerBin/2;
        yMax = (fftSize/2-1)*hzPerBin + hzPerBin/2;        
        if(setYAxis)
        {
            axis.Y.Min = yMin;
            axis.Y.Max = yMax;
        }                                                                                                                         
    } //End UpdateData float

    void Spectrogram::UpdateData(std::shared_ptr<SampleSource<std::complex<float>>> src, int &currentSampleIndex)
    {        
        //Mini power spectrum window 
        int miniPSDFFTSize = 256;                
        auto miniSamples = src->GetSamples(currentSampleIndex, miniPSDFFTSize);
        samplingRate = src->Rate();
        vcf miniSamplesWindow(miniSamples.get()->begin(), miniSamples.get()->end());            
        processing::transforms::PowerSpectrumWindowLog(miniSamplesWindow, miniPowerWindow, miniPSDFFTSize, miniPSDFFTSize, 1, 1, WINDOW_HAMMING);
        miniBinIndices.resize(miniPowerWindow.size());   
        float hzPerBin = samplingRate/miniPowerWindow.size();                    
        for (int i = 0; i < miniPowerWindow.size(); i++)
        {
            miniBinIndices[i] = -samplingRate/2+i*hzPerBin+hzPerBin/2;
            if(initYMinMax)
            {
                if(miniPowerWindow[i] > yMaxPSD)
                    yMaxPSD = miniPowerWindow[i];
                if(miniPowerWindow[i] < yMinPSD)
                    yMinPSD = miniPowerWindow[i];   
            }
        } 
        initYMinMax = false;
        //Spectrogram
        int numFFTs = samplesPerRow/windowShift;
        if(numFFTs < 1)
            numFFTs = 1;         
        spectrogram.resize(numFFTs*fftSize);
        spectrogram2D.resize(numFFTs*fftSize);
        //Setup windowing taps        
        std::vector<float> windowTaps(windowLength);
        SetupWindow(windowType, windowLength, windowTaps); 
        vf powerWindow(fftSize,0);
        //auto t1 = high_resolution_clock::now();
        for (int i = 0; i < numFFTs; i++)
        {            
            auto samples = src->GetSamples(currentSampleIndex+i*windowShift,windowLength);
            vcf samplesWindow(samples.get()->begin(), samples.get()->end());  
            processing::transforms::SpectrogramFFT(samplesWindow, powerWindow, windowLength, fftSize, windowType, fftwForwardPlans[fftSizeIndex], fftForwardIn, fftForwardOut, windowTaps);            
            int idx = 0;
            for (int j = 0; j < fftSize; j++)
            {                
                idx = (fftSize-1-j)*numFFTs+i;
                spectrogram[idx] = powerWindow[j];                    
                spectrogram2D[idx].x = static_cast<float>(idx);
                spectrogram2D[idx].y = powerWindow[j];             
            }            
        }
        //auto t2 = high_resolution_clock::now();
        //auto ms_int = duration_cast<milliseconds>(t2 - t1);
        //std::cout << "FFTs took " <<  ms_int.count() << " milliseconds" << std::endl;
        //Downsample            
        if(numFFTs>samplesPerRowPlotMax)       
        {
            updateDownsampling = true;   
        } else
        {
            samplesPerRowPlot = numFFTs;                      
        }

        //Setup y axis limits                                
        hzPerBin = samplingRate/fftSize;                    
        yMin = -samplingRate/2 + hzPerBin/2;
        yMax = -samplingRate/2 + (fftSize-1)*hzPerBin + hzPerBin/2;        
        if(setYAxis)
        {
            axis.Y.Min = yMin;
            axis.Y.Max = yMax;
        }                                                                                                                           
    } //End UpdateData complex<float>

    void Spectrogram::ShowControls(int &currentSampleIndex, Markers &markers, bool &updatePlots)
    {        
        ImGui::BeginChild("ControlChild");//, ImVec2(ImGui::GetWindowContentRegionWidth() * 0.2f, -1));
        //ImGui::Text("Display parameters");
        ImGui::Text("Start sample");        
        ImGui::SetNextItemWidth(180);                 
        //if(ImGui::InputInt("##currentsampleindex", &currentSampleIndex))
        currentSampleIndexStr = std::to_string(currentSampleIndex);
        if(gui::InputStrAndEvaluateToInt("##spsampleIndexStr",currentSampleIndexStr))
        {         
            currentSampleIndex = std::stoi(currentSampleIndexStr);
            //Check if we went out-of-bounds with out expression
            if(currentSampleIndex < 0)
                    currentSampleIndex = 0;            
            axis.X.Min = currentSampleIndex;
            axis.X.Max = axis.X.Min+samplesPerRow;   
            updatePlots = true;        
            setXAxis = true;
        }            
        ImGui::Text("Samples per row");   
        ImGui::SetNextItemWidth(180);                      
        //if(ImGui::InputInt("##numsamplesperrow", &samplesPerRow))
        samplesPerRowStr = std::to_string(samplesPerRow);
        if(gui::InputStrAndEvaluateToInt("##spsamplesPerRowStr",samplesPerRowStr))
        {
            samplesPerRow = std::stoi(samplesPerRowStr);
            if(samplesPerRow<1)
                samplesPerRow = 1; 
            
            updatePlots = true;                         
            axis.X.Max = axis.X.Min+samplesPerRow;
            setXAxis = true;
            //windowShift = samplesPerRow/(2*fftSize*10);                           
        }  
        //Need code below in case plot is initialized with SPR>numSamples and
        //the code in Helper is not called        
        if(currentSampleIndex+samplesPerRow+windowLength >= numSamples)
            currentSampleIndex = numSamples-samplesPerRow-windowLength;
        if(currentSampleIndex < 0)
        {
            currentSampleIndex = 0;
            samplesPerRow = numSamples-windowLength-1;
        } 
        axis.X.Min = currentSampleIndex;
        axis.X.Max = axis.X.Min+samplesPerRow;   
        ImGui::Separator();
        ImGui::Text("Samples after downsampling");  
        if(ImGui::InputInt("##numsamplesafterdown", &samplesPerRowPlotMax))
        {
            if(samplesPerRowPlotMax<10)
                samplesPerRowPlotMax = 10;
            updateDownsampling = true;
        }
        ImGui::Text("Spectral parameters");
        ImGui::Text("FFT length");
        if (ImGui::Combo("##fftsize", &fftSizeIndex, "64\0""128\0""256\0""512\0""1024\0""2048\0""4096\0""8192\0""16384\0")) {                    
            fftSize = fftSizesList[fftSizeIndex];                    
            setXAxis = true;
            setYAxis = true;
            updatePlots = true;
            windowShift = fftSize;
            windowLength = fftSize;
        } 
        ImGui::Text("Window type");
        if (ImGui::Combo("##windowtypeSP", &windowTypeIndex, "Hamming\0""Hann\0""BlackmanHarris4\0""BlackmanHarris7\0""Kaiser\0""Flattop\0""Triangular\0""RCosTaper\0")) {                    
            windowType = windowTypeList[windowTypeIndex];                    
            setXAxis = true;
            setYAxis = true;
            updatePlots = true;
        }            
        ImGui::Text("Window length");
        if(ImGui::InputInt("##windowlength", &windowLength))
            updatePlots = true;
        if(windowLength>fftSize)
            windowLength = fftSize;
        ImGui::Text("Window shift");
        ImGui::InputInt("##windowshift", &windowShift);
            //updatePlots = true;
        if(windowShift < 1)
            windowShift = 1;
        //if(strideLength > fftSize)
        //    strideLength = fftSize;
        if(ImGui::Button("Resize Y"))
        {
            setYAxis = true;
            axis.Y.Min = yMin;
            axis.Y.Max = yMax;
        }
        ImGui::Text("Markers");
        ImGui::Checkbox("X markers", &markers.displayXMarkers);
        if(ImGui::Checkbox("Periodic", &markers.xPeriodic))
            updatePeriodicXVals = true;
        if(!markers.xPeriodic)
            ImGui::BeginDisabled();
        ImGui::Text("Period       "); ImGui::SameLine();        
        xPeriodStr = std::to_string(markers.xPeriod);
        if(gui::InputStrAndEvaluateToDouble("##xperiod", xPeriodStr))
        {
            markers.xPeriod = std::stof(xPeriodStr);
            if(markers.xPeriod <= 0)
                markers.xPeriod = 1;
            updatePeriodicXVals = true;
        }
            
        ImGui::Text("Reference"); ImGui::SameLine();        
        xReferenceStr = std::to_string(markers.xPeriodicReference);
        if(gui::InputStrAndEvaluateToDouble("##xPeriodRef", xReferenceStr))
        {
            markers.xPeriodicReference = std::stod(xReferenceStr);
            if(markers.xPeriodicReference < 0)
                markers.xPeriodicReference = 0;
            updatePeriodicXVals = true;
        }
        if(!markers.xPeriodic)
            ImGui::EndDisabled();  
        if(ImGui::Button("Reset x markers"))
        {
            markers.XmarkerTD0 = axis.X.Min+10;
            markers.XmarkerTD1 = markers.XmarkerTD0+samplesPerRow/4;    
            markers.xPeriodicReference = currentSampleIndex;
            updatePeriodicXVals = true;                
        }
        ImGui::Checkbox("Y markers", &markers.displayYMarkers);                
        if(ImGui::Button("Reset y markers"))
        {
            markers.YmarkerTD0 = axis.Y.Min+10;
            markers.YmarkerTD1 = axis.Y.Max-10;                    
        }
        //map = ImPlotColormap_Greys;
        if (ImPlot::ColormapButton(ImPlot::GetColormapName(map),ImVec2(225,0),map)) {
            map = (map + 1) % ImPlot::GetColormapCount();
            // We bust the color cache of our plots so that item colors will
            // resample the new colormap in the event that they have already
            // been created. See documentation in implot.h.
            ImPlot::BustColorCache("##Heatmap1");
            //BustColorCache("##Heatmap2");
        }

        ImPlot::SetNextAxesToFit();
        //Do plotting
         //Mini power spectrum window          
        if(ImPlot::BeginPlot("##minipsd"))
        {                                                                
            ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);   
            ImPlot::DragLineY(0,&yMinPSD,ImVec4(1,1,1,1),1,ImPlotDragToolFlags_None);
            ImPlot::DragLineY(1,&yMaxPSD,ImVec4(1,1,1,1),1,ImPlotDragToolFlags_None);                                                       
            ImPlot::PlotLine("", miniBinIndices.data(), miniPowerWindow.data(), miniBinIndices.size());//,ImPlotLineFlags_None, 0, sizeof(float));                                                
            ImPlot::EndPlot();
        } 
        ImGui::EndChild();
    }

}