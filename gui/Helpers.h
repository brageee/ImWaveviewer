//#include "imgui.h"
#include "imgui_internal.h"
#include "Parser/exprtk.hpp"
#include <iostream>
#include "misc/cpp/imgui_stdlib.h"

typedef exprtk::expression<double> expression_t;
typedef exprtk::parser<double>         parser_t;

namespace gui
{
    bool InputSampleIndex(int &sampleIndex, int &samplesPerRow, int numSamples);

    bool InputSamplesPerRow(int &sampleIndex, int &samplesPerRow, int numSamples);

    bool InputStrAndEvaluateToInt(const char *label, std::string &value);    

    bool InputStrAndEvaluateToDouble(const char *label, std::string &value); 

}