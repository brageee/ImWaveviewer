cmake_minimum_required(VERSION 3.11.0)
project(waveviewer VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
#add_compile_options(-O3 -march=native)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
find_package(FFTW REQUIRED)
find_package(LIQUID REQUIRED)

include_directories(
    ${FFTW_INCLUDES}
    ${LIQUID_INCLUDES}
)


# GUI code
include_directories(gui/)

# Processing and helpers code
include_directories(processing/)

# GLAD
#include_directories(glad/include/)
add_subdirectory(glad)

#Fonts
include_directories(common/)

###############################################################################
# IMGUI
###############################################################################

set(IMGUI_HEADERS
    imgui/imconfig.h
    imgui/imgui_internal.h
    imgui/imgui.h
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h    
    imgui/backends/imgui_impl_glfw.h
    imgui/backends/imgui_impl_opengl3.h
    imgui/backends/imgui_impl_opengl3_loader.h
    imgui/misc/cpp/imgui_stdlib.h
)

set(IMGUI_SRC
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/misc/cpp/imgui_stdlib.cpp
)



add_library(imgui ${IMGUI_HEADERS} ${IMGUI_SRC})
if(MSVC)
  target_compile_options(imgui PRIVATE /W4 /WX /arch:AVX2 /fp:fast)
endif()
if (WIN32)
  target_link_libraries(imgui PUBLIC glfw glad OpenGL::GL imm32)
else()
  target_link_libraries(imgui PUBLIC glfw glad OpenGL::GL dl)
endif()
target_compile_definitions(imgui PRIVATE IMGUI_DLL_EXPORT)

include_directories(imgui/ imgui/examples imgui/examples/libs/gl3w imgui/backends imgui/misc/cpp)

##########################################
#imgui filebrowser
#########################################
include_directories(imgui-filebrowser/)

###############################################################################
# IMPLOT
###############################################################################

set(IMPLOT_HEADERS implot/implot.h implot/implot_internal.h)
set(IMPLOT_SRC     implot/implot.cpp implot/implot_items.cpp) 

add_library(implot ${IMPLOT_HEADERS} ${IMPLOT_SRC})
target_link_libraries(implot PUBLIC imgui)
target_compile_definitions(implot PUBLIC IMPLOT_DEBUG IMPLOT_DLL_EXPORT IMPLOT_BACKEND_ENABLE_OPENGL3 IMGUI_IMPL_OPENGL_LOADER_GLAD)
set_property(TARGET implot PROPERTY CXX_STANDARD 11)
if(MSVC)
  target_compile_options(implot PRIVATE /W4 /WX /arch:AVX2 /fp:fast /permissive-) 
else()
  target_compile_options(implot PRIVATE -Wall -Wextra -pedantic -Werror -mavx2 -Ofast)
endif()

include_directories(implot/)

# waveviewer
list(APPEND project_sources
  main.cpp
  gui/MainGui.cpp
  gui/PopupsAndModals.cpp
  gui/TimeDomain.cpp
  gui/PowerSpectrum.cpp
  gui/Spectrogram.cpp
  gui/Scatterplot.cpp
  gui/Helpers.cpp
  processing/Filter.cpp
  processing/Transformations.cpp
  processing/Windowing.cpp
  processing/InputSource.cpp
  processing/AbstractSampleSource.cpp
  processing/SampleSource.cpp
  processing/SampleProc.cpp
  processing/RealDetect.cpp
  processing/ImagDetect.cpp
  processing/AMDetect.cpp
  processing/FMDetect.cpp
  processing/PhaseDetect.cpp
  processing/FreqShift.cpp
  processing/Resampler.cpp
  processing/FilterProc.cpp
  processing/DDC.cpp
  processing/OFDMDemod.cpp
  processing/Utils.cpp
  processing/TakeSkip.cpp
  gui/Fonts/FontAwesome5.cpp
  gui/Fonts/FontAwesome5Brands.cpp
  gui/Fonts/RobotoBold.cpp
  gui/Fonts/RobotoItalic.cpp
  gui/Fonts/RobotoMonoBold.cpp
  gui/Fonts/RobotoMonoItalic.cpp
  gui/Fonts/RobotoMonoRegular.cpp
  gui/Fonts/RobotoRegular.cpp

)

add_executable(waveviewer ${project_sources} )
target_link_libraries(waveviewer implot ${FFTW_LIBRARIES} ${LIQUID_LIBRARIES})
target_compile_features(waveviewer PRIVATE cxx_std_17)
    