#include "Helpers.h"

namespace gui
{
    bool InputSampleIndex(int &sampleIndex, int &samplesPerRow, int numSamples)
    {
        bool res = false;
        if(ImGui::InputInt("##currentsampleindex", &sampleIndex))
        {
           res = true;
           if(sampleIndex < 0)
            sampleIndex = 0;
           if(sampleIndex+samplesPerRow >= numSamples)
                samplesPerRow = numSamples-sampleIndex;            
        }
        return res;
    }

    bool InputSamplesPerRow(int &sampleIndex, int &samplesPerRow, int numSamples)
    {
        bool res = false;
        if(ImGui::InputInt("##numsamplesperrow", &samplesPerRow))
        {
            res = true;
            if(sampleIndex+samplesPerRow >= numSamples)
                sampleIndex = numSamples-samplesPerRow;
            if(sampleIndex < 0)
            {
                sampleIndex = 0;
                samplesPerRow = numSamples;
            }
            if(samplesPerRow<1)
                samplesPerRow = 1;                             
        }
        return res;
    }

    bool InputStrAndEvaluateToInt(const char *label, std::string &value)
    {
        bool res = false;
        std::string backup = value;        
        bool evaluate = false;
        double dresult = 0.0;        
        if(ImGui::InputText(label, &value))        
        {
            evaluate = true;
            res = true;
        }
        if(evaluate)
        {            
            expression_t expression;
            parser_t parser;

            if (parser.compile(value,expression))
            {
                dresult= expression.value();
                //std::cout << "Result was " << dresult << std::endl;
                int result = static_cast<int>(dresult);
                value = std::to_string(result);
                //printf("Result: %19.15 \n",dresult);
            }
            else
            {
                //printf("Error in expression. \n");
                value = backup;
            }                
        }        
        return res;
    }

    bool InputStrAndEvaluateToDouble(const char *label, std::string &value)
    {
        bool res = false;
        std::string backup = value;        
        bool evaluate = false;
        double result = 0.0;        
        if(ImGui::InputText(label, &value))        
        {
            evaluate = true;
            res = true;
        }
        if(evaluate)
        {            
            expression_t expression;
            parser_t parser;

            if (parser.compile(value,expression))
            {
                result= expression.value();                
                value = std::to_string(result);             
            }
            else
            {                
                value = backup;
            }                
        }        
        return res;
    }
} // namespace gui
