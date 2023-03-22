#include "InputSource.h"

/*###########################################
####     Complex sample adapters            ####
#############################################*/

/*
Complex double native byte ordering
*/
class CD64ByteSwappedSampleAdapter : public SampleAdapter<std::complex<float>> {
public:
    size_t SampleSize() override {
        return sizeof(std::complex<double>);
    }
    void CopyRange(std::ifstream &fileHandler, size_t start, size_t length, std::vector<std::complex<float>>* const dest) override {        
        dest->resize(length);
        fileHandler.seekg (start*SampleSize(), std::ios::beg);
        double sampleR;
        double sampleI;
        for (int i = 0; i < length; i++)
        {
            fileHandler.read(reinterpret_cast<char*>(&sampleR), 8);            
            fileHandler.read(reinterpret_cast<char*>(&sampleI), 8); 
            dest->at(i) = std::complex<float>(static_cast<float>(sampleR), static_cast<float>(sampleI));
        } 				
    }
};

/*
Complex float native byte ordering
*/
class CF32ByteSwappedSampleAdapter : public SampleAdapter<std::complex<float>> {
public:
    size_t SampleSize() override {
        return sizeof(std::complex<float>);
    }
    void CopyRange(std::ifstream &fileHandler, size_t start, size_t length, std::vector<std::complex<float>>* const dest) override {        
        dest->resize(length);
        fileHandler.seekg (start*SampleSize(), std::ios::beg);
        fileHandler.read ( reinterpret_cast<char*>(dest->data()), length*SampleSize());					
    }
};
/*
Complex float non-native byte ordering
*/
class CF32SampleAdapter : public SampleAdapter<std::complex<float>> {
public:
    size_t SampleSize() override {
        return sizeof(std::complex<float>);
    }
    void CopyRange(std::ifstream &fileHandler, size_t start, size_t length, std::vector<std::complex<float>>* const dest) override {        
        dest->resize(length);
        fileHandler.seekg (start*SampleSize(), std::ios::beg);
        uint8_t sampleR[4];
        uint8_t sampleI[4];
        for (int i = 0; i < length; i++)
        {
            fileHandler.read ( reinterpret_cast<char*>(&sampleR[0]), 4);
            fileHandler.read ( reinterpret_cast<char*>(&sampleI[0]), 4);
            uint32_t tempReal = 0;
            uint32_t tempImag = 0;
            tempReal = (uint32_t)((sampleR[0] << 24) | (sampleR[1] << 16) | (sampleR[2] <<  8) | sampleR[3]);
            tempImag = (uint32_t)((sampleI[0] << 24) | (sampleI[1] << 16) | (sampleI[2] <<  8) | sampleI[3]);
            //dest->at(i) = std::complex<float>(*((float *) &tempReal), *((float *) &tempImag));
            dest->at(i) = std::complex<float>((float) tempReal, (float ) tempImag);
        }        				
    }
};

/*
Complex int16 native byte ordering
*/
class CI16LittleEndianSampleAdapter : public SampleAdapter<std::complex<float>> {
public:
    size_t SampleSize() override {
        return sizeof(std::complex<int16_t>);
    }
    void CopyRange(std::ifstream &fileHandler, size_t start, size_t length, std::vector<std::complex<float>>* const dest) override {        
        dest->resize(length);
        fileHandler.seekg (start*SampleSize(), std::ios::beg);
        std::complex<int16_t> sample;
        for (int i = 0; i < length; i++)
        {
            fileHandler.read(reinterpret_cast<char*>(&sample), 4);            
            dest->at(i) = std::complex<float>(sample.real()*kS16F32, sample.imag()*kS16F32);
        }        				
    }
};

/*
Complex int16 non-native byte ordering
*/
class CI16BigEndianSampleAdapter : public SampleAdapter<std::complex<float>> {
public:
    size_t SampleSize() override {
        return sizeof(std::complex<int16_t>);
    }
    void CopyRange(std::ifstream &fileHandler, size_t start, size_t length, std::vector<std::complex<float>>* const dest) override {        
        dest->resize(length);
        fileHandler.seekg (start*SampleSize(), std::ios::beg);
        uint8_t sampleR[2];
        uint8_t sampleI[2];			
        int16_t lEReal;
        int16_t lEImag;
        for (int i = 0; i < length; i++)
        {
            fileHandler.read(reinterpret_cast<char*>(&sampleR[0]), 2);
            fileHandler.read(reinterpret_cast<char*>(&sampleI[0]), 2);
            lEReal = sampleR[0] << 8 | sampleR[1];
            lEImag = sampleI[0] << 8 | sampleI[1];
            dest->at(i) = std::complex<float>(lEReal*kS16F32,lEImag*kS16F32);
        }        				
    }
};

/*
Complex int8 
*/
class CI8SampleAdapter : public SampleAdapter<std::complex<float>> {
public:
    size_t SampleSize() override {
        return sizeof(int8_t);
    }
    void CopyRange(std::ifstream &fileHandler, size_t start, size_t length, std::vector<std::complex<float>>* const dest) override {        
        dest->resize(length);
        fileHandler.seekg (start*SampleSize(), std::ios::beg);
        std::complex<int8_t> sample;
        for (int i = 0; i < length; i++)
        {
            fileHandler.read(reinterpret_cast<char*>(&sample), 2);            
            dest->at(i) = std::complex<float>(sample.real()*kS8F32, sample.imag()*kS8F32);
        }        				
    }
};

/*
Complex uint8
*/
class CU8SampleAdapter : public SampleAdapter<std::complex<float>> {
public:
    size_t SampleSize() override {
        return sizeof(uint8_t);
    }
    void CopyRange(std::ifstream &fileHandler, size_t start, size_t length, std::vector<std::complex<float>>* const dest) override {        
        dest->resize(length);
        fileHandler.seekg (start*SampleSize(), std::ios::beg);
        int8_t sampleR;
        int8_t sampleI;			        
        for (int i = 0; i < length; i++)
        {
            fileHandler.read(reinterpret_cast<char*>(&sampleR), 1);
            fileHandler.read(reinterpret_cast<char*>(&sampleI), 1);            
            dest->at(i) = std::complex<float>((sampleR-127.4f)*kS8F32,(sampleI-127.4f)*kS8F32);
        }        				
    }
};


/*###########################################
####     Real sample adapters            ####
#############################################*/


class D64ByteSwappedSampleAdapter : public SampleAdapter<float> {
public:
    size_t SampleSize() override {
        return sizeof(double);
    }
    void CopyRange(std::ifstream &fileHandler, size_t start, size_t length, std::vector<float>* const dest) override {        
        dest->resize(length);
        fileHandler.seekg (start*SampleSize(), std::ios::beg);
        double sample;
        for (int i = 0; i < length; i++)
        {
            fileHandler.read(reinterpret_cast<char*>(&sample), 8);            
            dest->at(i) = static_cast<float>(sample);
        } 				
    }
};

class F32ByteSwappedSampleAdapter : public SampleAdapter<float> {
public:
    size_t SampleSize() override {
        return sizeof(float);
    }
    void CopyRange(std::ifstream &fileHandler, size_t start, size_t length, std::vector<float>* const dest) override {        
        dest->resize(length);
        fileHandler.seekg (start*SampleSize(), std::ios::beg);
        fileHandler.read ( reinterpret_cast<char*>(dest->data()), length*SampleSize());					
    }
};

/*
Real float non-native byte ordering
*/
class F32SampleAdapter : public SampleAdapter<float> {
public:
    size_t SampleSize() override {
        return sizeof(float);
    }
    void CopyRange(std::ifstream &fileHandler, size_t start, size_t length, std::vector<float>* const dest) override {        
        dest->resize(length);
        fileHandler.seekg (start*SampleSize(), std::ios::beg);
        uint8_t sample[4];        
        for (int i = 0; i < length; i++)
        {
            fileHandler.read ( reinterpret_cast<char*>(&sample[0]), 4);            
            uint32_t tempReal = 0;            
            tempReal = (uint32_t)((sample[0] << 24) | (sample[1] << 16) | (sample[2] <<  8) | sample[3]);                        
            dest->at(i) = tempReal;
        }        				
    }
};

/*
Real int16 native byte ordering
*/
class I16LittleEndianSampleAdapter : public SampleAdapter<float> {
public:
    size_t SampleSize() override {
        return sizeof(int16_t);
    }
    void CopyRange(std::ifstream &fileHandler, size_t start, size_t length, std::vector<float>* const dest) override {        
        dest->resize(length);
        fileHandler.seekg (start*SampleSize(), std::ios::beg);
        int16_t sample;
        for (int i = 0; i < length; i++)
        {
            fileHandler.read(reinterpret_cast<char*>(&sample), 2);            
            dest->at(i) = sample*kS16F32;
        }        				
    }
};

/*
Real int16 native byte ordering
*/
class I16BigEndianSampleAdapter : public SampleAdapter<float> {
public:
    size_t SampleSize() override {
        return sizeof(int16_t);
    }
    void CopyRange(std::ifstream &fileHandler, size_t start, size_t length, std::vector<float>* const dest) override {        
        dest->resize(length);
        fileHandler.seekg (start*SampleSize(), std::ios::beg);
        uint8_t sample[2];
        for (int i = 0; i < length; i++)
        {
            fileHandler.read(reinterpret_cast<char*>(&sample[0]), 2);            
            dest->at(i) = (sample[0] << 8 | sample[1])*kS16F32;
        }        				
    }
};

/*
Real int8
*/
class I8SampleAdapter : public SampleAdapter<float> {
public:
    size_t SampleSize() override {
        return sizeof(int8_t);
    }
    void CopyRange(std::ifstream &fileHandler, size_t start, size_t length, std::vector<float>* const dest) override {        
        dest->resize(length);
        fileHandler.seekg (start*SampleSize(), std::ios::beg);
        int8_t sample;
        for (int i = 0; i < length; i++)
        {
            fileHandler.read(reinterpret_cast<char*>(&sample), 1);            
            dest->at(i) = sample*kS8F32;
        }        				
    }
};

/*
Real uint8
*/
class U8SampleAdapter : public SampleAdapter<float> {
public:
    size_t SampleSize() override {
        return sizeof(uint8_t);
    }
    void CopyRange(std::ifstream &fileHandler, size_t start, size_t length, std::vector<float>* const dest) override {        
        dest->resize(length);
        fileHandler.seekg (start*SampleSize(), std::ios::beg);
        uint8_t sample;
        for (int i = 0; i < length; i++)
        {
            fileHandler.read(reinterpret_cast<char*>(&sample), 1);            
            dest->at(i) = (static_cast<float>(sample)-127.4f)*kS8F32;
        }        				
    }
};

template<typename T>
InputSource<T>::InputSource()
{
}

template<typename T>
InputSource<T>::~InputSource()
{
    Cleanup();
}

template<typename T>
void InputSource<T>::Cleanup()
{    
    if (inputFile != nullptr) {
        delete inputFile;
        inputFile = nullptr;
    }
}

template <typename T>
size_t InputSource<T>::OpenFile(FileInfo fileInfo)
{       
}

template <>
size_t InputSource<float>::OpenFile(FileInfo fileInfo)
{    
    std::streampos fileSize;    
    fileHandler.open(fileInfo.filename, std::ios::in|std::ios::binary|std::ios::ate);    
    fileSize = fileHandler.tellg();
    numSamples = fileSize / getDataTypeByteSize(fileInfo.datatype);    
    if(fileInfo.datatype == REAL_FLOAT32_BYTE_SWAPPED)
        sampleAdapter = std::make_unique<F32ByteSwappedSampleAdapter>();
    else if(fileInfo.datatype == REAL_FLOAT32)
        sampleAdapter = std::make_unique<F32SampleAdapter>();
    else if(fileInfo.datatype == REAL_INT16_LITTLE_ENDIAN)
        sampleAdapter = std::make_unique<I16LittleEndianSampleAdapter>();
    else if(fileInfo.datatype == REAL_INT16_BIG_ENDIAN)
        sampleAdapter = std::make_unique<I16BigEndianSampleAdapter>();
    else if(fileInfo.datatype == REAL_INT8)
        sampleAdapter = std::make_unique<I8SampleAdapter>();
    else if(fileInfo.datatype == REAL_UINT8)
        sampleAdapter = std::make_unique<U8SampleAdapter>();
    else if(fileInfo.datatype == REAL_DOUBLE)
        sampleAdapter = std::make_unique<D64ByteSwappedSampleAdapter>();
    
    
    return fileSize;
}

template <>
size_t InputSource<std::complex<float>>::OpenFile(FileInfo fileInfo)
{    
    std::streampos fileSize;    
    fileHandler.open(fileInfo.filename, std::ios::in|std::ios::binary|std::ios::ate);    
    fileSize = fileHandler.tellg();
    numSamples = fileSize / getDataTypeByteSize(fileInfo.datatype);    
    if(fileInfo.datatype == COMPLEX_FLOAT32_BYTE_SWAPPED)
        sampleAdapter = std::make_unique<CF32ByteSwappedSampleAdapter>();
    else if(fileInfo.datatype == COMPLEX_FLOAT32)
        sampleAdapter = std::make_unique<CF32SampleAdapter>();
    else if(fileInfo.datatype == COMPLEX_INT16_LITTLE_ENDIAN)
        sampleAdapter = std::make_unique<CI16LittleEndianSampleAdapter>();
    else if(fileInfo.datatype == COMPLEX_INT16_BIG_ENDIAN)
        sampleAdapter = std::make_unique<CI16BigEndianSampleAdapter>();
    else if(fileInfo.datatype == COMPLEX_INT8)
        sampleAdapter = std::make_unique<CI8SampleAdapter>();
    else if(fileInfo.datatype == COMPLEX_UINT8)
        sampleAdapter = std::make_unique<CU8SampleAdapter>();
    else if(fileInfo.datatype == COMPLEX_DOUBLE)
        sampleAdapter = std::make_unique<CD64ByteSwappedSampleAdapter>();

    return fileSize;
}

template <typename T>
std::unique_ptr<std::vector<T>> InputSource<T>::GetSamples(size_t start, size_t length)
{
    //if (inputFile == nullptr)
    //    return nullptr;
   
    if(start < 0 || length < 0)
        return nullptr;

    if (start + length > numSamples)
        return nullptr;

    auto dest = std::make_unique<std::vector<T>>(length);
    sampleAdapter->CopyRange(fileHandler, start, length, dest.get());

    return dest;
}

template class InputSource<std::complex<float>>;
template class InputSource<float>;
