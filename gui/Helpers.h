//#include "imgui.h"
#include "imgui_internal.h"
#include "Parser/exprtk.hpp"
#include <iostream>
#include "misc/cpp/imgui_stdlib.h"
#include <sys/stat.h>
#include <sstream>
#include <fstream>
#include "Structs.h"
#include <map>

typedef exprtk::expression<double> expression_t;
typedef exprtk::parser<double>         parser_t;

namespace gui
{
    bool InputSampleIndex(int &sampleIndex, int &samplesPerRow, int numSamples);

    bool InputSamplesPerRow(int &sampleIndex, int &samplesPerRow, int numSamples);

    bool InputStrAndEvaluateToInt(const char *label, std::string &value);    

    bool InputStrAndEvaluateToDouble(const char *label, std::string &value); 

    bool LoadSettings(Settings &settings, std::string filename);

    bool SetSettingsPath(std::string path, std::string filename);

    bool CreateConfig(std::vector<std::string> &lines, std::map<std::string, std::string> &settingsMap);

}