#pragma once

#include "SampleSource.h"

template<typename Tin, typename Tout>
class SampleProc : public SampleSource<Tout>
{
protected:    
    std::shared_ptr<SampleSource<Tin>> src;  
    size_t history = 0;  

public:
    SampleProc(std::shared_ptr<SampleSource<Tin>> src);
    virtual ~SampleProc() {};
    //void invalidateEvent();
    virtual std::unique_ptr<std::vector<Tout>> GetSamples(size_t start, size_t length);
    virtual void Work(void *input, void *output, int count, size_t sampleid) = 0;    
    virtual size_t Count() {
        return src->Count();
    };
    double Rate() {
        return src->Rate();
    };

    float RelativeBandwidth() {
        return src->RelativeBandwidth();
    }; 

    size_t GetHistory() { return history; }
    void SetHistory(size_t h) { history = h; }
};
