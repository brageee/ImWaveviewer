#pragma once

#include <complex>
#include <memory>
#include <vector>
#include "AbstractSampleSource.h"
#include "Subscriber.h"

template<typename T>
class SampleSource : public AbstractSampleSource, public Subscriber
{
protected:
    int saveSize = 10000;

public:
    virtual ~SampleSource() { };    
    virtual std::unique_ptr<std::vector<T>> GetSamples(size_t start, size_t length) = 0;
    //virtual void invalidateEvent() { };
    std::type_index SampleType() override;    
    int GetSaveSize() { return saveSize; }
    void SetSaveSize(int size) { saveSize = size; }
};
