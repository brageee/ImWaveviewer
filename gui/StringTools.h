#pragma once

#include <stdio.h>
#include <cmath>
#include <mutex>
#include <map>
#include <sstream>
#include <iomanip>
#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <ctime>

namespace gui
{
    static inline std::string formatSIValue(double value, unsigned int decimals = 6)
    {
        std::map<int, std::string> prefixes =
        {
            {  9,   "G" },
            {  6,   "M" },
            {  3,   "k" },
            {  0,   ""  },
            { -3,   "m" },
            { -6,   "u" },
            { -9,   "n" },
        };

        bool isNegative = (value < 0.0f) ? true: false;

        if(isNegative) value = value * -1.0f;

        int power = 0;
        while (value < 1.0f && power > -9)
        {
            value *= 1e3;
            power -= 3;
        }
        while (value >= 1e3 && power < 9)
        {
            value *= 1e-3;
            power += 3;
        }

        if(isNegative) value = value * -1.0f;
        
        std::ostringstream streamObj;
        // Set Fixed -Point Notation
        streamObj << std::fixed;
        // Set precision
        streamObj << std::setprecision(decimals);
        //Add double to stream
        streamObj << value;

        return streamObj.str() + prefixes[power];
    }


    static inline bool is_number(const std::string& s)
    {
        try
        {
            std::stod(s);
        }
        catch(...)
        {
            return false;
        }
        return true;
    }

}