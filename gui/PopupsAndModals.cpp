
#include "PopupsAndModals.h"

using namespace std;
namespace gui
{
    void ShowError(bool &showError, string &errorMsg)
    {
        ImGui::OpenPopup("Error");
        ImGui::SetNextWindowPos({0.5f*ImGui::GetWindowContentRegionWidth(), 0.5f*ImGui::GetWindowHeight()});
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(200), static_cast<float>(100)),ImGuiCond_FirstUseEver);
        if(ImGui::BeginPopupModal("Error"))
        {
            ImGui::Text("%s", errorMsg.c_str());
            if (ImGui::Button("Close"))
            {
                showError = false;
                errorMsg = "";                
                ImGui::CloseCurrentPopup();                                
            }
            ImGui::EndPopup();   
        }
    }

    void ShowFrequencyShiftMenu(double* shift, bool &show, bool &configured)
    {
        ImGui::OpenPopup("Frequency shift");      
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));    
        if(ImGui::BeginPopupModal("Frequency shift"))
        {
            ImGui::Text("Frequency shift signal");
            ImGui::Text("Shift in Hz:"); ImGui::SameLine();              
            ImGui::InputDouble("##shift", shift);          
            if (ImGui::Button("OK"))
            {                            
                show = false;
                configured = true;
                //cout << "Shift var: " << shift << endl;
                ImGui::CloseCurrentPopup();                                
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {        
                //shift = 0.0f;
                configured = false;
                show = false;
                ImGui::CloseCurrentPopup();                                
            }
            ImGui::EndPopup();   
        }
    }

    void ShowResampleMenu(double* outputRate, bool &show, bool &configured)
    {
        ImGui::OpenPopup("Resample");      
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));    
        if(ImGui::BeginPopupModal("Resample"))
        {
            ImGui::Text("Resample signal");
            ImGui::Text("New rate:"); ImGui::SameLine();              
            ImGui::InputDouble("##shift", outputRate);          
            if (ImGui::Button("OK"))
            {                            
                show = false;
                configured = true;
                //cout << "Shift var: " << shift << endl;
                ImGui::CloseCurrentPopup();                                
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {        
                //shift = 0.0f;
                configured = false;
                show = false;
                ImGui::CloseCurrentPopup();                                
            }
            ImGui::EndPopup();   
        }
    }

    void ShowFilterMenu(FilterVars &vars, bool &show, bool &configured)
    {
        ImGui::OpenPopup("Filter");      
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));    
        if(ImGui::BeginPopupModal("Filter"))
        {
            ImGui::Text("Filter signal");
            ImGui::RadioButton("BW range", &vars.filterType, 0);
            if(vars.filterType != 0)    ImGui::BeginDisabled();            
            ImGui::Text("Center freq:"); ImGui::SameLine();
            ImGui::InputDouble("##center", &vars.centerFreq);          
            ImGui::Text("Pass bandwidth"); ImGui::SameLine();              
            ImGui::InputDouble("##cutoff", &vars.cutoff);  
            ImGui::Text("Stop-band attenuation [dB]"); ImGui::SameLine();              
            ImGui::InputDouble("##att", &vars.attenuation); 
            if(vars.filterType != 0)    ImGui::EndDisabled();

            ImGui::RadioButton("Real taps", &vars.filterType, 1);
            if(vars.filterType != 1)    ImGui::BeginDisabled();
            ImGui::Checkbox("Flip taps", &vars.flipTapsReal);
            ImGui::Text("Taps:");    
            ImGui::SameLine();
            ImGui::InputText("##realtaps", vars.realTapsChar, 10000); 
            if(vars.filterType != 1)    ImGui::EndDisabled();

            ImGui::RadioButton("Complex taps", &vars.filterType, 2);
            if(vars.filterType != 2)    ImGui::BeginDisabled();
            ImGui::Checkbox("Flip taps", &vars.flipTapsImag);
            ImGui::Text("Taps:");    
            ImGui::SameLine();
            ImGui::InputText("##complextaps", vars.complexTapsChar, 10000);             
            if(vars.filterType != 2)    ImGui::EndDisabled();
            //ImGui::Text("Output rate"); ImGui::SameLine();              
            //ImGui::InputDouble("##rate", outputRate);          
            if (ImGui::Button("OK"))
            {                            
                show = false;
                configured = true;
                double tmp = vars.cutoff;
                vars.cutoff = tmp/2.0f; //this is how it is passed to FilterProc
                if(vars.filterType == 1)
                {
                    std::istringstream ss(vars.realTapsChar);
                    vars.realTaps.clear();                    
                    std::string token;
                    while(std::getline(ss, token, ','))
                    {
                        vars.realTaps.push_back(std::stof(token));
                    }                         
                    if(vars.realTaps.size() == 0)               
                    {
                        //todo fix pupop for no taps
                        configured = false;                        
                    } else
                    {
                        if(vars.flipTapsReal)
                        {
                            int numTaps = vars.realTaps.size();
                            std::vector<float> flipped(numTaps);
                            for (size_t i = 0; i < numTaps; i++)
                            {
                                flipped[i] = vars.realTaps[numTaps-1-i];
                            }
                            vars.realTaps = flipped;
                        }
                    }
                } else if (vars.filterType == 2)
                {
                    std::istringstream ss(vars.complexTapsChar);
                    vars.complexTaps.clear();                    
                    std::string token;
                    std::complex<float> t;
                    while(std::getline(ss, token, ','))
                    {
                        std::istringstream sss(token);
                        std::string real,imag;
                        std::getline(sss, real, '+');
                        std::getline(sss, imag, '+');
                        t = std::complex<float>(std::stof(real), std::stof(imag));
                        vars.complexTaps.push_back(t);
                    }
                    if(vars.complexTaps.size() == 0)               
                    {
                        //todo fix pupop for no taps
                        configured = false;                        
                    } else
                    {
                        if(vars.flipTapsImag)
                        {
                            int numTaps = vars.complexTaps.size();
                            std::vector<std::complex<float>> flipped(numTaps);
                            for (size_t i = 0; i < numTaps; i++)
                            {
                                flipped[i] = vars.complexTaps[numTaps-1-i];
                            }
                            vars.complexTaps = flipped;
                        }
                    }
                }                
                ImGui::CloseCurrentPopup();                                
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {        
                //shift = 0.0f;
                configured = false;
                show = false;
                ImGui::CloseCurrentPopup();                                
            }
            ImGui::EndPopup();   
        }
    }

    void ShowDDCMenu(double* centerFreq, double* outputRate, double* cutoff, double* attenuation, bool &show, bool &configured)
    {
        ImGui::OpenPopup("DDC");      
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));    
        if(ImGui::BeginPopupModal("DDC"))
        {
            ImGui::Text("DDC signal");
            ImGui::Text("Center freq:"); ImGui::SameLine();
            ImGui::InputDouble("##center", centerFreq);       
            ImGui::Text("New rate:"); ImGui::SameLine();              
            ImGui::InputDouble("##shift", outputRate);       
            ImGui::Text("Cutoff"); ImGui::SameLine();              
            ImGui::InputDouble("##cutoff", cutoff);  
            ImGui::Text("Stop-band attenuation [dB]"); ImGui::SameLine();              
            ImGui::InputDouble("##att", attenuation);  
            //ImGui::Text("Output rate"); ImGui::SameLine();              
            //ImGui::InputDouble("##rate", outputRate);          
            if (ImGui::Button("OK"))
            {                            
                show = false;
                configured = true;
                //cout << "Shift var: " << shift << endl;
                ImGui::CloseCurrentPopup();                                
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {        
                //shift = 0.0f;
                configured = false;
                show = false;
                ImGui::CloseCurrentPopup();                                
            }
            ImGui::EndPopup();   
        }
    }

    void ShowOFDMDemodMenu(OFDMDemodVars &vars, bool &ofdmEnableCustomSampleReference, bool &showOFDMDemodWindow, bool &ofdmDemodConfigured)
    {
        ImGui::OpenPopup("OFDM demod");      
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));    
        if(ImGui::BeginPopupModal("OFDM demod"))
        {            
            ImGui::Text("FFT size:"); ImGui::SameLine();
            ImGui::InputInt("##fftsize", &vars.fftSize);          
            ImGui::Text("CP size"); ImGui::SameLine();              
            ImGui::InputInt("##cplen", &vars.cpLen);  
            ImGui::Text("Symbol start"); ImGui::SameLine();              
            ImGui::InputInt("##symstart", &vars.symStart);  
            ImGui::Checkbox("Custom sample ref", &ofdmEnableCustomSampleReference);
            if(!ofdmEnableCustomSampleReference)
                ImGui::BeginDisabled();            
            ImGui::Text("Sample index reference"); ImGui::SameLine();              
            ImGui::InputInt("##sampleref", &vars.sampleReference);          
            if(!ofdmEnableCustomSampleReference)
                ImGui::EndDisabled();   
            ImGui::Checkbox("Init with freq shift", &vars.initFreqShift);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("We find the a CFO estimate by averaging the angle of the OFDM symbol start sample."); 
            if(vars.initFreqShift)
            {
                ImGui::SameLine();
                ImGui::InputInt("##numsyms", &vars.numSymsToEstimateOffset);
            }
            ImGui::Checkbox("Calc freq shift/symbol", &vars.cpBasedShift);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("We use the cyclic prefix in each OFDM symbol to estimate the carrier offset for that symbol."); 
            ImGui::Checkbox("CFO est w/ pilot carriers", &vars.pilotCarrierEq);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("The pilot carriers are assumed to be BPSK. By using pilots we try to make all carriers \n coherent so that constalletionss with absolute mapping are mapped correctly."); 
            if(!vars.pilotCarrierEq) ImGui::BeginDisabled();
            ImGui::Text("Pilot carriers:");    
            ImGui::SameLine();
            ImGui::InputText("##pilots", vars.pilotsChar, 1000);            
            if(!vars.pilotCarrierEq) ImGui::EndDisabled();
            if (ImGui::Button("OK"))
            {                            
                showOFDMDemodWindow = false;
                ofdmDemodConfigured = true;
                std::stringstream ss(vars.pilotsChar);
                vars.pilotCarrierIndices.clear();
                for (int i; ss >> i;) {
                    vars.pilotCarrierIndices.push_back(i);    
                    if (ss.peek() == ',')
                        ss.ignore();
                }
                for (size_t i = 0; i < vars.pilotCarrierIndices.size(); i++)                                
                    std::cout << vars.pilotCarrierIndices[i] << std::endl;
                //cout << "Shift var: " << shift << endl;
                ImGui::CloseCurrentPopup();                                
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {        
                //shift = 0.0f;
                showOFDMDemodWindow = false;
                ofdmDemodConfigured = false;
                ImGui::CloseCurrentPopup();                                
            }
            ImGui::EndPopup();   
        }
    }
    

    void ShowReconfigureWarning(bool &parse)
    {
        ImGui::OpenPopup("Reconfig warning");
        ImGui::SetNextWindowPos({0.5f*ImGui::GetWindowContentRegionWidth(), 0.5f*ImGui::GetWindowHeight()});
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(200), static_cast<float>(100)),ImGuiCond_FirstUseEver);
        if(ImGui::BeginPopupModal("Reconfig warning"))
        {
            ImGui::Text("%s", "Reconfiguring input will delete all derived plots! Continue ?");
            if (ImGui::Button("OK"))
            {                            
                ImGui::CloseCurrentPopup();                                
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                parse = false;
                ImGui::CloseCurrentPopup();                                
            }
            ImGui::EndPopup();   
        }
    }
}
/*
void ShowFrequencyShiftMeny(FrequencyShiftStruct &s)
{
    ImGui::OpenPopup("Frequency shift");
    ImGui::SetNextWindowPos({0.5f*ImGui::GetWindowContentRegionWidth(), 0.5f*ImGui::GetWindowHeight()});
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(200), static_cast<float>(100)));
    if(ImGui::BeginPopupModal("Frequency shift"))
    {
        ImGui::Text("%s", "Frequency shift signal");
        if (ImGui::Button("OK"))
        {                            
            ImGui::CloseCurrentPopup();                                
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {        
            ImGui::CloseCurrentPopup();                                
        }
        ImGui::EndPopup();   
    }
}

void ShowDerivedPlotMenu(DerivedPlot plot)
{
    ImGui::Text("Add derived plots for %s", plot.name.c_str());
    FrequencyShiftStruct s;
    if(ImGui::MenuItem("Frequency shift"))
    {
        cout << "Frequency shift meny " << endl;
        ShowFrequencyShiftMeny(s);
    }
        
    if (ImGui::Button("Close"))
        ImGui::CloseCurrentPopup();
}

*/