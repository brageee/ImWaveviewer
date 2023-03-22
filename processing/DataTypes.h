#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <complex>
#include <vector>

/*
enum SampleType{
    uint8_t,
    int16_t,
    float,
    complex<float>   
};*/

const float kS16F32 = 1.0f / 32768.0f;
const float kS8F32 = 1.0f / 256.0f;

enum DataType {
    UNKNOWN,
    COMPLEX_DOUBLE,
    COMPLEX_FLOAT32,
    COMPLEX_FLOAT32_BYTE_SWAPPED,
    COMPLEX_INT16_BIG_ENDIAN,
    COMPLEX_INT16_LITTLE_ENDIAN,
    COMPLEX_INT8,
    COMPLEX_UINT8,
    REAL_DOUBLE,
    REAL_FLOAT32,
    REAL_FLOAT32_BYTE_SWAPPED,
    REAL_INT16_BIG_ENDIAN,
    REAL_INT16_LITTLE_ENDIAN,
    REAL_INT8,
    REAL_UINT8
};


struct SupportedDataTypes{
    int num = 1;
    std::vector<DataType> types = {COMPLEX_FLOAT32,
    COMPLEX_FLOAT32_BYTE_SWAPPED,
    COMPLEX_INT16_BIG_ENDIAN,
    COMPLEX_INT16_LITTLE_ENDIAN,
    COMPLEX_INT8,
    REAL_FLOAT32,
    REAL_FLOAT32_BYTE_SWAPPED,
    REAL_INT16_BIG_ENDIAN,
    REAL_INT16_LITTLE_ENDIAN,
    REAL_INT8,
    };
};

inline bool IsSupported(DataType dt)
{
    bool result = false;
    SupportedDataTypes s;
    for (size_t i = 0; i < s.types.size(); i++)
    {
        if(dt == s.types[i])
            result = true;
    }
    return result;
    
}

inline bool IsComplex(DataType dt)
{
    if(dt == COMPLEX_FLOAT32 || dt == COMPLEX_FLOAT32_BYTE_SWAPPED || dt == COMPLEX_INT16_BIG_ENDIAN || dt == COMPLEX_INT16_LITTLE_ENDIAN || dt == COMPLEX_INT8 || dt == COMPLEX_UINT8)
        return true;
    else    
        return false;
}


inline bool IsNative(DataType dt)
{
    if(dt == COMPLEX_FLOAT32_BYTE_SWAPPED || dt == COMPLEX_INT16_LITTLE_ENDIAN || dt == COMPLEX_INT8 || dt == COMPLEX_UINT8 || dt == REAL_FLOAT32_BYTE_SWAPPED || dt == REAL_INT16_LITTLE_ENDIAN)
        return true;
    else    
        return false;
}

inline DataType FileConfigureToDataType(int rc, int st, int e)
{
    if(rc == 0) //complex
    {
        if(st == 0)            
            return COMPLEX_INT8;
        else if(st == 1)
        {
            if(e == 0)
                return COMPLEX_INT16_LITTLE_ENDIAN;
            else
                return COMPLEX_INT16_BIG_ENDIAN;
        }
        else if(st == 2)
        {
            if(e == 0)
                return COMPLEX_FLOAT32_BYTE_SWAPPED;
            else
                return COMPLEX_FLOAT32;
        }
    }
    else //real
    {
        if(st == 0)
            return REAL_INT8;        
        else if(st == 1)
        {
            if(e == 0)
                return REAL_INT16_LITTLE_ENDIAN;
            else
                return REAL_INT16_BIG_ENDIAN;
        }
        else if(st == 2)
        {
            if(e == 0)
                return REAL_FLOAT32_BYTE_SWAPPED;
            else
                return REAL_FLOAT32;
        }
    }
    return UNKNOWN;
}

inline std::string DataTypeToString(DataType dataType)
{
    std::string ret;
    switch (dataType)
    {
        case COMPLEX_DOUBLE:                { ret = "CD64";               break; }
        case COMPLEX_FLOAT32:               { ret = "CF32";               break; }
        case COMPLEX_FLOAT32_BYTE_SWAPPED:  { ret = "CF32ByteSwapped";    break; }
        case COMPLEX_INT16_BIG_ENDIAN:      { ret = "CI16BigEndian";        break; }
        case COMPLEX_INT16_LITTLE_ENDIAN:   { ret = "CI16LittleEndian";     break; }
        case COMPLEX_INT8:                  { ret = "CI8";                  break; }
        case COMPLEX_UINT8:                 { ret = "CU8";                 break; }
        case REAL_DOUBLE:                   { ret = "D64";                  break; }
        case REAL_FLOAT32:                  { ret = "F32";                  break; }
        case REAL_FLOAT32_BYTE_SWAPPED:     { ret = "F32ByteSwapped";       break; }
        case REAL_INT16_BIG_ENDIAN:         { ret = "I16BigEndian";           break; }
        case REAL_INT16_LITTLE_ENDIAN:      { ret = "I16LittleEndian";        break; }
        case REAL_INT8:                     { ret = "INT8";                     break; }
        case REAL_UINT8:                    { ret = "UINT8";                    break; }
        case UNKNOWN:                        { ret = "UNKNOWN";                  break; }
    }
    return ret;
}

inline DataType StringToDataType(std::string dataTypeString)
{
    // Remove '"" from start or end if present
    if(dataTypeString.front() == '\"') dataTypeString.erase(0, 1);
    if(dataTypeString.back() == '\"') dataTypeString.pop_back();

    DataType ret = UNKNOWN;
    if      (dataTypeString == "complexfloat32")            { ret = COMPLEX_FLOAT32;                }
    else if (dataTypeString == "CD64")                      { ret = COMPLEX_DOUBLE;   }
    else if (dataTypeString == "complexfloat32byteswapped") { ret = COMPLEX_FLOAT32_BYTE_SWAPPED;   }
    else if (dataTypeString == "complexint16bigendian")     { ret = COMPLEX_INT16_BIG_ENDIAN;       }
    else if (dataTypeString == "complexint16littleendian")  { ret = COMPLEX_INT16_LITTLE_ENDIAN;    }
    else if (dataTypeString == "complexint8")               { ret = COMPLEX_INT8;                   }
    else if (dataTypeString == "complexuint8")              { ret = COMPLEX_UINT8;                  }
    else if (dataTypeString == "D64")                       { ret = REAL_DOUBLE;                    }
    else if (dataTypeString == "realfloat32")               { ret = REAL_FLOAT32;                   }
    else if (dataTypeString == "realfloat32byteswapped")    { ret = REAL_FLOAT32_BYTE_SWAPPED;      }
    else if (dataTypeString == "realint16bigendian")        { ret = REAL_INT16_BIG_ENDIAN;          }
    else if (dataTypeString == "realint16littleendian")     { ret = REAL_INT16_LITTLE_ENDIAN;       }
    else if (dataTypeString == "realint8")                  { ret = REAL_INT8;                      }
    else if (dataTypeString == "realuint8")                 { ret = REAL_UINT8;                     }
    return ret;
}

static int getDataTypeByteSize(DataType dataType)
{
    if(dataType == DataType::COMPLEX_FLOAT32 || dataType == DataType::COMPLEX_FLOAT32_BYTE_SWAPPED)
    {
        return sizeof(std::complex<float>);
    }
    else if(dataType == DataType::COMPLEX_INT16_BIG_ENDIAN || dataType == DataType::COMPLEX_INT16_LITTLE_ENDIAN)
    {
        return sizeof(std::complex<int16_t>);
    }
    else if(dataType == DataType::COMPLEX_INT8 || dataType == DataType::COMPLEX_UINT8)
    {
        return sizeof(std::complex<int8_t>);
    }
    else if(dataType == DataType::REAL_FLOAT32 || dataType == DataType::REAL_FLOAT32_BYTE_SWAPPED)
    {
        return sizeof(float);
    }
    else if(dataType == DataType::REAL_INT16_BIG_ENDIAN || dataType == DataType::REAL_INT16_LITTLE_ENDIAN)
    {
        return sizeof(int16_t);
    }
    else if(dataType == DataType::REAL_INT8 || dataType == DataType::REAL_UINT8)
    {
        return sizeof(int8_t);
    }
    else return 4;
}

