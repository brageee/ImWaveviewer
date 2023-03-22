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
#include "StringTools.h"
#include "../processing/Structs.h"

namespace gui
{
    inline bool getKingFileMetadata(std::string filename, FileInfo &fm)
    {
        // Example filename: 20201231_235959_1.000000MSPS_123.000000MHz.fc32
        // Example filename: 20201231_235959_1.000000MSPS_123.000000MHz_interestingFile.fc32
        // search for date_time_rateXSPS_freqXHz.type
        // where:
        // date = yyyymmdd
        // time = HHmmss
        // rate = 123.123456
        // XSPS = X Samples Per Second
        // freq = 123.123456
        // XHz = X Hertz (X => G (e9), M (e6), k (e3), m (e-3), u (e-6), n (e-9))
        // optional characters such as file comments can be placed after Hz and .type
        // type = cfile, cf32, fc32, cs8, sc8, c8, cs16, sc16, c16, cu8, uc8, f32, s8, s16, u8, uc

        fm.filename = filename;

        std::string regexPattern = "(\\d{8})_(\\d{6})_(\\d{1,3}.\\d{0,6})(.)SPS_(\\d{1,3}.\\d{0,6})(.)Hz.*[.](\\b(cfile|cf32|fc32|cs16|sc16|c16|cs8|sc8|c8|cu8|uc8|f32|s8|s16|u8|uc)\\b)";

        std::smatch sm;

        std::string baseName = filename.substr(filename.find_last_of("/\\") + 1); // get only filename from path

        if (std::regex_match (baseName, sm, std::regex(regexPattern) ))
        {
            if(sm.size() >= 7)
            {
                // Convert time stamp from filename
                std::string fileTs = sm[1].str() + "_" + sm[2].str(); // Combine year date and time into one string

                struct tm fileTm;

                if(strptime(fileTs.c_str(), "%Y%m%d_%H%M%S", &fileTm))
                {
                    // Skipping timestamp
                    fm.timestampStart = timegm(&fileTm);
                }
                else
                {
                    // Could not detect time stamp. Filename is not a valid king file
                    return false;
                }
                

                // Convert rate from filename
                if(!is_number(sm[3].str()))
                {
                    // Could not detect sample rate. Filename is not a valid king file
                    return false;
                }
                double sampleRate = std::stod(sm[3].str());
                std::string rateMultiplier = sm[4].str();
                if(rateMultiplier == "G")       sampleRate *= 1000000000;
                else if(rateMultiplier == "M")  sampleRate *= 1000000;
                else if(rateMultiplier == "k")  sampleRate *= 1000;
                else if(rateMultiplier == "m")  sampleRate /= 1000;
                else if(rateMultiplier == "u")  sampleRate /= 1000000;
                else if(rateMultiplier == "n")  sampleRate /= 1000000000;
                else
                {
                    // Could not detect sample rate. Filename is not a valid king file
                    return false;
                }
                
                fm.sampleRate = sampleRate;

                // Convert frequency from filename
                if(!is_number(sm[5].str()))
                {
                    // Could not detect frequency. Filename is not a valid king file
                    return false;
                }
                double frequency = std::stod(sm[5].str());
                std::string freqMultiplier = sm[6].str();
                if(freqMultiplier == "G")       frequency *= 1000000000;
                else if(freqMultiplier == "M")  frequency *= 1000000;
                else if(freqMultiplier == "k")  frequency *= 1000;
                else if(freqMultiplier == "m")  frequency /= 1000;
                else if(freqMultiplier == "u")  frequency /= 1000000;
                else if(freqMultiplier == "n")  frequency /= 1000000000;
                else
                {
                    // Could not detect frequency. Filename is not a valid king file
                    return false;
                }

                fm.frequency = frequency;

                // Get filetype from filename
                if(sm[7].str() == "u8" || sm[7].str() == "uc")                                      { fm.datatype = REAL_UINT8;                     fm.sampleType = 0; fm.endianness = 0; }
                else if(sm[7].str() == "s8")                                                        { fm.datatype = REAL_INT8;                      fm.sampleType = 0; fm.endianness = 0; }
                else if(sm[7].str() == "s16")                                                       { fm.datatype = REAL_INT16_LITTLE_ENDIAN;       fm.sampleType = 1; fm.endianness = 0; }
                else if(sm[7].str() == "f32")                                                       { fm.datatype = REAL_FLOAT32_BYTE_SWAPPED;      fm.sampleType = 2; fm.endianness = 0; }
                else if(sm[7].str() == "uc8" || sm[7].str() == "cu8")                               { fm.datatype = COMPLEX_UINT8;                  fm.sampleType = 0; fm.endianness = 0; }
                else if(sm[7].str() == "sc8" || sm[7].str() == "cs8" || sm[7].str() == "c8")        { fm.datatype = COMPLEX_INT8;                   fm.sampleType = 0; fm.endianness = 0; }
                else if(sm[7].str() == "sc16" || sm[7].str() == "cs16" || sm[7].str() == "c16")     { fm.datatype = COMPLEX_INT16_LITTLE_ENDIAN;    fm.sampleType = 1; fm.endianness = 0; }
                else if(sm[7].str() == "cfile" || sm[7].str() == "cf32" || sm[7].str() == "fc32")   { fm.datatype = COMPLEX_FLOAT32_BYTE_SWAPPED;   fm.sampleType = 2; fm.endianness = 0; }
                else
                {
                    // Could not detect sample type. Filename is not a valid king file
                    return false;
                }
            }
        }
        else
        {
            return false;
        }

        fm.detectedFileType = "KingFile";
        
        return true;
    }


    // comment should not include "_"
    inline std::string generateKingFileNameWithComment(double sampleRate, double frequency, std::string fileTypeExtension, std::string comment = "")
    {
        std::string fname;
        
        time_t rawtime;
        struct tm * timeinfo;
        char buffer[80];

        //Create timestamp string for filename
        time (&rawtime);
        timeinfo = localtime(&rawtime);

        strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", timeinfo);
        std::string timestampString(buffer);

        //Put together filename
        if(comment != "")
        {
            fname = timestampString + "_" + formatSIValue(sampleRate) + "SPS_" + formatSIValue(frequency) + "Hz_" + comment + "." + fileTypeExtension;
        }
        else
        {
            fname = timestampString + "_" + formatSIValue(sampleRate) + "SPS_" + formatSIValue(frequency) + "Hz." + fileTypeExtension;
        }
        
        return fname;
    }

    inline bool getWaveviewerMetadata(std::string filename, FileInfo &fm)
    {
        // Example filename: dumbstuff.complex.1000.32fl
        // Example filename: dumbstuff.complex.1000.fc32
        // Example filename: dumbstuff.complex.1000.float
        // Example filename: dumbstuff.complex.1000.16t
        // Example filename: dumbstuff.complex.1000.sc16
        // Example filename: dumbstuff.complex.1000.16tl
        // Example filename: dumbstuff.real.1000.float

        // search for <real><complex><cplx>.<sample rate>.<type>
        // where:
        // type = cfile, cf32, fc32, cs8, sc8, c8, cs16, sc16, c16, cu8, uc8, f32, s8, s16, u8, uc

        fm.filename = filename;
        bool parsed = false;        
        std::string regexPattern = "\\b(real|cplx|complex).\\d{1,20}.\\d{0,2}.\\b(cf32|fc32|cs16|sc16|c16|cs8|sc8|c8|cu8|uc8|f32|s8|s16|u8|uc|8t|16tl|16t|32fl|32f)"; 
        std::smatch sm;

        std::string baseName = filename.substr(filename.find_last_of("/\\") + 1); // get only filename from path
        //std::cout << "Filename " << baseName << std::endl;
        bool _int8 = false;
        bool _int16 = false;
        bool _float32 = false;
        
        if (std::regex_search (baseName, sm, std::regex(regexPattern) ))
        {
            parsed = true;
            //Found file type
            //std::cout << "Found file parser " << sm.str() << std::endl;  
            size_t firstDot = sm.str().find_first_of(".");
            size_t lastDot = sm.str().find_last_of(".");
            std::string realOrComplex = sm.str().substr(0, firstDot); // get only filename from path
            std::string sampleRateStr = sm.str().substr(firstDot+1, lastDot-firstDot-1);
            std::string type = sm.str().substr(lastDot+1);
            //std::cout << "real or complex: " << realOrComplex << std::endl;        
            //std::cout << "sample rate: " << sampleRateStr << std::endl;    
            //std::cout << "type: " << type << std::endl;        
            if(type == "u8" || type == "uc")                                             { fm.datatype = REAL_UINT8;                     fm.sampleType = 0; fm.endianness = 0; }
            else if(type == "s8")                                                        { fm.datatype = REAL_INT8;                      fm.sampleType = 0; fm.endianness = 0; }
            else if(type == "s16")                                                       { fm.datatype = REAL_INT16_LITTLE_ENDIAN;       fm.sampleType = 1; fm.endianness = 0; }
            else if(type == "f32")                                                       { fm.datatype = REAL_FLOAT32_BYTE_SWAPPED;      fm.sampleType = 2; fm.endianness = 0; }
            else if(type == "uc8" || type == "cu8")                                      { fm.datatype = COMPLEX_UINT8;                  fm.sampleType = 0; fm.endianness = 0; }
            else if(type == "sc8" || type == "cs8" || type == "c8")                      { fm.datatype = COMPLEX_INT8;                   fm.sampleType = 0; fm.endianness = 0; }
            else if(type == "sc16" || type == "cs16" || type == "c16")                   { fm.datatype = COMPLEX_INT16_LITTLE_ENDIAN;    fm.sampleType = 1; fm.endianness = 0; }
            else if(type == "cfile" || type == "cf32" || type == "fc32")                 { fm.datatype = COMPLEX_FLOAT32_BYTE_SWAPPED;   fm.sampleType = 2; fm.endianness = 0; }
            else if(type == "8t")                                                        { fm.sampleType = 0; fm.endianness = 0; _int8 = true; }            
            else if(type == "16t")                                                       { fm.sampleType = 1; fm.endianness = 1;  _int16 = true;           }
            else if(type == "16tl")                                                      { fm.sampleType = 1; fm.endianness = 0;   _int16 = true;         }
            else if(type == "32f")                                                       { fm.sampleType = 2; fm.endianness = 1;  _float32 = true;          }
            else if(type == "32fl")                                                      { fm.sampleType = 2; fm.endianness = 0;  _float32 = true;           }            
        
            //Found real or complex
            if(realOrComplex == "cplx" || realOrComplex == "complex")     
            {
                fm.realOrComplex = 0;
                if(_int8) fm.datatype = COMPLEX_UINT8;                
                if(_int16)
                {
                    if(fm.endianness == 0) 
                        fm.datatype = COMPLEX_INT16_LITTLE_ENDIAN;
                    else
                        fm.datatype = COMPLEX_INT16_BIG_ENDIAN;
                }
                if(_float32)
                {
                    if(fm.endianness == 0) 
                        fm.datatype = COMPLEX_FLOAT32_BYTE_SWAPPED;
                    else
                        fm.datatype = COMPLEX_FLOAT32_BYTE_SWAPPED;
                }
            } else
            {
                fm.realOrComplex = 1;
                if(_int8) fm.datatype = REAL_INT8;                
                if(_int16)
                {
                    if(fm.endianness == 0) 
                        fm.datatype = REAL_INT16_LITTLE_ENDIAN;
                    else
                        fm.datatype = REAL_INT16_BIG_ENDIAN;
                }
                if(_float32)
                {
                    if(fm.endianness == 0) 
                        fm.datatype = REAL_FLOAT32_BYTE_SWAPPED;
                    else
                        fm.datatype = REAL_FLOAT32;
                }
            }

            //Sample rate
            if(firstDot != lastDot)
            {
                std::string letter = "[kMG]";
                std::smatch smm;
                float scale = 0.0f;
                if (std::regex_search (sampleRateStr, smm, std::regex(letter) ))
                {
                    if(smm.str() == "k") scale = 1000.0f;
                    else if(smm.str() == "M") scale = 1000000.0f;
                    else if(smm.str() == "G") scale = 1000000000.0f;
                }
                float sampleRate = std::stof(sampleRateStr) * scale;
                //check valid number
                fm.sampleRate = sampleRate;
            }
        }

        fm.detectedFileType = "Waveviewer";
        
        return parsed;
    }

    inline bool getAnyMetadata(std::string filename, FileInfo &fm)
    {
        // Example filename: dumbstuff.complex.32fl
        // Example filename: dumbstuff.real.16t
        // Example filename: dumbstuff.real.something.16t

        // search for <real><complex><cplx>
        // search for <type>
        // where:
        // type = cfile, cf32, fc32, cs8, sc8, c8, cs16, sc16, c16, cu8, uc8, f32, s8, s16, u8, uc

        fm.filename = filename;
        bool parsed = false;        
        std::string regexPatternType = "\\b(cf32|fc32|cs16|sc16|c16|cs8|sc8|c8|cu8|uc8|f32|s8|s16|u8|uc|8t|16tl|16t|32fl|32f)";        
        std::string regexPatternReal = "\\b(real|cplx|complex)";
        std::smatch sm;

        std::string baseName = filename.substr(filename.find_last_of("/\\") + 1); // get only filename from path
        std::cout << "Filename " << baseName << std::endl;
        bool typeSet = false;
        bool sampleRateSet = false;
        bool realComplecSet = false;
        bool _int8 = false;
        bool _int16 = false;
        bool _float32 = false;
        
        if (std::regex_search (baseName, sm, std::regex(regexPatternType) ))
        {
            parsed = true;
            //Found file type
            std::cout << "Found file metadata " << sm.str() << std::endl; 
            //std::string sampleRateStr = 

            if(sm.str() == "u8" || sm.str() == "uc")                                         { fm.datatype = REAL_UINT8;                     fm.sampleType = 0; fm.endianness = 0; }
            else if(sm.str() == "s8")                                                        { fm.datatype = REAL_INT8;                      fm.sampleType = 0; fm.endianness = 0; }
            else if(sm.str() == "s16")                                                       { fm.datatype = REAL_INT16_LITTLE_ENDIAN;       fm.sampleType = 1; fm.endianness = 0; }
            else if(sm.str() == "f32")                                                       { fm.datatype = REAL_FLOAT32_BYTE_SWAPPED;      fm.sampleType = 2; fm.endianness = 0; }
            else if(sm.str() == "uc8" || sm.str() == "cu8")                                  { fm.datatype = COMPLEX_UINT8;                  fm.sampleType = 0; fm.endianness = 0; }
            else if(sm.str() == "sc8" || sm.str() == "cs8" || sm.str() == "c8")              { fm.datatype = COMPLEX_INT8;                   fm.sampleType = 0; fm.endianness = 0; }
            else if(sm.str() == "sc16" || sm.str() == "cs16" || sm.str() == "c16")           { fm.datatype = COMPLEX_INT16_LITTLE_ENDIAN;    fm.sampleType = 1; fm.endianness = 0; }
            else if(sm.str() == "cfile" || sm.str() == "cf32" || sm.str() == "fc32")         { fm.datatype = COMPLEX_FLOAT32_BYTE_SWAPPED;   fm.sampleType = 2; fm.endianness = 0; }
            else if(sm.str() == "8t")                                                        {fm.sampleType = 0; fm.endianness = 0; _int8 = true; }            
            else if(sm.str() == "16t")                                                       { fm.sampleType = 1; fm.endianness = 1;  _int16 = true;           }
            else if(sm.str() == "16tl")                                                      { fm.sampleType = 1; fm.endianness = 0;   _int16 = true;         }
            else if(sm.str() == "32f")                                                       { fm.sampleType = 2; fm.endianness = 1;  _float32 = true;          }
            else if(sm.str() == "32fl")                                                      { fm.sampleType = 2; fm.endianness = 0;  _float32 = true;           }            
        }
        if (std::regex_search (baseName, sm, std::regex(regexPatternReal) ))
        {
            //Found real or complex
            parsed = true;
            std::cout << "File is " << sm.str() << std::endl;            
            if(sm.str() == "cplx" || sm.str() == "complex")     
            {
                fm.realOrComplex = 0;
                if(_int8) fm.datatype = COMPLEX_UINT8;                
                if(_int16)
                {
                    if(fm.endianness == 0) 
                        fm.datatype = COMPLEX_INT16_LITTLE_ENDIAN;
                    else
                        fm.datatype = COMPLEX_INT16_BIG_ENDIAN;
                }
                if(_float32)
                {
                    if(fm.endianness == 0) 
                        fm.datatype = COMPLEX_FLOAT32_BYTE_SWAPPED;
                    else
                        fm.datatype = COMPLEX_FLOAT32_BYTE_SWAPPED;
                }
            } else
            {
                fm.realOrComplex = 1;
                if(_int8) fm.datatype = REAL_INT8;                
                if(_int16)
                {
                    if(fm.endianness == 0) 
                        fm.datatype = REAL_INT16_LITTLE_ENDIAN;
                    else
                        fm.datatype = REAL_INT16_BIG_ENDIAN;
                }
                if(_float32)
                {
                    if(fm.endianness == 0) 
                        fm.datatype = REAL_FLOAT32_BYTE_SWAPPED;
                    else
                        fm.datatype = REAL_FLOAT32;
                }
            }
        }                

        fm.detectedFileType = "";
        
        return parsed;
    }

    /// @brief General function that searches for known filename formats. First successfull recognizion is used
    /// @param filename input filename
    /// @param fm target FileInfo struct to save metadata
    /// @return 
    inline bool getFileMetadata(std::string filename, FileInfo &fm)
    {
        FileInfo temp;
        
        // Check all known filename types for a match
        if(getKingFileMetadata(filename, temp))
        {
            fm = temp;
            return true;
        } else if(getWaveviewerMetadata(filename, temp))
        {
            std::cout << "Detected waveviewer name" << std::endl;
            fm = temp;
            return true;
        } else if(getAnyMetadata(filename, temp))
        {
            std::cout << "Detected parts of filename" << std::endl;
            fm = temp;
            return true;
        }
        else
        {
            std::cout << "Filename formatting not recognized for filename: " << filename << std::endl;
            return false; // no known filename types detected
        }
    }
}