#pragma once

#include <complex>
#include <vector>
#include <iostream>
#include <fstream>
#include "SampleSource.h"
#include "Structs.h"
#include "DataTypes.h"

template<typename T>
class SampleAdapter {
public:
    virtual size_t SampleSize() = 0;
    virtual void CopyRange(std::ifstream &fileHandler, size_t start, size_t length, std::vector<T>* const dest) = 0;    
    virtual ~SampleAdapter() { };
};

template<typename T>
class InputSource : public SampleSource<T>
{
private:
    FILE *inputFile = nullptr;
    size_t numSamples = 0;    
    std::ifstream fileHandler;
    //uchar *mmapData = nullptr;
    std::unique_ptr<SampleAdapter<T>> sampleAdapter;
    std::string _fmt;    

    //void readMetaData(const QString &filename);

public:
    InputSource();
    ~InputSource();
    void Cleanup();
    size_t OpenFile(FileInfo fileInfo);
    std::unique_ptr<std::vector<T>> GetSamples(size_t start, size_t length);
    size_t Count() {
        return numSamples;
    };        
    void SetFormat(std::string fmt);   
};
