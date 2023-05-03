#include "TimeDomain.h"
#include "Helpers.h"

namespace gui
{
    TimeDomain::TimeDomain()
    {
        samplesPerRow = 1000;
    }

    TimeDomain::~TimeDomain()
    {
        
    }

    void TimeDomain::Update(std::shared_ptr<AbstractSampleSource> src, int &currentSampleIndex, Markers &markers, bool &updatePlots)
    {    
        numSamples = static_cast<int>(src->Count());
        isReal = src->RealSignal();        
        /*
        if(updatePeriodicYVals)
        {
            //### Some work to be done for this to work ###
            numPeriodicXVals = static_cast<int>(samplesPerRow/markers.xPeriod)+1;
            periodicXVals.resize(numPeriodicXVals);
            double initVal = currentSampleIndex;
            for (size_t i = 0; i < numPeriodicXVals; i++)
            {
                periodicXVals[i] = initVal+i*markers.xPeriod;
            }
            updatePeriodicXVals = false;
        }*/

        static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV;// | ImGuiTableFlags_RowBg;// | ImGuiTableFlags_NoBordersInBody;
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
                if(!isReal)         
                {
                    std::shared_ptr<SampleSource<std::complex<float>>> concrete = std::dynamic_pointer_cast<SampleSource<std::complex<float>>>(src);
                    UpdateData(concrete, currentSampleIndex);
                } else
                {
                    std::shared_ptr<SampleSource<float>> concrete = std::dynamic_pointer_cast<SampleSource<float>>(src);
                    UpdateData(concrete, currentSampleIndex);
                }
                updatePlots = false;
                updateXAxis.assign(5,true);
                if(markers.xPeriodic) updatePeriodicXVals = true;      
                if(markers.yPeriodic) updatePeriodicYVals = true;      
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
            ImGui::TableNextColumn();
            PlotData(currentSampleIndex, markers, updatePlots);
            ImGui::EndTable();
        }
        
                                                 
        //ImGui::EndChild();        
    }

    void TimeDomain::PlotData(int &currentSampleIndex, Markers &markers, bool &updatePlots)
    {
        //Check number of windows to display
        float numWins = 0.0f;
        if(displayReal) numWins++;      
        if(displayImag && !isReal) numWins++;  
        if(displayAM) numWins++;
        if(displayFM) numWins++;
        if(displayPhase) numWins++;  
        ImGui::BeginChild("PlotData");
        if (ImPlot::BeginAlignedPlots("AlignedGroup")) 
        {             
            if(displayReal)
            {                                                                                                                                                                                                  
                if(ImPlot::BeginPlot("##tdreal", ImVec2(-1, 0.95f*ImGui::GetWindowHeight()/numWins)))
                {                                                         
                    ImPlot::SetupAxes("Samples","Real");                                                                                                        
                    if(updateXAxis[0]){
                        ImPlot::SetupAxisLimits(ImAxis_X1, axis.X.Min, axis.X.Max, ImPlotCond_Always);//, ImPlotCond_Once);                                
                        //ImPlot::SetupAxisLimits(ImAxis_Y1, yRealMin, yRealMax, ImPlotCond_Always);//, ImPlotCond_Once);                                
                    } 
                    if(resizeY[0])    
                    {
                        ImPlot::SetupAxisLimits(ImAxis_Y1, yRealMin, yRealMax, ImPlotCond_Always);
                        //resizeY[0] = false;
                    } 
                    ImPlot::SetupAxisFormat(ImAxis_X1, "%.0f");
                    ImPlot::SetupAxisLinks(ImAxis_X1, &axis.X.Min, &axis.X.Max);
                    if(markers.displayXMarkers)
                    {
                        ImPlot::DragLineX(markers.XmarkerTD0ID,&markers.XmarkerTD0,ImVec4(1,0,0,0.7f),0.5f,ImPlotDragToolFlags_None);
                        ImPlot::TagX(markers.XmarkerTD0, ImVec4(1,0,0,1), "M1");
                        ImPlot::DragLineX(markers.XmarkerTD1ID,&markers.XmarkerTD1,ImVec4(1,0,0,0.7f),0.5f,ImPlotDragToolFlags_None);
                        ImPlot::TagX(markers.XmarkerTD1, ImVec4(1,0,0,1), "M2");
                    }
                    if(markers.xPeriodic)
                    {
                        ImPlot::SetNextLineStyle(ImVec4(0.3f, 0.7f, 0.2f, 0.700f), 1.0f);  
                        ImPlot::PlotInfLines("##rv", periodicXVals.data(), numPeriodicXVals);
                    }
                    if(markers.displayYMarkers && markers.controlTrace==0)
                    {
                        ImPlot::DragLineY(markers.YmarkerTD0ID,&markers.YmarkerTD0,ImVec4(1,0,0,0.5f),0.5f,ImPlotDragToolFlags_None);
                        ImPlot::TagY(markers.YmarkerTD0, ImVec4(1,0,0,1), "M1");
                        ImPlot::DragLineY(markers.YmarkerTD1ID,&markers.YmarkerTD1,ImVec4(1,0,0,0.5f),0.5f,ImPlotDragToolFlags_None);
                        ImPlot::TagY(markers.YmarkerTD1, ImVec4(1,0,0,1), "M2");
                    }                       
                    ImPlot::SetNextLineStyle(ImVec4(0.409f, 0.510f, 0.835f, 1.000f), 1.0f);                                
                    ImPlot::PlotLine("##real", displayIndices.data(), dataReal.data(), samplesPerRow);                                        
                    //Handle zoom and pan'ing
                    axis = ImPlot::GetPlotLimits();                     
                    if(axis.X.Min<currentSampleIndex || axis.X.Max > currentSampleIndex+samplesPerRow)
                    {
                        updatePlots = true;
                        currentSampleIndex = (axis.X.Min>=0) ? axis.X.Min : 0;  
                        samplesPerRow = axis.X.Max-axis.X.Min;
                    }                        
                    if(axis.X.Max>numSamples) axis.X.Max = numSamples;                    
                    if(markers.controlTrace == 0)                       
                    {
                        markers.yMarkerRangeMin = axis.Y.Min;
                        markers.yMarkerRangeMax = axis.Y.Max;
                    }
                    ImPlot::EndPlot();
                }
                updateXAxis[0] = false;                                                                           
            }                          
            if(displayImag && !isReal)
            {                                                                                                    
                if(ImPlot::BeginPlot("##tdimag",ImVec2(-1, 0.95f*ImGui::GetWindowHeight()/numWins)))
                {                                            
                    ImPlot::SetupAxes("Samples","Imag"); 
                    if(updateXAxis[1]){
                        ImPlot::SetupAxisLimits(ImAxis_X1,axis.X.Min, axis.X.Max, ImPlotCond_Always);//, ImPlotCond_Once);                                                        
                    } 
                    if(resizeY[1])    
                    {
                        ImPlot::SetupAxisLimits(ImAxis_Y1, yImagMin, yImagMax, ImPlotCond_Always);//, ImPlotCond_Once);                                
                        //resizeY[1] = false;
                    }
                    ImPlot::SetupAxisFormat(ImAxis_X1, "%.0f");                                
                    ImPlot::SetupAxisLinks(ImAxis_X1, &axis.X.Min, &axis.X.Max);
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
                        ImPlot::PlotInfLines("##iv", periodicXVals.data(), numPeriodicXVals);
                    }
                    if(markers.displayYMarkers && markers.controlTrace==1)
                    {
                        ImPlot::DragLineY(markers.YmarkerTD0ID,&markers.YmarkerTD0,ImVec4(1,0,0,0.5f),0.5f,ImPlotDragToolFlags_None);
                        ImPlot::TagY(markers.YmarkerTD0, ImVec4(1,0,0,1), "M1");
                        ImPlot::DragLineY(markers.YmarkerTD1ID,&markers.YmarkerTD1,ImVec4(1,0,0,0.5f),0.5f,ImPlotDragToolFlags_None);
                        ImPlot::TagY(markers.YmarkerTD1, ImVec4(1,0,0,1), "M2");
                    }                      
                    ImPlot::SetNextLineStyle(ImVec4(0.409f, 0.510f, 0.835f, 1.000f), 1.0f);  
                    ImPlot::PlotLine("##imag", displayIndices.data(), dataImag.data(), samplesPerRow);//,ImPlotLineFlags_None, 0, sizeof(float));                                                
                    //Handle zoom and pan'ing
                    axis = ImPlot::GetPlotLimits();                     
                    if(axis.X.Min<currentSampleIndex || axis.X.Max > currentSampleIndex+samplesPerRow)
                    {
                        updatePlots = true;
                        currentSampleIndex = (axis.X.Min>=0) ? axis.X.Min : 0;  
                        samplesPerRow = axis.X.Max-axis.X.Min;
                    }                             
                    if(axis.X.Max>numSamples) axis.X.Max = numSamples;                    
                    if(markers.controlTrace == 1)                       
                    {
                        markers.yMarkerRangeMin = axis.Y.Min;
                        markers.yMarkerRangeMax = axis.Y.Max;
                    }      
                    ImPlot::EndPlot();
                }
                updateXAxis[1] = false;                                       
            }                
            if(displayAM)
            {                                                                                           
                if(ImPlot::BeginPlot("##tdam",ImVec2(-1, 0.95f*ImGui::GetWindowHeight()/numWins)))
                {                                            
                    ImPlot::SetupAxes("Samples","AM");            
                    if(updateXAxis[2])
                    {
                        ImPlot::SetupAxisLimits(ImAxis_X1, axis.X.Min, axis.X.Max, ImPlotCond_Always);//, ImPlotCond_Once);                                                            
                    }
                    if(resizeY[2])    
                    {
                        ImPlot::SetupAxisLimits(ImAxis_Y1, yAMMin, yAMMax, ImPlotCond_Always);//, ImPlotCond_Once);                                                               
                        //resizeY[2] = false;
                    } 
                    ImPlot::SetupAxisFormat(ImAxis_X1, "%.0f");                          
                    ImPlot::SetupAxisLinks(ImAxis_X1, &axis.X.Min, &axis.X.Max);                                    
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
                        ImPlot::PlotInfLines("##av", periodicXVals.data(), numPeriodicXVals);
                    }
                    if(markers.displayYMarkers && markers.controlTrace==2)
                    {
                        ImPlot::DragLineY(markers.YmarkerTD0ID,&markers.YmarkerTD0,ImVec4(1,0,0,0.5f),0.5f,ImPlotDragToolFlags_None);
                        ImPlot::TagY(markers.YmarkerTD0, ImVec4(1,0,0,1), "M1");
                        ImPlot::DragLineY(markers.YmarkerTD1ID,&markers.YmarkerTD1,ImVec4(1,0,0,0.5f),0.5f,ImPlotDragToolFlags_None);
                        ImPlot::TagY(markers.YmarkerTD1, ImVec4(1,0,0,1), "M2");
                    }       
                    ImPlot::SetNextLineStyle(ImVec4(0.409f, 0.510f, 0.835f, 1.000f), 1.0f);     
                    ImPlot::PlotLine("##am", displayIndices.data(), dataAM.data(), samplesPerRow);//,ImPlotLineFlags_None, 0, sizeof(float));                                                                            
                    //Handle zoom and pan'ing
                    axis = ImPlot::GetPlotLimits();                     
                    if(axis.X.Min<currentSampleIndex || axis.X.Max > currentSampleIndex+samplesPerRow)
                    {
                        updatePlots = true;
                        currentSampleIndex = (axis.X.Min>=0) ? axis.X.Min : 0;  
                        samplesPerRow = axis.X.Max-axis.X.Min;
                    }                               
                    if(axis.X.Max>numSamples) axis.X.Max = numSamples;                    
                    if(markers.controlTrace == 2)                       
                    {
                        markers.yMarkerRangeMin = axis.Y.Min;
                        markers.yMarkerRangeMax = axis.Y.Max;
                    }    
                    ImPlot::EndPlot();
                }
                updateXAxis[2] = false;                          
            }
            if(displayFM)
            {                                                                                              
                if(ImPlot::BeginPlot("##tdfm",ImVec2(-1, 0.95f*ImGui::GetWindowHeight()/numWins)))
                {                                            
                    ImPlot::SetupAxes("Samples","FM");  
                    if(updateXAxis[3])
                    {
                        ImPlot::SetupAxisLimits(ImAxis_X1, axis.X.Min, axis.X.Max, ImPlotCond_Always);//, ImPlotCond_Once);                              
                    } 
                    if(resizeY[3])    
                    {
                        ImPlot::SetupAxisLimits(ImAxis_Y1, yFMMin, yFMMax, ImPlotCond_Always);//, ImPlotCond_Once);                                
                        //resizeY[3] = false;
                    }
                    ImPlot::SetupAxisFormat(ImAxis_X1, "%.0f");                          
                    ImPlot::SetupAxisLinks(ImAxis_X1, &axis.X.Min, &axis.X.Max);                                    
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
                        ImPlot::PlotInfLines("##av", periodicXVals.data(), numPeriodicXVals);
                    }
                    if(markers.displayYMarkers && markers.controlTrace==3)
                    {
                        ImPlot::DragLineY(markers.YmarkerTD0ID,&markers.YmarkerTD0,ImVec4(1,0,0,0.5f),0.5f,ImPlotDragToolFlags_None);
                        ImPlot::TagY(markers.YmarkerTD0, ImVec4(1,0,0,1), "M1");
                        ImPlot::DragLineY(markers.YmarkerTD1ID,&markers.YmarkerTD1,ImVec4(1,0,0,0.5f),0.5f,ImPlotDragToolFlags_None);
                        ImPlot::TagY(markers.YmarkerTD1, ImVec4(1,0,0,1), "M2");
                    }        
                    ImPlot::SetNextLineStyle(ImVec4(0.409f, 0.510f, 0.835f, 1.000f), 1.0f);            
                    ImPlot::PlotLine("##fm", displayIndices.data(), dataFM.data(), samplesPerRow);//,ImPlotLineFlags_None, 0, sizeof(float));                                                
                    //Handle zoom and pan'ing
                    axis = ImPlot::GetPlotLimits();                     
                    if(axis.X.Min<currentSampleIndex || axis.X.Max > currentSampleIndex+samplesPerRow)
                    {
                        updatePlots = true;
                        currentSampleIndex = (axis.X.Min>=0) ? axis.X.Min : 0;  
                        samplesPerRow = axis.X.Max-axis.X.Min;
                    }                            
                    if(axis.X.Max>numSamples) axis.X.Max = numSamples;                    
                    if(markers.controlTrace == 3)                       
                    {
                        markers.yMarkerRangeMin = axis.Y.Min;
                        markers.yMarkerRangeMax = axis.Y.Max;
                    }   
                    ImPlot::EndPlot();
                }
                updateXAxis[3] = false;             
            }
            if(displayPhase)
            {                                                                                              
                if(ImPlot::BeginPlot("##tdphase",ImVec2(-1, 0.95f*ImGui::GetWindowHeight()/numWins)))
                {                                            
                    ImPlot::SetupAxes("Samples","Phase"); 
                     if(updateXAxis[4])
                    {
                        ImPlot::SetupAxisLimits(ImAxis_X1, axis.X.Min, axis.X.Max, ImPlotCond_Always);//, ImPlotCond_Once);      
                        ImPlot::SetupAxisLimits(ImAxis_Y1, yPhaseMin, yPhaseMax, ImPlotCond_Always);//, ImPlotCond_Once);                                
                    }
                    if(resizeY[4])    
                    {
                        ImPlot::SetupAxisLimits(ImAxis_Y1, yPhaseMin, yPhaseMax, ImPlotCond_Always);//, ImPlotCond_Once);                                                             
                        //resizeY[4] = false;
                    } 
                    ImPlot::SetupAxisFormat(ImAxis_X1, "%.0f");                          
                    ImPlot::SetupAxisLinks(ImAxis_X1, &axis.X.Min, &axis.X.Max);                                 
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
                        ImPlot::PlotInfLines("##av", periodicXVals.data(), numPeriodicXVals);
                    }
                    if(markers.displayYMarkers && markers.controlTrace==4)
                    {
                        ImPlot::DragLineY(markers.YmarkerTD0ID,&markers.YmarkerTD0,ImVec4(1,0,0,0.5f),0.5f,ImPlotDragToolFlags_None);
                        ImPlot::TagY(markers.YmarkerTD0, ImVec4(1,0,0,1), "M1");
                        ImPlot::DragLineY(markers.YmarkerTD1ID,&markers.YmarkerTD1,ImVec4(1,0,0,0.5f),0.5f,ImPlotDragToolFlags_None);
                        ImPlot::TagY(markers.YmarkerTD1, ImVec4(1,0,0,1), "M2");
                    }                
                    ImPlot::SetNextLineStyle(ImVec4(0.409f, 0.510f, 0.835f, 1.000f), 1.0f);  
                    ImPlot::PlotLine("##phase", displayIndices.data(), dataPhase.data(), samplesPerRow);//,ImPlotLineFlags_None, 0, sizeof(float));                                                
                    //Handle zoom and pan'ing
                    axis = ImPlot::GetPlotLimits();   
                    if(axis.X.Max>numSamples)
                        axis.X.Max = numSamples;                         
                    if(axis.X.Min<currentSampleIndex || axis.X.Max > currentSampleIndex+samplesPerRow)
                    {
                        updatePlots = true;
                        currentSampleIndex = (axis.X.Min>=0) ? axis.X.Min : 0;                                       
                        samplesPerRow = axis.X.Max-axis.X.Min;
                    }                               
                    
                    if(markers.controlTrace == 4)                       
                    {
                        markers.yMarkerRangeMin = axis.Y.Min;
                        markers.yMarkerRangeMax = axis.Y.Max;
                    }    
                    ImPlot::EndPlot();
                }
                updateXAxis[4] = false;                                     
            }
            
            if(numWins>0)
            {
                ImGui::SetNextItemWidth(-1);                    
                if(ImGui::SliderInt("##sampleSlider", &currentSampleIndex, 0, numSamples-samplesPerRow))
                {                                                                  
                    axis.X.Min = currentSampleIndex;
                    axis.X.Max = axis.X.Min+samplesPerRow;     
                    updatePlots = true;                              
                }
            }                                  
            ImPlot::EndAlignedPlots();
        }
        ImGui::EndChild();
    }

    void TimeDomain::UpdateData(std::shared_ptr<SampleSource<float>> src, int &currentSampleIndex)
    {     
        //Do out-of-bounds check in case we came from another domain
        //with currentSampleIndex that would lead to out-of-bounds here
        if(currentSampleIndex+samplesPerRow >= numSamples)
            currentSampleIndex = numSamples-samplesPerRow;
        if(currentSampleIndex < 0)
        {
            currentSampleIndex = 0;
            samplesPerRow = numSamples;
        }            
        std::unique_ptr<std::vector<float>> temp;
        temp = src->GetSamples(currentSampleIndex, samplesPerRow);
        if(temp == nullptr)
        {
            //std::cout << "No data to display " << std::endl;
            currentSampleIndex = numSamples-samplesPerRow;
            if(currentSampleIndex < 0)
            {
                currentSampleIndex = 0;
                samplesPerRow = numSamples;
            }
            temp = src->GetSamples(currentSampleIndex, samplesPerRow);
            if(temp == nullptr)
                temp = std::make_unique<std::vector<float>>(samplesPerRow);
        }
        samplesPerRow = temp.get()->size();
        dataWindowF.resize(samplesPerRow);
        displayIndices.resize(samplesPerRow);
        dataReal.resize(samplesPerRow);
        dataAM.resize(samplesPerRow);
        dataFM.resize(samplesPerRow);
        dataPhase.resize(samplesPerRow);
        yRealMax = 0;
        yRealMin = 0;
        yAMMax = 0;
        yAMMin = 0;
        yFMMax = 0;
        yFMMin = 0;
        yPhaseMax = 0;
        yPhaseMin = 0;
        float prev = 0.0f;
        for (size_t i = 0; i < samplesPerRow; i++)
        {
            dataWindowF[i] = temp.get()->at(i);
            realMA.push(dataWindowF[i]);
            realMedian.push(realMA.mean());
            dataReal[i] = realMedian.median();
            if(dataReal[i]>yRealMax)
                yRealMax = dataReal[i];
            if(dataReal[i]<yRealMin)
                yRealMin = dataReal[i];                     
            if(displayAM)
            {
                amMA.push(dataWindowF[i]*dataWindowF[i]);
                amMedian.push(amMA.mean());
                dataAM[i] = amMedian.median();
                if(dataAM[i]>yAMMax)
                    yAMMax = dataAM[i];
                if(dataAM[i]<yAMMin)
                    yAMMin = dataAM[i];
            }
            if(displayFM)
            {                    
                fmMA.push(atanf(prev*dataWindowF[i]) * degScale/(2*M_PI));
                fmMedian.push(fmMA.mean());
                dataFM[i] = fmMedian.median();
                if(dataFM[i]>yFMMax)
                    yFMMax = dataFM[i];
                if(dataFM[i]<yFMMin)
                    yFMMin = dataFM[i];
            }
            if(displayPhase)
            {
                phaseMA.push(-atanf(dataWindowF[i]) * degScale/(2*M_PI));
                phaseMedian.push(phaseMA.mean());
                dataPhase[i] = phaseMedian.median();
                if(dataPhase[i]>yPhaseMax)
                    yPhaseMax = dataPhase[i];ImPlot::SetupAxisLimits(ImAxis_Y1, yFMMin, yFMMax, ImPlotCond_Always);//, ImPlotCond_Once);   
                if(dataPhase[i]<yPhaseMin)
                    yPhaseMin = dataPhase[i];
            }
            prev = dataWindowF[i];
            displayIndices[i] = i+currentSampleIndex;                
        }  
        axis.X.Min = currentSampleIndex;
        axis.X.Max = currentSampleIndex+samplesPerRow;          
        
    } //End Update <float>

    void TimeDomain::UpdateData(std::shared_ptr<SampleSource<std::complex<float>>> src, int &currentSampleIndex)
    {  
        //Do out-of-bounds check in case we came from another domain
        //with currentSampleIndex that would lead to out-of-bounds here
        if(currentSampleIndex+samplesPerRow >= numSamples)
            currentSampleIndex = numSamples-samplesPerRow;
        if(currentSampleIndex < 0)
        {
            currentSampleIndex = 0;
            samplesPerRow = numSamples;
        }
        std::unique_ptr<std::vector<std::complex<float>>> temp;
        temp = src->GetSamples(currentSampleIndex, samplesPerRow);
        if(temp == nullptr)
        {
            //std::cout << "No data to display " << std::endl;
            currentSampleIndex = numSamples-samplesPerRow;
            if(currentSampleIndex < 0)
            {
                currentSampleIndex = 0;
                samplesPerRow = numSamples;
            }
            temp = src->GetSamples(currentSampleIndex, samplesPerRow);
            if(temp == nullptr)
                temp = std::make_unique<std::vector<std::complex<float>>>(samplesPerRow);
        }
        samplesPerRow = temp.get()->size();
        dataWindowCF.resize(samplesPerRow);
        displayIndices.resize(samplesPerRow);
        dataReal.resize(samplesPerRow);
        dataImag.resize(samplesPerRow);
        dataAM.resize(samplesPerRow);
        dataFM.resize(samplesPerRow);
        dataPhase.resize(samplesPerRow);
        yRealMax = 0;
        yRealMin = 0;
        yImagMax = 0;
        yImagMin = 0;
        yAMMax = 0;
        yAMMin = 0;
        yFMMax = 0;
        yFMMin = 0;
        yPhaseMax = 0;
        yPhaseMin = 0;
        std::complex<float> prev = std::polar(1.0f,0.0f);
        for (size_t i = 0; i < samplesPerRow; i++)
        {
            dataWindowCF[i] = temp.get()->at(i);
            realMA.push(dataWindowCF[i].real());
            realMedian.push(realMA.mean());
            dataReal[i] = realMedian.median();
            imagMA.push(dataWindowCF[i].imag());
            imagMedian.push(imagMA.mean());
            dataImag[i] = imagMedian.median();
            if(dataReal[i]>yRealMax)
                yRealMax = dataReal[i];
            if(dataReal[i]<yRealMin)
                yRealMin = dataReal[i];                
            if(dataImag[i]>yImagMax)
                yImagMax = dataImag[i];
            if(dataImag[i]<yImagMin)
                yImagMin = dataImag[i];     
            if(displayAM)
            {
                amMA.push(dataWindowCF[i].real()*dataWindowCF[i].real()+dataWindowCF[i].imag()*dataWindowCF[i].imag());
                amMedian.push(amMA.mean());
                dataAM[i] = amMedian.median();
                if(dataAM[i]>yAMMax)
                    yAMMax = dataAM[i];
                if(dataAM[i]<yAMMin)
                    yAMMin = dataAM[i];
            }
            if(displayFM)
            {
                fmMA.push(arg(conj(prev)*dataWindowCF[i]) * degScale/(2*M_PI));
                fmMedian.push(fmMA.mean());
                dataFM[i] = fmMedian.median();
                if(dataFM[i]>yFMMax)
                    yFMMax = dataFM[i];
                if(dataFM[i]<yFMMin)
                    yFMMin = dataFM[i];
            }
            if(displayPhase)
            {
                phaseMA.push(-arg(dataWindowCF[i]) * degScale/(2*M_PI));
                phaseMedian.push(phaseMA.mean());
                dataPhase[i] = phaseMedian.median();
                if(dataPhase[i]>yPhaseMax)
                    yPhaseMax = dataPhase[i];
                if(dataPhase[i]<yPhaseMin)
                    yPhaseMin = dataPhase[i];
            }
            prev = dataWindowCF[i];
            displayIndices[i] = i+currentSampleIndex;                
        }  
        axis.X.Min = currentSampleIndex;
        axis.X.Max = currentSampleIndex+samplesPerRow;
        
    } //End Update complex<float>

    void TimeDomain::ShowControls(int &currentSampleIndex, Markers &markers, bool &updatePlots)
    {
        //Setup controls
        ImGui::BeginChild("ConotrolChild");//, ImVec2(ImGui::GetWindowContentRegionWidth() * 0.2f, -1));        
        ImGui::Text("Start sample");      
        ImGui::SetNextItemWidth(180);                 
        //if(ImGui::InputInt("##startsample", &currentSampleIndex))
        currentSampleIndexStr = std::to_string(currentSampleIndex);
        if(gui::InputStrAndEvaluateToInt("##tdsampleIndexStr",currentSampleIndexStr))
        {                
            currentSampleIndex = std::stoi(currentSampleIndexStr);
            //Check if we went out-of-bounds with out expression
            if(currentSampleIndex < 0)
                currentSampleIndex = 0;
            if(currentSampleIndex+samplesPerRow >= numSamples)
                samplesPerRow = numSamples-currentSampleIndex;
                
            //Update axis
            axis.X.Min = currentSampleIndex;
            axis.X.Max = axis.X.Min+samplesPerRow;
            updatePlots = true;                    
            updateXAxis.assign(5,true);     
        }                
        ImGui::Text("Samples/row");
        ImGui::SetNextItemWidth(180);                    
        //if(ImGui::InputInt("##samplesperrowtd", &samplesPerRow))
        samplesPerRowStr = std::to_string(samplesPerRow);
        if(gui::InputStrAndEvaluateToInt("##tdsamplesPerRowStr",samplesPerRowStr))
        {    
            samplesPerRow = std::stoi(samplesPerRowStr);
            //Check if we went out of bounds with out expression
            if(samplesPerRow<1)
                samplesPerRow = 1;        
            if(currentSampleIndex+samplesPerRow >= numSamples)
                currentSampleIndex = numSamples-samplesPerRow;
            if(currentSampleIndex < 0)
            {
                currentSampleIndex = 0;
                samplesPerRow = numSamples;
            }          

            //Update axis  
            axis.X.Max = axis.X.Min+samplesPerRow;
            updatePlots = true;
            updateXAxis.assign(5,true);  
        }
        ImGui::Separator();
        ImGui::Text("Plots to display");
        if(ImGui::Checkbox("Real", &displayReal))
        {
            resizeY[0] = true;
            updatePlots = true;
        }
        if(!isReal)
            if(ImGui::Checkbox("Imag", &displayImag))
            {
                resizeY[1] = true;
                updatePlots = true;
            } 
        if(ImGui::Checkbox("AM Detects", &displayAM))
        {
            if(displayAM)
            {
                resizeY[2] = true;
                updatePlots = true;
            }                            
        } 
        if(ImGui::Checkbox("FM Detect", &displayFM))
        {
            resizeY[3] = true;
            updatePlots = true;
        }
        if(ImGui::Checkbox("Phase", &displayPhase))
        {
            resizeY[4] = true;
            updatePlots = true;
        }
        ImGui::Separator();
        ImGui::SetNextItemWidth(100);  
        ImGui::Text("Control");
        ImGui::SetNextItemWidth(100);  
        ImGui::Combo("##controlTrace", &markers.controlTrace, "Real\0""Imag\0""AM\0""FM\0""Phase\0");   
        ImGui::SetNextItemWidth(100.0f);
        ImGui::Text("Filter MA       "); ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        if(ImGui::InputInt("##filterma", &filterLenMA[markers.controlTrace]))
        {
            //filterLen[markers.controlTrace] = filterLenTemp;
            if(filterLenMA[markers.controlTrace]<1) filterLenMA[markers.controlTrace] = 1;
            if(markers.controlTrace == 0)
                realMA.reset(filterLenMA[0]);
            if(markers.controlTrace == 1)
                imagMA.reset(filterLenMA[1]);
            if(markers.controlTrace == 2)
                amMA.reset(filterLenMA[2]);
            if(markers.controlTrace == 3)
                fmMA.reset(filterLenMA[3]);
            if(markers.controlTrace == 4)
                phaseMA.reset(filterLenMA[4]);
            updatePlots = true;
        }      
        ImGui::SetNextItemWidth(100.0f);
        ImGui::Text("Filter median"); ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        if(ImGui::InputInt("##filterlenmed", &filterLenMedian[markers.controlTrace]))
        {
            //filterLen[markers.controlTrace] = filterLenTemp;
            if(filterLenMedian[markers.controlTrace]<1) filterLenMedian[markers.controlTrace] = 1;
            if(markers.controlTrace == 0)
                realMedian.reset(filterLenMedian[0]);
            if(markers.controlTrace == 1)
                imagMedian.reset(filterLenMedian[1]);
            if(markers.controlTrace == 2)
                amMedian.reset(filterLenMedian[2]);
            if(markers.controlTrace == 3)
                fmMedian.reset(filterLenMedian[3]);
            if(markers.controlTrace == 4)
                phaseMedian.reset(filterLenMedian[4]);
            updatePlots = true;
        }        
        traceResize = resizeY[markers.controlTrace];
        ImGui::Checkbox("Auto-size Y", &traceResize);
        if(traceResize)
            resizeY[markers.controlTrace] = true;
        else
            resizeY[markers.controlTrace] = false;        
        ImGui::Separator();
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
            markers.XmarkerTD0 = currentSampleIndex;
            markers.XmarkerTD1 = markers.XmarkerTD0+(axis.X.Max-axis.X.Min)/2; 
            markers.xPeriodicReference = currentSampleIndex;
            updatePeriodicXVals = true;                   
        }
        ImGui::Checkbox("Y markers", &markers.displayYMarkers);                                                     
        /*
        if(ImGui::Checkbox("Periodic", &markers.yPeriodic))
            updatePeriodicYVals = true;
        if(!markers.yPeriodic)
            ImGui::BeginDisabled();
        ImGui::Text("Period"); ImGui::SameLine();
        if(ImGui::InputFloat("##yperiod", &markers.yPeriod))
            updatePeriodicYVals = true;
        if(!markers.yPeriodic)
            ImGui::EndDisabled();  
        */
        if(ImGui::Button("Reset y markers"))
        {
            markers.YmarkerTD0 = markers.yMarkerRangeMin/2;
            markers.YmarkerTD1 = markers.yMarkerRangeMax/2;                    
        }           
        ImGui::EndChild();            
    }
}