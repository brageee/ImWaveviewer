#include "MainGui.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

using namespace gui;

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

ImVec2 MainGui::GetWindowSize() const
{
    int w, h;
    glfwGetWindowSize(Window, &w, &h);
    return ImVec2(w, h);
}

void MainGui::StyeColorsApp()
{
    static const ImVec4 bg_dark = ImVec4(0.15f, 0.16f, 0.21f, 1.00f);
    static const ImVec4 bg_mid = ImVec4(0.20f, 0.21f, 0.27f, 1.00f);
    static const ImVec4 accent_dark = ImVec4(0.292f, 0.360f, 0.594f, 1.000f);
    static const ImVec4 accent_light = ImVec4(0.409f, 0.510f, 0.835f, 1.000f);
    static const ImVec4 active = ImVec4(0.107f, 0.118f, 0.157f, 1.000f);
    static const ImVec4 attention = ImVec4(0.821f, 1.000f, 0.000f, 1.000f);

    auto &style = ImGui::GetStyle();
    style.WindowPadding = {6, 6};
    style.FramePadding = {6, 3};
    style.CellPadding = {6, 3};
    style.ItemSpacing = {6, 6};
    style.ItemInnerSpacing = {6, 6};
    style.ScrollbarSize = 16;
    style.GrabMinSize = 8;
    style.WindowBorderSize = style.ChildBorderSize = style.PopupBorderSize = style.TabBorderSize = 0;
    style.FrameBorderSize = 1;
    style.WindowRounding = style.ChildRounding = style.PopupRounding = style.ScrollbarRounding = style.GrabRounding = style.TabRounding = 4;

    ImVec4 *colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(0.89f, 0.89f, 0.92f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.38f, 0.45f, 0.64f, 1.00f);
    colors[ImGuiCol_WindowBg] = bg_mid;
    colors[ImGuiCol_ChildBg] = ImVec4(0.20f, 0.21f, 0.27f, 0.00f);
    colors[ImGuiCol_PopupBg] = bg_mid;
    colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.06f);
    colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.02f);
    colors[ImGuiCol_FrameBgHovered] = accent_light;
    colors[ImGuiCol_FrameBgActive] = active;
    colors[ImGuiCol_TitleBg] = accent_dark;
    colors[ImGuiCol_TitleBgActive] = accent_dark;
    colors[ImGuiCol_TitleBgCollapsed] = accent_dark;
    colors[ImGuiCol_MenuBarBg] = accent_dark;
    colors[ImGuiCol_ScrollbarBg] = bg_mid;
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.89f, 0.89f, 0.93f, 0.27f);
    colors[ImGuiCol_ScrollbarGrabHovered] = accent_light;
    colors[ImGuiCol_ScrollbarGrabActive] = active;
    colors[ImGuiCol_CheckMark] = accent_dark;
    colors[ImGuiCol_SliderGrab] = accent_dark;
    colors[ImGuiCol_SliderGrabActive] = accent_light;
    colors[ImGuiCol_Button] = accent_dark;
    colors[ImGuiCol_ButtonHovered] = accent_light;
    colors[ImGuiCol_ButtonActive] = active;
    colors[ImGuiCol_Header] = accent_dark;
    colors[ImGuiCol_HeaderHovered] = accent_light;
    colors[ImGuiCol_HeaderActive] = active;
    colors[ImGuiCol_Separator] = accent_dark;
    colors[ImGuiCol_SeparatorHovered] = accent_light;
    colors[ImGuiCol_SeparatorActive] = active;
    colors[ImGuiCol_ResizeGrip] = accent_dark;
    colors[ImGuiCol_ResizeGripHovered] = accent_light;
    colors[ImGuiCol_ResizeGripActive] = active;
    colors[ImGuiCol_Tab] = ImVec4(1.00f, 1.00f, 1.00f, 0.02f);
    colors[ImGuiCol_TabHovered] = accent_light;
    colors[ImGuiCol_TabActive] = accent_dark;
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = active;
    colors[ImGuiCol_PlotLines] = accent_light;
    colors[ImGuiCol_PlotLinesHovered] = active;
    colors[ImGuiCol_PlotHistogram] = accent_light;
    colors[ImGuiCol_PlotHistogramHovered] = active;
    colors[ImGuiCol_TableHeaderBg] = accent_dark;
    colors[ImGuiCol_TableBorderStrong] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(1.00f, 1.00f, 1.00f, 0.02f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0, 0, 0, 0);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.02f);
    colors[ImGuiCol_TextSelectedBg] = accent_light;
    colors[ImGuiCol_DragDropTarget] = attention;
    colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
#ifdef IMGUI_HAS_DOCK
    colors[ImGuiCol_DockingPreview] = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
#endif

    ImPlot::StyleColorsAuto();

    ImVec4 *pcolors = ImPlot::GetStyle().Colors;
    pcolors[ImPlotCol_PlotBg] = ImVec4(0, 0, 0, 0);
    pcolors[ImPlotCol_PlotBorder] = ImVec4(0, 0, 0, 0);
    pcolors[ImPlotCol_Selection] = attention;
    pcolors[ImPlotCol_Crosshairs] = colors[ImGuiCol_Text];

    ImPlot::GetStyle().DigitalBitHeight = 20;

    auto &pstyle = ImPlot::GetStyle();
    pstyle.PlotPadding = pstyle.LegendPadding = {12, 12};
    pstyle.LabelPadding = pstyle.LegendInnerPadding = {6, 6};
    pstyle.LegendSpacing = {10, 2};
    pstyle.AnnotationPadding = {4,2};

    const ImU32 Dracula[]  = {4288967266, 4285315327, 4286315088, 4283782655, 4294546365, 4287429361, 4291197439, 4294830475, 4294113528, 4284106564                        };
    ImPlot::GetStyle().Colormap = ImPlot::AddColormap("Dracula",Dracula,10);
}


MainGui::MainGui(std::string title, int w, int h)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        abort();

        // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);          // 3.0+ only
#endif

    const bool no_vsync = false;
    const bool use_msaa = false;

    if (use_msaa) {
        title += " - 4X MSAA";
        glfwWindowHint(GLFW_SAMPLES, 4);
    }

    // Create window with graphics context
    Window = glfwCreateWindow(w, h, title.c_str(), NULL, NULL);
    if (Window == NULL)
    {
        fprintf(stderr, "Failed to initialize GLFW window!\n");
        abort();
    }
    glfwMakeContextCurrent(Window);
    glfwSwapInterval(no_vsync ? 0 : 1);

    // Initialize OpenGL loader
    bool err = gladLoadGL() == 0;
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        abort();
    } 

    const GLubyte* vendor = glGetString(GL_VENDOR); 
    const GLubyte* renderer = glGetString(GL_RENDERER); 

    title +=  " - ";
    title += reinterpret_cast< char const * >(renderer);
    glfwSetWindowTitle(Window, title.c_str());

    //Set window icon
    //GLFWimage images[2];
    //images[0] = load_icon("my_icon.png");
    //images[1] = load_icon("my_icon_small.png");
    GLFWimage images[1]; 
    images[0].pixels = stbi_load("../gui/Images/psd_small.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
    glfwSetWindowIcon(Window, 1, images); 
    stbi_image_free(images[0].pixels);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(Window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    if (use_msaa)
        glEnable(GL_MULTISAMPLE); 

    if (false) { //im_style
        ImGui::StyleColorsDark();
        ImPlot::StyleColorsDark();
    }
    else {
        ClearColor = ImVec4(0.15f, 0.16f, 0.21f, 1.00f);
        StyeColorsApp();
    }

    ImGuiIO &io = ImGui::GetIO();

    //io.DeltaTime = 10;

    // add fonts
    io.Fonts->Clear();

    ImFontConfig font_cfg;
    font_cfg.FontDataOwnedByAtlas = false;

    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = 14.0f;
    icons_config.GlyphOffset = ImVec2(0, 0);
    icons_config.OversampleH = 1;
    icons_config.OversampleV = 1;
    icons_config.FontDataOwnedByAtlas = false;

    static const ImWchar fa_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};

    if (false) {//im_style
        io.Fonts->AddFontDefault();
        io.Fonts->AddFontFromMemoryTTF(fa_solid_900_ttf, fa_solid_900_ttf_len, 14.0f, &icons_config, fa_ranges);
    }
    
    ImStrncpy(font_cfg.Name, "Roboto Bold", 40);
    Fonts[font_cfg.Name] = io.Fonts->AddFontFromMemoryTTF(Roboto_Bold_ttf, Roboto_Bold_ttf_len, 15.0f, &font_cfg);
    io.Fonts->AddFontFromMemoryTTF(fa_solid_900_ttf, fa_solid_900_ttf_len, 14.0f, &icons_config, fa_ranges);

    ImStrncpy(font_cfg.Name, "Roboto Italic", 40);
    Fonts[font_cfg.Name] = io.Fonts->AddFontFromMemoryTTF(Roboto_Italic_ttf, Roboto_Italic_ttf_len, 15.0f, &font_cfg);
    io.Fonts->AddFontFromMemoryTTF(fa_solid_900_ttf, fa_solid_900_ttf_len, 14.0f, &icons_config, fa_ranges);

    ImStrncpy(font_cfg.Name, "Roboto Regular", 40);
    Fonts[font_cfg.Name] = io.Fonts->AddFontFromMemoryTTF(Roboto_Regular_ttf, Roboto_Regular_ttf_len, 15.0f, &font_cfg);
    io.Fonts->AddFontFromMemoryTTF(fa_solid_900_ttf, fa_solid_900_ttf_len, 14.0f, &icons_config, fa_ranges);

    ImStrncpy(font_cfg.Name, "Roboto Mono Bold", 40);
    Fonts[font_cfg.Name] = io.Fonts->AddFontFromMemoryTTF(RobotoMono_Bold_ttf, RobotoMono_Bold_ttf_len, 15.0f, &font_cfg);
    io.Fonts->AddFontFromMemoryTTF(fa_solid_900_ttf, fa_solid_900_ttf_len, 14.0f, &icons_config, fa_ranges);

    ImStrncpy(font_cfg.Name, "Roboto Mono Italic", 40);
    Fonts[font_cfg.Name] = io.Fonts->AddFontFromMemoryTTF(RobotoMono_Italic_ttf, RobotoMono_Italic_ttf_len, 15.0f, &font_cfg);
    io.Fonts->AddFontFromMemoryTTF(fa_solid_900_ttf, fa_solid_900_ttf_len, 14.0f, &icons_config, fa_ranges);

    ImStrncpy(font_cfg.Name, "Roboto Mono Regular", 40);
    Fonts[font_cfg.Name] = io.Fonts->AddFontFromMemoryTTF(RobotoMono_Regular_ttf, RobotoMono_Regular_ttf_len, 15.0f, &font_cfg);
    io.Fonts->AddFontFromMemoryTTF(fa_solid_900_ttf, fa_solid_900_ttf_len, 14.0f, &icons_config, fa_ranges);
    
}

MainGui::~MainGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    glfwDestroyWindow(Window);
    glfwTerminate();
}

void MainGui::Render()
{
    chrono::system_clock::time_point now;
    chrono::system_clock::time_point procTime;
    // Main loop
    while (!glfwWindowShouldClose(Window))
    {
        now = chrono::system_clock::now();
        glfwPollEvents();
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        Update();
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(Window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(Window);
        procTime = chrono::system_clock::now();
        chrono::duration<double, milli> delta = procTime-now;
        //cout << "Millisec per update " << delta.count() << endl;
        //double restTime = 100 - delta.count();
        //if(restTime>0)
        //    usleep(restTime*1000);
    }
}


void MainGui::Update()
{
    //Begin main window and menu
    ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(GetWindowSize(), ImGuiCond_Always);
    ImGui::Begin("Waveviewer",nullptr, ImGuiWindowFlags_NoResize| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar);        
    if(ImGui::BeginMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("Read file", NULL, &displayFileDialogOpen);
            ImGui::MenuItem("Configure", NULL, &reconfigureFile);
            ImGui::MenuItem("Close", NULL, &closeApp);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    } 
    if(closeApp)
        exit(1);

    if(loadSettings)
    {
        if(LoadSettings(settings, settingsFile))
            fileDialogOpen.SetPwd(settings.defaultPath);
        loadSettings = false;
    }    

    //Open file
    if(displayFileDialogOpen)
    {
        fileDialogOpen.Open();
        displayFileDialogOpen = false;
    }
    fileDialogOpen.Display();
    if(fileDialogOpen.HasSelected())
    {                        
        //std::cout << "Selected filename" << fileDialogOpen.GetSelected().string() << std::endl;
        fileinfo.filename = fileDialogOpen.GetSelected().string();
        if(settings.updatePath)
            SetSettingsPath(fileinfo.filename, settingsFile);

        // Try to decode file metadata from filename
        if(getFileMetadata(fileDialogOpen.GetSelected().string(), fileinfo))
        {
            
        }
        else
        {

        }
                                        
        fileDialogOpen.Close();
        fileDialogOpen.ClearSelected();            
        inputConfigured = false;  
        showPopup = true;                    
    }        

    //Parse file if opened new file
    if(parseFile && fileinfo.supportedFormat)
    {          
        derivationType = INPUT;
        deriveSignal = true;
    }

    //Save signal
    if(displayFileDialogSave)
    {
        std::string s = deriveSignalSource->GetName();
        if(deriveSignalSource->RealSignal())
            s += ".real";
        else
            s += ".complex";
        std::stringstream ss;
        ss.precision(2);
        ss << fixed;
        ss << deriveSignalSource->Rate();
        s += "." + ss.str();
        s += ".32fl";
        fileDialogSave.Open();
        fileDialogSave.SetInputName(s);
        displayFileDialogSave = false;
    }
    fileDialogSave.Display();

    if(fileDialogSave.HasSelected())
    { 
        //std::cout << "Selected filename " << fileDialogSave.GetSelected().string() << std::endl;
        ShowSaveRange();
    }
    
    if(deriveSignal)
    {
        if(!reconfigure)
            DeriveSignal();
        else    
            ReconfigureSignal();
    }

    if(saveConfigured)
        SaveSignal();
    
    if(showPopup)
        HandlePopups();
    
    
   
    /*##################################
    ### Plot Uppder window with signals tree
    ### and marker tables
    ####################################*/ 
    static ImGuiTableFlags flags2 = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable;// | ImGuiTableFlags_NoBordersInBody;
    if (ImGui::BeginTable("Upper", 2, flags2))
    {
        ImGui::TableSetupColumn("Tree", ImGuiTableColumnFlags_NoHide);
        ImGui::TableSetupColumn("Markers", ImGuiTableColumnFlags_WidthFixed,  ImGui::GetWindowContentRegionWidth()* 0.7f);
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        //ImGui::BeginChild("fileinfo", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.3f, ImGui::GetWindowHeight()*0.15f));
        ImGui::Text("%s", fileinfo.filename.data());                        
        if(showTree)
        {
            //static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
            static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
            if (ImGui::BeginTable("3ways", 3, flags))
            {
                float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
                ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);                
                ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 16.0f);
                ImGui::TableSetupColumn("Samples", ImGuiTableColumnFlags_WidthFixed,  TEXT_BASE_WIDTH * 21.0f);
                ImGui::TableHeadersRow();
                ShowTreeStructure(rootSampleSource);
                ImGui::EndTable();
            }
        }
        //ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);        
        ImGui::TableNextColumn();
        if(markers.displayXMarkers || markers.displayYMarkers) ShowMarkerTable();
        ImGui::EndTable();
    }
    
   
    /*##########################################
    ###               Main plots             ###
    ############################################*/
    if (ImGui::BeginTabBar("Plots")) 
    {
        if(currentSampleSource != nullptr)
        {
            if (ImGui::BeginTabItem("Time domain")) 
            { 
                plotType = 0;
                if(prevPlotType != plotType)
                    updatePlots = true;
                td->Update(currentSampleSource, currentSampleIndex, markers, updatePlots);
                prevPlotType = 0;
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Power spectrum")) 
            { 
                plotType = 1;
                if(prevPlotType != plotType)
                    updatePlots = true;
                psd->Update(currentSampleSource, currentSampleIndex, markers, updatePlots);
                prevPlotType = 1;
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem("Spectrogram")) 
            { 
                plotType = 2;
                if(prevPlotType != plotType)
                    updatePlots = true;
                spectrogram->Update(currentSampleSource, currentSampleIndex, markers, updatePlots);
                prevPlotType = 2;
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Scatterplot")) 
            { 
                plotType = 3;
                if(prevPlotType != plotType)
                    updatePlots = true;
                scatterplot->Update(currentSampleSource, currentSampleIndex, markers, updatePlots);
                prevPlotType = 3;
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void MainGui::TreeStructureContextMenu(std::shared_ptr<AbstractSampleSource> src)
{
    std::string s = std::to_string(src->GetID());
    if (ImGui::BeginPopupContextItem(s.c_str())) // <-- use last item id as popup id
    {                
        //ImGui::Text("Add derived plots from %s", derivedSignals[i]->GetName().c_str());
        //FrequencyShiftStruct s;
        if(ImGui::MenuItem("Real"))
        {
            deriveSignal = true;
            deriveSignalSource = src;
            derivationType = REAL;
        }
        if(!src->RealSignal())
        {
            if(ImGui::MenuItem("Imag"))
            {
                deriveSignal = true;
                deriveSignalSource = src;
                derivationType = IMAG;
            }
        }
        if(ImGui::MenuItem("AM detect"))
        {
            deriveSignal = true;
            deriveSignalSource = src;
            derivationType = AM;
        }
        if(ImGui::MenuItem("FM detect"))
        {
            deriveSignal = true;
            deriveSignalSource = src;
            derivationType = FM;
        }
        if(ImGui::MenuItem("Phase detect"))
        {
            deriveSignal = true;
            deriveSignalSource = src;
            derivationType = PHASE;             
        }
        ImGui::Separator();
        if(ImGui::MenuItem("Frequency shift"))
        {
            //showError = true;
            //errorMsg = "Not implemented yet!";
            showFreqShiftWindow = true; 
            derivationType = FREQSHIFT;
            deriveSignalSource = src;
            showPopup = true;
        }                            
        if(ImGui::MenuItem("Resample"))
        {
            showResampleWindow = true; 
            derivationType = RESAMPLE;
            deriveSignalSource = src;
            showPopup = true;
        }
        if(ImGui::MenuItem("Filter"))
        {
            //showError = true;
            //errorMsg = "Not implemented yet!";
            showFilterWindow = true;
            deriveSignalSource = src;
            derivationType = FILTER;
            outputRate = deriveSignalSource->Rate()/2;
            cutoff = deriveSignalSource->Rate()/10;
            showPopup = true;
        }
        if(ImGui::MenuItem("DDC"))
        {
            //showError = true;
            //errorMsg = "Not implemented yet!";
            showDDCWindow = true;
            deriveSignalSource = src;
            derivationType = DDC;
            outputRate = deriveSignalSource->Rate()/2;
            cutoff = deriveSignalSource->Rate()/10;
            showPopup = true;
        }
        if(!src->RealSignal())
        {
            ImGui::Separator();
            if(ImGui::MenuItem("OFDM Demod"))
            {
                //showError = true;
                //errorMsg = "Not implemented yet!";
                showOFDMDemodWindow = true;
                deriveSignalSource = src;
                derivationType = OFDMDEMOD;
                ofdmDemodVars.sampleReference = currentSampleIndex;
                //ofdmSampleReference = currentSampleIndex;            
                showPopup = true;
            }
        }        
        ImGui::Separator();
        if(src->CanReconfigure())
        {
            if(ImGui::MenuItem("Reconfigure"))
            {
                deriveSignalSource = src;
                derivationType = src->GetSourceType();
                if(derivationType == INPUT)
                {
                    reconfigureFile = true;
                } else if(derivationType == FREQSHIFT)
                    showFreqShiftWindow = true;
                else if(derivationType == RESAMPLE)
                    showResampleWindow = true;
                else if(derivationType == FILTER)
                    showFilterWindow = true;
                else if(derivationType == DDC)
                    showDDCWindow = true;
                else if(derivationType == OFDMDEMOD)
                    showOFDMDemodWindow = true;
                reconfigure = true;
                showPopup = true;
            }
            ImGui::Separator();
        }
        if(ImGui::MenuItem("Save signal"))
        {
            deriveSignalSource = src;
            displayFileDialogSave = true;
            showPopup = true;
        }
        if(ImGui::MenuItem("Delete signal"))
        {
            showDeletePlotWindow = true;
            showPopup = true;
        }
        ImGui::EndPopup();
    }
}

void MainGui::ShowTreeStructure(std::shared_ptr<AbstractSampleSource> src)
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    if(src->subscriberCount()>0)
    {
        bool open = ImGui::TreeNodeEx(src->GetName().c_str(), ImGuiTreeNodeFlags_SpanFullWidth);
        ImGui::TableNextColumn();
        ImGui::TableNextColumn();
        if(open)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            std::string _s = src->GetName();//+std::to_string(src->GetID());
            if (ImGui::Selectable(_s.c_str()))//, currentSampleSource->GetID() == src->GetID()))//, ImGuiSelectableFlags_AllowDoubleClick))
            {
                //std::cout << "Current sample source: " << src->GetName() << src->GetID() << std::endl;
                if(currentSampleSource->GetID() != src->GetID())
                    updatePlots = true;
                currentSampleSource = src;
                
            }  
            TreeStructureContextMenu(src);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Double click to select derived plot. Right-click to open menu.");            
            ImGui::TableNextColumn();
            if(src->RealSignal()) ImGui::TextUnformatted("Real");
            else ImGui::TextUnformatted("Complex");
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(std::to_string(src->Count()).c_str());
            for (auto itr : src->GetSubscribers())
            {                
                ShowTreeStructure(itr);
            }
            ImGui::TreePop();
        }
        
    } else
    {
        std::string _s = src->GetName();//+std::to_string(src->GetID());
        if (ImGui::Selectable(_s.c_str()))//, currentSampleSource->GetID() == src->GetID()))//, ImGuiSelectableFlags_AllowDoubleClick))
        {
            //std::cout << "Current sample source: " << src->GetName() << src->GetID() << std::endl;
            if(currentSampleSource->GetID() != src->GetID())
                updatePlots = true;
            currentSampleSource = src;
        } 
        TreeStructureContextMenu(src);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Double click to select derived plot. Right-click to open menu.");
        ImGui::TableNextColumn();
        if(src->RealSignal()) ImGui::TextUnformatted("Real");
        else ImGui::TextUnformatted("Complex");
        ImGui::TableNextColumn();
        ImGui::TextUnformatted(std::to_string(src->Count()).c_str());
    }    
}

void MainGui::ShowMarkerTable()
{
    int numColumns = 0;
    if(markers.displayXMarkers) numColumns++;
    if(markers.displayYMarkers) numColumns++;
    ImGuiTableFlags markerflags = ImGuiTableFlags_Resizable;
    if(ImGui::BeginTable("markers", numColumns,markerflags))
    {
        float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
        if(markers.displayXMarkers) ImGui::TableSetupColumn("Xmarker", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 100.0f);
        if(markers.displayYMarkers) ImGui::TableSetupColumn("Ymarker", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 80.0f);
        ImGui::TableNextColumn();
        //ImGui::TableNextRow();
        //ImGui::BeginChild("Markertable",ImVec2(ImGui::GetWindowContentRegionWidth() * 0.3f, ImGui::GetWindowHeight()*0.15f));        
        ImGuiTableFlags markertableflags = ImGuiTableFlags_Borders;// | ImGuiTableFlags_RowBg;
        if(markers.displayXMarkers)
        {
            if (ImGui::BeginTable("Xmarkers", 4, markertableflags))
            {
                ImGui::TableSetupColumn("X markers");
                ImGui::TableSetupColumn("Samples");
                ImGui::TableSetupColumn("Time (sec)");
                ImGui::TableSetupColumn("1/Time");
                ImGui::TableHeadersRow();
                //Marker 0
                ImGui::TableNextColumn();
                ImGui::Text("M1");
                ImGui::TableNextColumn();
                ImGui::Text("%.4f", markers.XmarkerTD0);
                ImGui::TableNextColumn();
                ImGui::Text("%.4f", markers.XmarkerTD0/currentSampleSource->Rate());
                ImGui::TableNextColumn();
                ImGui::Text("%.4f", (markers.XmarkerTD0 == 0) ? 0 : currentSampleSource->Rate()/markers.XmarkerTD0);
                //Marker 1
                ImGui::TableNextColumn();
                ImGui::Text("M2");
                ImGui::TableNextColumn();
                ImGui::Text("%.4f", markers.XmarkerTD1);
                ImGui::TableNextColumn();
                ImGui::Text("%.4f", markers.XmarkerTD1/currentSampleSource->Rate());
                ImGui::TableNextColumn();
                ImGui::Text("%.4f", (markers.XmarkerTD1 == 0) ? 0 : currentSampleSource->Rate()/markers.XmarkerTD1);
                //Diff
                float diffx = markers.XmarkerTD1-markers.XmarkerTD0;
                ImGui::TableNextColumn();
                ImGui::Text("Diff");
                ImGui::TableNextColumn();
                ImGui::Text("%.4f", diffx);
                ImGui::TableNextColumn();
                ImGui::Text("%.4f", diffx/currentSampleSource->Rate());
                ImGui::TableNextColumn();
                ImGui::Text("%.4f", (diffx == 0) ? 0 : currentSampleSource->Rate()/diffx);
                //Center
                float centerx = (markers.XmarkerTD1+markers.XmarkerTD0)/2;
                ImGui::TableNextColumn();
                ImGui::Text("Center");
                ImGui::TableNextColumn();
                ImGui::Text("%.4f", centerx);
                ImGui::TableNextColumn();
                ImGui::Text("%.4f", centerx/currentSampleSource->Rate());
                ImGui::TableNextColumn();
                ImGui::Text("%.4f", (centerx == 0) ? 0 : currentSampleSource->Rate()/centerx);                
                ImGui::EndTable();
            }            
        }

        if(markers.displayYMarkers)
        {
            ImGui::TableNextColumn();
            if (ImGui::BeginTable("Ymarkers", 2, markertableflags))
            {                
                ImGui::TableSetupColumn("Y markers");
                ImGui::TableSetupColumn("Values");                
                ImGui::TableHeadersRow();
                //Marker 0
                ImGui::TableNextColumn();
                ImGui::Text("M1");
                ImGui::TableNextColumn();
                ImGui::Text("%.6f", markers.YmarkerTD0);                
                //Marker 1
                ImGui::TableNextColumn();
                ImGui::Text("M2");
                ImGui::TableNextColumn();
                ImGui::Text("%.6f", markers.YmarkerTD1);                
                //Diff
                ImGui::TableNextColumn();
                ImGui::Text("Diff");
                ImGui::TableNextColumn();
                ImGui::Text("%.6f", markers.YmarkerTD1-markers.YmarkerTD0);                
                //Center
                ImGui::TableNextColumn();
                ImGui::Text("Center");
                ImGui::TableNextColumn();
                ImGui::Text("%.6f", (markers.YmarkerTD1+markers.YmarkerTD0)/2);                
                
                ImGui::EndTable();
            }
        }        
        ImGui::EndTable();
    }
}

void MainGui::ShowSaveRange()
{
    ImGui::OpenPopup("Sample range");      
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f)); 
    bool close = false;
    if(ImGui::BeginPopupModal("Sample range"))
    {
        ImGui::RadioButton("Current view", &rangeType, 0); ImGui::SameLine();
        ImGui::RadioButton("X markers", &rangeType, 1); ImGui::SameLine();
        ImGui::RadioButton("Custom", &rangeType, 2); ImGui::SameLine();
        ImGui::RadioButton("All samples", &rangeType, 3); 
        if(rangeType == 0)
            switch (plotType)
            {
            case 0: //Time domain
                rangeMin = currentSampleIndex;
                rangeMax = currentSampleIndex + td->GetSamplesPerRow();
                break;
            case 1: //Power spectrum
                rangeMin = currentSampleIndex;
                rangeMax = currentSampleIndex + psd->GetSamplesPerRow();
                break;
            case 2: //Spectrogram
                rangeMin = currentSampleIndex;
                rangeMax = currentSampleIndex + spectrogram->GetSamplesPerRow();
                break;
            case 3: //Scatterplot
                rangeMin = currentSampleIndex;
                rangeMax = currentSampleIndex + scatterplot->GetSamplesPerRow();
                break;
            default:
                break;
            }
        else if(rangeType == 1)
        {
            rangeMin = markers.XmarkerTD0;
            rangeMax = markers.XmarkerTD1;
        }
        else if (rangeType == 2)
        {

        } 
        else if (rangeType == 3)
        {
            rangeMin = 0;
            rangeMax = -1;
        }
        if(rangeType != 2) ImGui::BeginDisabled();
        ImGui::InputInt("Min", &rangeMin);
        ImGui::InputInt("Max", &rangeMax);
        if(rangeType != 2) ImGui::EndDisabled();
        
        if (ImGui::Button("OK"))
        {
            ImGui::CloseCurrentPopup();
            outFilename = fileDialogSave.GetSelected().string();
            saveConfigured = true;
            close = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            saveConfigured = false;
            close = true;
        }
        ImGui::EndPopup();   
    }
    if(close)
    {
        fileDialogSave.Close();
        fileDialogSave.ClearSelected();  
    }
}

void MainGui::SaveSignal()
{
    std::ofstream ob (outFilename, std::ios::out|std::ios::binary|std::ios::ate);
    int numSamples = (rangeMax < 0) ? deriveSignalSource->Count() : rangeMax-rangeMin;    
    if(!deriveSignalSource->RealSignal())
    {
        int sampleSize = 8; //complex<float>
        std::shared_ptr<SampleSource<std::complex<float>>> concrete = std::dynamic_pointer_cast<SampleSource<std::complex<float>>>(deriveSignalSource);
        int numBlocks;
        if(numSamples == concrete->GetSaveSize())
            numBlocks = 1;
        else
            numBlocks = numSamples / concrete->GetSaveSize() + 1;        
        int start = rangeMin;
        for (size_t i = 0; i < numBlocks; i++)
        {
            int length = std::min(concrete->GetSaveSize(), numSamples);
            auto samples = concrete->GetSamples(start, length);
            start += length;
            numSamples -= length;
            ob.write(reinterpret_cast<const char*>(samples->data()), length*sampleSize);
        }
        
    } else
    {
        int sampleSize = 4; //float
        std::shared_ptr<SampleSource<float>> concrete = std::dynamic_pointer_cast<SampleSource<float>>(deriveSignalSource);
        int numBlocks;
        if(numSamples == concrete->GetSaveSize())
            numBlocks = 1;
        else
            numBlocks = numSamples / concrete->GetSaveSize() + 1; 
        int start = rangeMin;
        for (size_t i = 0; i < numBlocks; i++)
        {
            int length = std::min(concrete->GetSaveSize(), numSamples);
            auto samples = concrete->GetSamples(start, length);
            start += length;
            numSamples -= length;
            ob.write(reinterpret_cast<const char*>(samples->data()), length*sampleSize);
        }
    }
    ob.close();
    saveConfigured = false;
}

void MainGui::DeriveSignal()
{   
    if(derivationType == INPUT)
    {
        if(IsComplex(fileinfo.datatype))
        {                
            auto inputSource = std::make_shared<InputSource<std::complex<float>>>();
            inputSource->SetName("Input");
            inputSource->SetRate(fileinfo.sampleRate);         
            inputSource->SetReconfigure(true);
            inputSource->SetSourceType(INPUT);
            size_t size = inputSource->OpenFile(fileinfo);
            currentSampleSource = inputSource;    
            rootSampleSource = inputSource;            
            
        } else
        {
            auto inputSource = std::make_shared<InputSource<float>>();
            inputSource->SetName("Input");
            inputSource->SetRealSignal(true);
            inputSource->SetRate(fileinfo.sampleRate);   
            inputSource->SetReconfigure(true);
            inputSource->SetSourceType(INPUT);             
            size_t size = inputSource->OpenFile(fileinfo);
            currentSampleSource = inputSource;
            rootSampleSource = inputSource;     
        }                                   
        parseFile = false;            
        ImPlot::SetNextAxesToFit();
        showTree = true;
    }
    if(derivationType == REAL) //AM
    {
        if(!deriveSignalSource->RealSignal())
        {
            std::shared_ptr<SampleSource<std::complex<float>>> concrete = std::dynamic_pointer_cast<SampleSource<std::complex<float>>>(deriveSignalSource);
            auto realDetect = std::make_shared<RealDetect<std::complex<float>>>(concrete);                
            std::string name = concrete->GetName()+"->Real";
            realDetect->SetName(name);
            realDetect->SetRealSignal(true);
            realDetect->SetRate(concrete->Rate());
            realDetect->SetReconfigure(false);
            realDetect->SetSourceType(REAL);
            deriveSignalSource->subscribe(realDetect);
            //derivedSignals.push_back(amDetect);
        } else
        {
            std::shared_ptr<SampleSource<float>> concrete = std::dynamic_pointer_cast<SampleSource<float>>(deriveSignalSource);
            auto realDetect = std::make_shared<RealDetect<float>>(concrete);                
            std::string name = concrete->GetName()+"->Real";
            realDetect->SetName(name);
            realDetect->SetRealSignal(true);
            realDetect->SetRate(concrete->Rate());
            realDetect->SetReconfigure(false);
            realDetect->SetSourceType(REAL);
            deriveSignalSource->subscribe(realDetect);
        }
        
    }
    if(derivationType == IMAG) //AM
    {
        if(!deriveSignalSource->RealSignal())
        {
            std::shared_ptr<SampleSource<std::complex<float>>> concrete = std::dynamic_pointer_cast<SampleSource<std::complex<float>>>(deriveSignalSource);
            auto imagDetect = std::make_shared<ImagDetect<std::complex<float>>>(concrete);               
            std::string name = concrete->GetName()+"->Imag"; 
            imagDetect->SetName(name);
            imagDetect->SetRealSignal(true);
            imagDetect->SetRate(concrete->Rate());
            imagDetect->SetReconfigure(false);
            imagDetect->SetSourceType(IMAG);
            deriveSignalSource->subscribe(imagDetect);
        }
    }
    if(derivationType == AM) //AM
    {
        if(!deriveSignalSource->RealSignal())
        {
            std::shared_ptr<SampleSource<std::complex<float>>> concrete = std::dynamic_pointer_cast<SampleSource<std::complex<float>>>(deriveSignalSource);
            auto amDetect = std::make_shared<AMDetect<std::complex<float>>>(concrete);      
            std::string name = concrete->GetName()+"->AMDetect";          
            amDetect->SetName(name);
            amDetect->SetRealSignal(true);
            amDetect->SetRate(concrete->Rate());
            amDetect->SetReconfigure(false);
            amDetect->SetSourceType(AM);
            deriveSignalSource->subscribe(amDetect);
            //derivedSignals.push_back(amDetect);
        } else
        {
            std::shared_ptr<SampleSource<float>> concrete = std::dynamic_pointer_cast<SampleSource<float>>(deriveSignalSource);
            auto amDetect = std::make_shared<AMDetect<float>>(concrete);                
            std::string name = concrete->GetName()+"->AMDetect";          
            amDetect->SetName(name);
            amDetect->SetRealSignal(true);
            amDetect->SetRate(concrete->Rate());
            amDetect->SetReconfigure(false);
            amDetect->SetSourceType(AM);
            deriveSignalSource->subscribe(amDetect);
        }        
    }
    if(derivationType == FM) //FM
    {
        if(!deriveSignalSource->RealSignal())
        {
            std::shared_ptr<SampleSource<std::complex<float>>> concrete = std::dynamic_pointer_cast<SampleSource<std::complex<float>>>(deriveSignalSource);
            auto fmDetect = std::make_shared<FMDetect<std::complex<float>>>(concrete);                
            std::string name = concrete->GetName()+"->FMDetect";          
            fmDetect->SetName(name);
            fmDetect->SetRealSignal(true);
            fmDetect->SetRate(concrete->Rate());
            fmDetect->SetHistory(1);
            fmDetect->SetReconfigure(false);
            fmDetect->SetSourceType(FM);
            deriveSignalSource->subscribe(fmDetect);
            //derivedSignals.push_back(amDetect);
        } else
        {
            std::shared_ptr<SampleSource<float>> concrete = std::dynamic_pointer_cast<SampleSource<float>>(deriveSignalSource);
            auto fmDetect = std::make_shared<FMDetect<float>>(concrete);                
            std::string name = concrete->GetName()+"->FMDetect";          
            fmDetect->SetName(name);
            fmDetect->SetRealSignal(true);
            fmDetect->SetRate(concrete->Rate());
            fmDetect->SetHistory(1);
            fmDetect->SetReconfigure(false);
            fmDetect->SetSourceType(FM);
            deriveSignalSource->subscribe(fmDetect);
        }            
    }
    if(derivationType == PHASE) //Phase
    {
        if(!deriveSignalSource->RealSignal())
        {
            std::shared_ptr<SampleSource<std::complex<float>>> concrete = std::dynamic_pointer_cast<SampleSource<std::complex<float>>>(deriveSignalSource);
            auto phaseDetect = std::make_shared<PhaseDetect<std::complex<float>>>(concrete);                
            std::string name = concrete->GetName()+"->PhaseDetect";          
            phaseDetect->SetName(name);
            phaseDetect->SetRealSignal(true);
            phaseDetect->SetRate(concrete->Rate());
            phaseDetect->SetReconfigure(false);
            phaseDetect->SetSourceType(PHASE);
            deriveSignalSource->subscribe(phaseDetect);
            //derivedSignals.push_back(amDetect);
        } else
        {
            std::shared_ptr<SampleSource<float>> concrete = std::dynamic_pointer_cast<SampleSource<float>>(deriveSignalSource);
            auto phaseDetect = std::make_shared<PhaseDetect<float>>(concrete);                
            std::string name = concrete->GetName()+"->PhaseDetect";          
            phaseDetect->SetName(name);
            phaseDetect->SetRealSignal(true);
            phaseDetect->SetRate(concrete->Rate());
            phaseDetect->SetReconfigure(false);
            phaseDetect->SetSourceType(PHASE);
            deriveSignalSource->subscribe(phaseDetect);
        }            
    }
    if(derivationType == FREQSHIFT) //freq shift
    {
        if(!deriveSignalSource->RealSignal())
        {
            std::shared_ptr<SampleSource<std::complex<float>>> concrete = std::dynamic_pointer_cast<SampleSource<std::complex<float>>>(deriveSignalSource);
            auto freqShiftClass = std::make_shared<FreqShift<std::complex<float>>>(concrete, static_cast<float>(freqShift));
            std::string name = concrete->GetName()+"->FreqShift";          
            freqShiftClass->SetName(name);
            freqShiftClass->SetRealSignal(false);
            freqShiftClass->SetRate(concrete->Rate());
            freqShiftClass->SetReconfigure(true);
            freqShiftClass->SetSourceType(FREQSHIFT);
            deriveSignalSource->subscribe(freqShiftClass);
        } else
        {
            std::shared_ptr<SampleSource<float>> concrete = std::dynamic_pointer_cast<SampleSource<float>>(deriveSignalSource);
            auto freqShiftClass = std::make_shared<FreqShift<float>>(concrete, static_cast<float>(freqShift));              
            std::string name = concrete->GetName()+"->FreqShift";          
            freqShiftClass->SetName(name);
            freqShiftClass->SetRealSignal(true);
            freqShiftClass->SetRate(concrete->Rate());
            freqShiftClass->SetReconfigure(true);
            freqShiftClass->SetSourceType(FREQSHIFT);
            deriveSignalSource->subscribe(freqShiftClass);
        }            
    }
    if(derivationType == RESAMPLE) //resampling
    {
        if(!deriveSignalSource->RealSignal())
        {
            std::shared_ptr<SampleSource<std::complex<float>>> concrete = std::dynamic_pointer_cast<SampleSource<std::complex<float>>>(deriveSignalSource);
            auto resampler = std::make_shared<Resampler<std::complex<float>>>(concrete, outputRate, concrete->Rate(), 60.0f);
            std::string name = concrete->GetName()+"->Resampled";          
            resampler->SetName(name);
            resampler->SetRealSignal(false);
            resampler->SetRate(outputRate);
            resampler->SetReconfigure(true);
            resampler->SetSourceType(RESAMPLE);
            deriveSignalSource->subscribe(resampler);
        } else
        {
            std::shared_ptr<SampleSource<float>> concrete = std::dynamic_pointer_cast<SampleSource<float>>(deriveSignalSource);
            auto resampler = std::make_shared<Resampler<float>>(concrete, outputRate, concrete->Rate(), 60.0f);              
            std::string name = concrete->GetName()+"->Resampled";          
            resampler->SetName(name);
            resampler->SetRealSignal(true);
            resampler->SetRate(outputRate);
            resampler->SetReconfigure(true);
            resampler->SetSourceType(RESAMPLE);
            deriveSignalSource->subscribe(resampler);
        }            
    }
    if(derivationType == FILTER)
    {
        //std::cout << "Adding filter sub signal" << std::endl;
        if(!deriveSignalSource->RealSignal())
        {
            std::shared_ptr<SampleSource<std::complex<float>>> concrete = std::dynamic_pointer_cast<SampleSource<std::complex<float>>>(deriveSignalSource);
            auto filter = std::make_shared<FilterProc<std::complex<float>>>(concrete, filterVars);
            std::string name = concrete->GetName()+"->Filtered";          
            filter->SetName(name);
            filter->SetRealSignal(false);
            filter->SetRate(concrete->Rate());
            filter->SetReconfigure(true);
            filter->SetSourceType(FILTER);
            deriveSignalSource->subscribe(filter);
        } else
        {
            std::shared_ptr<SampleSource<float>> concrete = std::dynamic_pointer_cast<SampleSource<float>>(deriveSignalSource);
            auto filter = std::make_shared<FilterProc<float>>(concrete,  filterVars);
            std::string name = concrete->GetName()+"->Filtered";          
            filter->SetName(name);
            filter->SetRealSignal(true);
            filter->SetRate(concrete->Rate());
            filter->SetReconfigure(true);
            filter->SetSourceType(FILTER);
            deriveSignalSource->subscribe(filter);
        }  
    }
    if(derivationType == DDC) //resampling
    {
        if(!deriveSignalSource->RealSignal())
        {
            std::shared_ptr<SampleSource<std::complex<float>>> concrete = std::dynamic_pointer_cast<SampleSource<std::complex<float>>>(deriveSignalSource);
            auto ddc = std::make_shared<DDCProc<std::complex<float>>>(concrete, freqShift, outputRate, cutoff, attenuation);
            std::string name = concrete->GetName()+"->DDC";          
            ddc->SetName(name);
            ddc->SetRealSignal(false);
            ddc->SetRate(outputRate);
            ddc->SetReconfigure(true);
            ddc->SetSourceType(DDC);
            deriveSignalSource->subscribe(ddc);
        } else
        {
            std::shared_ptr<SampleSource<float>> concrete = std::dynamic_pointer_cast<SampleSource<float>>(deriveSignalSource);
            auto ddc = std::make_shared<DDCProc<float>>(concrete, freqShift, outputRate, cutoff, attenuation);
            std::string name = concrete->GetName()+"->DDC";          
            ddc->SetName(name);
            ddc->SetRealSignal(true);
            ddc->SetRate(outputRate);
            ddc->SetReconfigure(true);
            ddc->SetSourceType(DDC);
            deriveSignalSource->subscribe(ddc);
        }            
    }
    if(derivationType == OFDMDEMOD)
    {        
        if(!deriveSignalSource->RealSignal())
        {
            std::shared_ptr<SampleSource<std::complex<float>>> concrete = std::dynamic_pointer_cast<SampleSource<std::complex<float>>>(deriveSignalSource);
            //auto ofdmDemod = std::make_shared<OFDMDemod<std::complex<float>>>(concrete, ofdmFFTSize, ofdmCPLen, ofdmSymbolStart, ofdmSampleReference, ofdmInitWithFreqShift, ofdmCPBasedShift, ofdmNumSymsToEstimateFreqShift);
            auto ofdmDemod = std::make_shared<OFDMDemod<std::complex<float>>>(concrete, ofdmDemodVars);
            std::string name = concrete->GetName()+"->OFDMDemod";          
            ofdmDemod->SetName(name);
            ofdmDemod->SetRealSignal(false);
            ofdmDemod->SetRate(concrete->Rate());
            ofdmDemod->SetSaveSize(ofdmDemodVars.fftSize*100);
            ofdmDemod->SetReconfigure(true);
            ofdmDemod->SetSourceType(OFDMDEMOD);
            deriveSignalSource->subscribe(ofdmDemod);
        } else
        {
            std::cout << "Cannot perform OFDM demod for real signals" << std::endl;
        }                   
    }
    deriveSignal = false;
    deriveSignalSource = nullptr;
}

void MainGui::ReconfigureSignal()
{
    if(derivationType == INPUT)
    {

    }
    else if(derivationType == FREQSHIFT)
    {
        if(!deriveSignalSource->RealSignal())
        {
                std::shared_ptr<FreqShift<std::complex<float>>> concrete = std::dynamic_pointer_cast<FreqShift<std::complex<float>>>(deriveSignalSource);
                concrete->SetShift(freqShift);
        } else
        {
                std::shared_ptr<FreqShift<float>> concrete = std::dynamic_pointer_cast<FreqShift<float>>(deriveSignalSource);
                concrete->SetShift(freqShift);
        }
    }
    else if(derivationType == RESAMPLE)
    {
        if(!deriveSignalSource->RealSignal())
        {
                std::shared_ptr<Resampler<std::complex<float>>> concrete = std::dynamic_pointer_cast<Resampler<std::complex<float>>>(deriveSignalSource);
                concrete->SetOutputRate(outputRate);            
        } else
        {
                std::shared_ptr<Resampler<float>> concrete = std::dynamic_pointer_cast<Resampler<float>>(deriveSignalSource);
                concrete->SetOutputRate(outputRate);
        }
    }
    else if(derivationType == FILTER)
    {
        if(!deriveSignalSource->RealSignal())
        {
                std::shared_ptr<FilterProc<std::complex<float>>> concrete = std::dynamic_pointer_cast<FilterProc<std::complex<float>>>(deriveSignalSource);
                concrete->Reconfigure(filterVars);
        } else
        {
                std::shared_ptr<FilterProc<float>> concrete = std::dynamic_pointer_cast<FilterProc<float>>(deriveSignalSource);
                concrete->Reconfigure(filterVars);
        }
    }
    else if(derivationType == DDC)
    {
        if(!deriveSignalSource->RealSignal())
        {
                std::shared_ptr<DDCProc<std::complex<float>>> concrete = std::dynamic_pointer_cast<DDCProc<std::complex<float>>>(deriveSignalSource);
                concrete->Reconfigure(freqShift, outputRate, cutoff, attenuation);
        } else
        {
                std::shared_ptr<DDCProc<float>> concrete = std::dynamic_pointer_cast<DDCProc<float>>(deriveSignalSource);
                concrete->Reconfigure(freqShift, outputRate, cutoff, attenuation);
        }
    }
    else if(derivationType == OFDMDEMOD)
    {
        if(!deriveSignalSource->RealSignal())
        {
                std::shared_ptr<OFDMDemod<std::complex<float>>> concrete = std::dynamic_pointer_cast<OFDMDemod<std::complex<float>>>(deriveSignalSource);
                concrete->Reconfigure(ofdmDemodVars);
        } 
    }
    deriveSignal = false;
    reconfigure = false;
}


void MainGui::HandlePopups()
{
    if(!inputConfigured || reconfigureFile)
    {            
        ImGui::OpenPopup("Config");            
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(500), static_cast<float>(250)));//,ImGuiCond_FirstUseEver);
        if (ImGui::BeginPopupModal("Config"))
        {
            ImGui::Text("Configure input file");                
            ImGui::Text("Real / complex");
            ImGui::SameLine();
            ImGui::RadioButton("Complex", &fileinfo.realOrComplex, 0); ImGui::SameLine();
            ImGui::RadioButton("Real", &fileinfo.realOrComplex, 1);
            //SampleType st = FLOAT32;                
            ImGui::Text("Sample type");
            ImGui::SameLine();
            ImGui::RadioButton("8 bit signed integer", &fileinfo.sampleType, 0); ImGui::SameLine();                                   
            ImGui::RadioButton("16 bit integer", &fileinfo.sampleType, 1); ImGui::SameLine();                
            ImGui::RadioButton("32 bit float", &fileinfo.sampleType, 2);               
            //Endianness e = LITTLE_ENDIAN;                
            ImGui::Text("Byte order");
            ImGui::RadioButton("Little endian", &fileinfo.endianness, 0); ImGui::SameLine();
            ImGui::RadioButton("Big endian", &fileinfo.endianness, 1);
            fileinfo.datatype = FileConfigureToDataType(fileinfo.realOrComplex, fileinfo.sampleType, fileinfo.endianness);
            fileinfo.supportedFormat = IsSupported(fileinfo.datatype);
            ImGui::Text("Sampling rate");
            ImGui::SameLine();
            ImGui::InputDouble("##samplingrateinput", &fileinfo.sampleRate);
            if(fileinfo.detectedFileType != "") ImGui::TextUnformatted(("Detected file type: " + fileinfo.detectedFileType).c_str());
            if (ImGui::Button("Save"))
            {
                if(!fileinfo.supportedFormat)
                {
                    showError = true;
                    errorMsg = "File type is not supported yet!";                
                }                       
                inputConfigured = true;
                displayFileDialogOpen = false;                     
                //reconfigure = false;
                parseFile = true;  
                showPopup = false;
                ImGui::CloseCurrentPopup();                 
            }
            ImGui::SameLine();     
            if (ImGui::Button("Cancel"))
            {
                //inputConfigured = true;
                if(reconfigureFile)
                    displayFileDialogOpen = false;
                else
                    displayFileDialogOpen = true;
                reconfigureFile = false;
                showPopup = false;
                ImGui::CloseCurrentPopup();
            }               
            ImGui::EndPopup();
        }            
    }
    if(showError)
    {
        ShowError(showError, errorMsg);
        if(!showError)
            showPopup = false;
    }

    
    if(showFreqShiftWindow)
    {
        ShowFrequencyShiftMenu(&freqShift, showFreqShiftWindow, deriveSignal);            
        if(!showFreqShiftWindow)
            showPopup = false;
    }

    if(showResampleWindow)
    {
        ShowResampleMenu(&outputRate, showResampleWindow, deriveSignal);            
        if(!showResampleWindow)
            showPopup = false;
    }

    if(showFilterWindow)
    {   
        ShowFilterMenu(filterVars, showFilterWindow, deriveSignal);
        if(!showFilterWindow)
            showPopup = false;
    }

    if(showDDCWindow)
    {
        ShowDDCMenu(&freqShift, &outputRate, &cutoff, &attenuation, showDDCWindow, deriveSignal);
        if(!showDDCWindow)
            showPopup = false;
    }

    if(showOFDMDemodWindow)
    {
        //ShowOFDMDemodMenu(&ofdmFFTSize, &ofdmCPLen, &ofdmSymbolStart, &ofdmSampleReference, ofdmInitWithFreqShift, ofdmCPBasedShift, ofdmEnableCustomSampleReference, &ofdmNumSymsToEstimateFreqShift, showOFDMDemodWindow, deriveSignal);
        ShowOFDMDemodMenu(ofdmDemodVars, ofdmEnableCustomSampleReference, showOFDMDemodWindow, deriveSignal);
        if(!showOFDMDemodWindow)
            showPopup = false;
    }

    if(showDeletePlotWindow)       
    {        
        ImGui::OpenPopup("Delete plot");      
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));    
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(150), static_cast<float>(100)),ImGuiCond_FirstUseEver);   
        if(ImGui::BeginPopupModal("Delete plot"))
        {                
            ImGui::Text("Are you sure you want to delete");
            ImGui::Text("%s", currentSampleSource->GetName().c_str());                           
            if (ImGui::Button("OK"))
            {               
                //Delete all children, childrens children etc
                DeleteSource(currentSampleSource);                
                showDeletePlotWindow = false;                
                ImGui::CloseCurrentPopup();                                
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {        
                showDeletePlotWindow = false;                
                ImGui::CloseCurrentPopup();                                
            }
            ImGui::EndPopup();   
        }
        if(!showDeletePlotWindow)    
            showPopup = false;
    }    
}

void MainGui::DeleteSource(std::shared_ptr<AbstractSampleSource> src)
{
    DeleteChildren(src);
       
    if(src == rootSampleSource)
    {
        //Has no parent
        src->~AbstractSampleSource();
        currentSampleSource = nullptr;
        showTree = false;        
    } else
    {
        //Find parent
        auto parent = FindParent(src);
        parent->unsubscribe(src); //This also invokes destructor of src    
        currentSampleSource = parent;
    }   
}

void MainGui::DeleteChildren(std::shared_ptr<AbstractSampleSource> src)
{
    if(src->subscriberCount()>0)
    {
        for(auto itr : src->GetSubscribers())
        {
            if(itr->subscriberCount()>0)
                DeleteSource(itr);
            else  
            {
                src->unsubscribe(itr);                
            }                  
        }            
    }
}

std::shared_ptr<AbstractSampleSource> MainGui::FindParent(std::shared_ptr<AbstractSampleSource> child)
{
    return FindParentFromSrc(rootSampleSource, child);
}

std::shared_ptr<AbstractSampleSource> MainGui::FindParentFromSrc(std::shared_ptr<AbstractSampleSource> src1,std::shared_ptr<AbstractSampleSource> src2)
{    

    if(MatchSources(src1, src2))
        return src1;
    else  
    {  
        for (auto itr : src1->GetSubscribers())
        {
            return FindParentFromSrc(itr, src2);
        }
    }
    return nullptr;
}

bool MainGui::MatchSources(std::shared_ptr<AbstractSampleSource> src1, std::shared_ptr<AbstractSampleSource> src2)
{    
    for(auto itr : src1->GetSubscribers())
        if(itr == src2)
            return true;
    return false;
}

