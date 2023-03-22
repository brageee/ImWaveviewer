
#pragma once

#include <complex>
#include <memory>
#include <set>
#include <typeindex>
#include "Subscriber.h"
#include "Utils.h"
#include "Structs.h"

/**
 * Abstract sample source handles all general aspects of a SampleSource and is used as the pointer type
 * in MainGui to keep track of derived signals.
 * 
 * Type specific things are handled in SampleSource and other inherited classes.
*/

class AbstractSampleSource
{

public:
    AbstractSampleSource();
    virtual ~AbstractSampleSource() {};
    virtual std::type_index SampleType() = 0;
    void subscribe(std::shared_ptr<AbstractSampleSource> subscriber);
    int subscriberCount();
    void unsubscribe(std::shared_ptr<AbstractSampleSource> subscriber);
    void SetName(std::string s) { name = s;}
    std::string GetName() { return name;}
    bool RealSignal() { return realSignal; }
    void SetRealSignal(bool r) { realSignal = r; }
    bool CanReconfigure() { return reconfigurable; }
    void SetReconfigure( bool r ) { reconfigurable = r; }
    DerivationType GetSourceType() { return sourceType; }
    void SetSourceType(DerivationType t) { sourceType = t; }
    virtual size_t Count() = 0;
    double Rate() { return sampleRate; };
    double RelativeBandwidth() {return relativeBandwidth; }
    double GetFrequency();
    void SetRate(double rate) { sampleRate = rate; } 
    unsigned int GetID()  { return id; }  
    std::set<std::shared_ptr<AbstractSampleSource>> GetSubscribers(); 

protected:
    std::string name = "";
    bool realSignal = false;
    double frequency = 0;
    double sampleRate = 0;
    //double numSamples = 0;
    double relativeBandwidth = 0;
    bool reconfigurable = false;
    DerivationType sourceType;

private:
    std::set<std::shared_ptr<AbstractSampleSource>> subscribers;
    unsigned int id;
    
};
