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
} // namespace gui
