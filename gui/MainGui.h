#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include <string>
#include <map>
#include <chrono>
#include <unistd.h>
#include <memory>

#include "Fonts/Fonts.h"
#include "imfilebrowser.h"
#include "Structs.h"
#include "PopupsAndModals.h"
#include "Filter.h"
#include "TimeDomain.h"
#include "PowerSpectrum.h"
#include "Spectrogram.h"
#include "Scatterplot.h"
#include "IO.h"
#include "InputSource.h"
#include "SampleSource.h"
#include "AbstractSampleSource.h"
#include "AMDetect.h"
#include "FMDetect.h"
#include "PhaseDetect.h"
#include "RealDetect.h"
#include "ImagDetect.h"
#include "FreqShift.h"
#include "Resampler.h"
#include "FilterProc.h"
#include "DDC.h"
#include "OFDMDemod.h"
#include "TakeSkip.h"
#include "Structs.h"
#include "Filename.h"

namespace gui
{
    
    class MainGui
    {
        public:
            // Constructor.
            MainGui(std::string title, int w, int h);
            // Destructor.
            ~MainGui();        
            //Render
            void Render();      
            // Get window size
            ImVec2 GetWindowSize() const;
            //static void glfw_error_callback();

        private:

            //Functions
            void StyeColorsApp();
            void HandlePopups();
            void Update();// called once per frame.
            void ShowTreeStructure(std::shared_ptr<AbstractSampleSource> src);
            void ShowMarkerTable();
            void DeriveSignal();
            void ReconfigureSignal();
            void SaveSignal();
            void ShowSaveRange();
            void TreeStructureContextMenu(std::shared_ptr<AbstractSampleSource> src);
            void DeleteSource(std::shared_ptr<AbstractSampleSource> src);
            void DeleteChildren(std::shared_ptr<AbstractSampleSource> src);
            std::shared_ptr<AbstractSampleSource> FindParent(std::shared_ptr<AbstractSampleSource> child);
            std::shared_ptr<AbstractSampleSource> FindParentFromSrc(std::shared_ptr<AbstractSampleSource> src1,std::shared_ptr<AbstractSampleSource> src2);
            bool MatchSources(std::shared_ptr<AbstractSampleSource> src1, std::shared_ptr<AbstractSampleSource> src2);

            //Gui specific
            ImVec4 ClearColor;                    // background clear color
            GLFWwindow* Window;                   // GLFW window handle
            std::map<std::string,ImFont*> Fonts;  // font map
            bool UsingDGPU; 

            // Settings
            Settings settings;
            bool loadSettings = true;
            std::string settingsFile = "../build/waveviewer.ini";

            //File dialogs etc
            ImGui::FileBrowser fileDialogOpen;
            ImGui::FileBrowser fileDialogSave{ImGuiFileBrowserFlags_EnterNewFilename};
            bool displayFileDialogOpen = true;
            bool displayFileDialogSave = false;
            FileInfo fileinfo;
            
            //Sample source ptrs
            std::shared_ptr<AbstractSampleSource> currentSampleSource = nullptr; //Sample source used as plot source    
            std::shared_ptr<AbstractSampleSource> rootSampleSource = nullptr;   //Initial sample source
            bool showTree = false;
            
            //Derivation of signal variables
            std::shared_ptr<AbstractSampleSource> deriveSignalSource = nullptr;   //Sample source that we operate on
            bool deriveSignal = false;
            DerivationType derivationType = REAL;

            //X,Y markers
            Markers markers;

            //Misc
            bool showError = false;
            std::string errorMsg = "";
            bool closeApp = false;
            bool reconfigureFile = false;
            bool parseFile = false;
            bool inputConfigured = true;
            bool updatePlots = true;
            bool showPopup = true;

            //Parameters to do with signal context menu (derive traces, freq shift etc)
            bool reconfigure = false;
            bool deriveTrace = false;
            bool showDeletePlotWindow = false;
            bool deleteSrc = false;
            bool showFreqShiftWindow = false;
            bool showResampleWindow = false;
            bool showFilterWindow = false;
            bool showDDCWindow = false;
            bool showTakeSkipWindow = false;
            bool freqShiftConfigured = false;
            bool showOFDMDemodWindow = false;
            bool ofdmDemodConfigured = false;
            OFDMDemodVars ofdmDemodVars;
            //bool ofdmInitWithFreqShift = true;
            //bool ofdmCPBasedShift = false;
            bool ofdmEnableCustomSampleReference = false;
            //double ofdmFFTSize = 64.0f;
            //double ofdmCPLen = 16.0f;
            //double ofdmSymbolStart = 0.0f;
            //int ofdmSampleReference = 0;
            //int ofdmNumSymsToEstimateFreqShift = 100;
            TakeSkipVars takeSkipVars;
            double freqShift = 0.0f;
            double freqShiftTemp = 0.0f;
            double outputRate = 0.0f;
            double attenuation = 60.0f;
            double cutoff = 0.0f; //fraction of output rate
            FilterVars filterVars;
            //Save signal variables
            bool saveConfigured = false;
            std::string outFilename; 
            int rangeType = 0;
            int rangeMin;
            int rangeMax;
            

            //Plot specific things
            TimeDomain* td = new TimeDomain();
            PowerSpectrum* psd = new PowerSpectrum();
            Spectrogram* spectrogram = new Spectrogram();
            Scatterplot* scatterplot = new Scatterplot();

            int currentSampleIndex = 0;
            int prevPlotType = 0; //0 Time domain, 1 power spectrum, 2 spectrogram, 3 scatter plot etc
            int plotType = 0;


    };

 }