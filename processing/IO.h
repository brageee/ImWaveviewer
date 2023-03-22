#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <random>
#include <limits>
#include <chrono>
#pragma once

#include "DataTypes.h"

using namespace std;

namespace processing::io
{  	
	/*template <typename T1, typename T2>
	inline int ReadSamples(std::string infile, T1 &samples, int numSamples=-1)
	{
		std::streampos filesize;  
		std::ifstream file (infile, std::ios::in|std::ios::binary|std::ios::ate);
		size_t sizeOfSample
		int res = 0;
		if (file.is_open())
		{
			filesize = file.tellg();
			int numBytes = (numSamples<0) ? static_cast<int>(filesize) : numSamples*sizeof(T2);
			//std::cout << "Filesize " << filesize << std::endl;
			//inBytes = (uint8_t*) malloc(filesize);
			samples.resize(filesize/sizeof(T2));
			file.seekg (0, std::ios::beg);
			file.read ( reinterpret_cast<char*>(samples.data()), numBytes);
			file.close();    
			res = numBytes/sizeof(T2);
		}  
		return res;
	}*/

	inline int OpenFile(std::string infile, std::ifstream &fileHandler)
	{
		std::streampos fileSize;
		//std::ifstream file (infile, std::ios::in|std::ios::binary|std::ios::ate);
		//fileHandler (infile, std::ios::in|std::ios::binary|std::ios::ate);
		fileHandler.open(infile, std::ios::in|std::ios::binary|std::ios::ate);
		//fileHandler = file;
		fileSize = fileHandler.tellg();
		return static_cast<int>(fileSize);
	}

	/**
	 * 
	 */ 	
	inline int ReadSamples(std::ifstream fileHandler, DataType dt, int offset, int numSamples, vector<complex<float>> &samples)
	{
		std::streampos filesize;
		int byteSize = getDataTypeByteSize(dt);
		int res = 0;
		if(IsNative(dt))
		{
			vector<complex<int8_t>> v8;
			vector<complex<int16_t>> v16;			
			if (fileHandler.is_open())
			{
				filesize = fileHandler.tellg();
				int numBytes = (filesize < (numSamples+offset)*byteSize) ? static_cast<int>(filesize)-offset : numSamples*byteSize;
				
				//samples.resize(numBytes/sizeof(complex<float>));
				fileHandler.seekg (offset*byteSize, std::ios::beg);
				if(dt==COMPLEX_INT8)
				{
					v8.resize(numBytes/byteSize);
					fileHandler.read ( reinterpret_cast<char*>(v8.data()), numBytes);
					samples.resize(v8.size());
					for (int i = 0; i < v8.size(); i++)
					{
						samples[i] = complex<float>(v8[i].real()*kS8F32, v8[i].imag()*kS8F32);
					}
				} else if(dt==COMPLEX_INT16_LITTLE_ENDIAN)
				{
					v16.resize(numBytes/byteSize);
					fileHandler.read ( reinterpret_cast<char*>(v16.data()), numBytes);
					samples.resize(v16.size());
					for (int i = 0; i < v16.size(); i++)
					{
						samples[i] = complex<float>(v16[i].real()*kS16F32, v16[i].imag()*kS16F32);
					}
				}
				else if(dt==COMPLEX_FLOAT32_BYTE_SWAPPED)
				{
					samples.resize(numBytes/byteSize);
					fileHandler.read ( reinterpret_cast<char*>(samples.data()), numBytes);					
				}
					
				fileHandler.close();    
				res = samples.size();
			} 
		} else
		{						
			if (fileHandler.is_open())
			{
				filesize = fileHandler.tellg();
				int numBytes = (filesize < (numSamples+offset)*byteSize) ? static_cast<int>(filesize)-offset : numSamples*byteSize;
				
				//samples.resize(numBytes/sizeof(complex<float>));
				fileHandler.seekg (offset*byteSize, std::ios::beg);
				if(dt==COMPLEX_INT16_BIG_ENDIAN)
				{
					uint8_t intReal[2];
					uint8_t intImag[2];			
					int16_t lEReal;
					int16_t lEImag;
					samples.resize(numBytes/byteSize);
					for (int i = 0; i < samples.size(); i++)
					{
						fileHandler.read(reinterpret_cast<char*>(&intReal[0]), 2);
						fileHandler.read(reinterpret_cast<char*>(&intImag[0]), 2);
						lEReal = intReal[0] << 8 | intReal[1];
						lEImag = intImag[0] << 8 | intImag[1];
						samples[i] = complex<float>(lEReal*kS16F32,lEImag*kS16F32);
					}						
				}
				else if(dt==COMPLEX_FLOAT32)
				{
					samples.resize(numBytes/byteSize);
					uint8_t f32Real[4];
					uint8_t f32Imag[4];
					for (int i = 0; i < samples.size(); i++)
					{
						fileHandler.read ( reinterpret_cast<char*>(&f32Real[0]), 4);
						fileHandler.read ( reinterpret_cast<char*>(&f32Imag[0]), 4);
						uint32_t tempReal = 0;
						uint32_t tempImag = 0;
						tempReal = (uint32_t)((f32Real[0] << 24) | (f32Real[1] << 16) | (f32Real[2] <<  8) | f32Real[3]);
						tempImag = (uint32_t)((f32Imag[0] << 24) | (f32Imag[1] << 16) | (f32Imag[2] <<  8) | f32Imag[3]);
						samples[i] = std::complex<float>(*((float *) &tempReal), *((float *) &tempImag));

					}															
				}					
				fileHandler.close();    
				res = samples.size();
			} 
		}
		
		 
		return res;
	}
	
	inline int ReadSamples(std::string infile, DataType dt, vector<complex<float>> &samples, int numSamples=-1)
	{
		std::streampos filesize;  
		std::ifstream file (infile, std::ios::in|std::ios::binary|std::ios::ate);		
		int res = 0;
		int byteSize = getDataTypeByteSize(dt);
		if(IsNative(dt))
		{
			vector<complex<int8_t>> v8;
			vector<complex<int16_t>> v16;			
			if (file.is_open())
			{
				filesize = file.tellg();
				int numBytes = (numSamples<0) ? static_cast<int>(filesize) : numSamples*sizeof(byteSize);
				
				//samples.resize(numBytes/sizeof(complex<float>));
				file.seekg (0, std::ios::beg);
				if(dt==COMPLEX_INT8)
				{
					v8.resize(numBytes/byteSize);
					file.read ( reinterpret_cast<char*>(v8.data()), numBytes);
					samples.resize(v8.size());
					for (int i = 0; i < v8.size(); i++)
					{
						samples[i] = complex<float>(v8[i].real()*kS8F32, v8[i].imag()*kS8F32);
					}
				} else if(dt==COMPLEX_INT16_LITTLE_ENDIAN)
				{
					v16.resize(numBytes/byteSize);
					file.read ( reinterpret_cast<char*>(v16.data()), numBytes);
					samples.resize(v16.size());
					for (int i = 0; i < v16.size(); i++)
					{
						samples[i] = complex<float>(v16[i].real()*kS16F32, v16[i].imag()*kS16F32);
					}
				}
				else if(dt==COMPLEX_FLOAT32_BYTE_SWAPPED)
				{
					samples.resize(numBytes/byteSize);
					file.read ( reinterpret_cast<char*>(samples.data()), numBytes);					
				}
					
				file.close();    
				res = samples.size();
			} 
		} else
		{						
			if (file.is_open())
			{
				filesize = file.tellg();
				int numBytes = (numSamples<0) ? static_cast<int>(filesize) : numSamples*sizeof(byteSize);
				
				//samples.resize(numBytes/sizeof(complex<float>));
				file.seekg (0, std::ios::beg);
				if(dt==COMPLEX_INT16_BIG_ENDIAN)
				{
					uint8_t intReal[2];
					uint8_t intImag[2];			
					int16_t lEReal;
					int16_t lEImag;
					samples.resize(numBytes/byteSize);
					for (int i = 0; i < samples.size(); i++)
					{
						file.read(reinterpret_cast<char*>(&intReal[0]), 2);
						file.read(reinterpret_cast<char*>(&intImag[0]), 2);
						lEReal = intReal[1] << 8 | intReal[0];
						lEImag = intImag[1] << 8 | intImag[0];
						samples[i] = complex<float>(lEReal*kS16F32,lEImag*kS16F32);
					}						
				}
				else if(dt==COMPLEX_FLOAT32)
				{
					samples.resize(numBytes/byteSize);
					uint8_t f32Real[4];
					uint8_t f32Imag[4];
					for (int i = 0; i < samples.size(); i++)
					{
						file.read ( reinterpret_cast<char*>(&f32Real[0]), 4);
						file.read ( reinterpret_cast<char*>(&f32Imag[0]), 4);
						uint32_t tempReal = 0;
						uint32_t tempImag = 0;
						tempReal = (uint32_t)((f32Real[0] << 24) | (f32Real[1] << 16) | (f32Real[2] <<  8) | f32Real[3]);
						tempImag = (uint32_t)((f32Imag[0] << 24) | (f32Imag[1] << 16) | (f32Imag[2] <<  8) | f32Imag[3]);
						samples[i] = std::complex<float>(*((float *) &tempReal), *((float *) &tempImag));

					}															
				}					
				file.close();    
				res = samples.size();
			} 
		}
		
		 
		return res;
	}

	inline int ReadSamples(std::string infile, DataType dt, vector<float> &samples, int numSamples=-1)
	{
		std::streampos filesize;  
		std::ifstream file (infile, std::ios::in|std::ios::binary|std::ios::ate);		
		int res = 0;
		int byteSize = getDataTypeByteSize(dt);
		if(IsNative(dt))
		{
			vector<int8_t> v8;
			vector<int16_t> v16;			
			if (file.is_open())
			{
				filesize = file.tellg();
				int numBytes = (numSamples<0) ? static_cast<int>(filesize) : numSamples*sizeof(byteSize);
				
				//samples.resize(numBytes/sizeof(complex<float>));
				file.seekg (0, std::ios::beg);
				if(dt==REAL_INT8)
				{
					v8.resize(numBytes/byteSize);
					file.read ( reinterpret_cast<char*>(v8.data()), numBytes);
					samples.resize(v8.size());
					for (int i = 0; i < v8.size(); i++)
					{
						samples[i] = v8[i]*kS8F32;
					}
				} else if(dt==REAL_INT16_LITTLE_ENDIAN)
				{
					v16.resize(numBytes/byteSize);
					file.read ( reinterpret_cast<char*>(v16.data()), numBytes);
					samples.resize(v16.size());
					for (int i = 0; i < v16.size(); i++)
					{
						samples[i] = v16[i]*kS16F32;
					}
				}
				else if(dt==REAL_FLOAT32_BYTE_SWAPPED)
				{
					samples.resize(numBytes/byteSize);
					file.read ( reinterpret_cast<char*>(samples.data()), numBytes);					
				}
					
				file.close();    
				res = samples.size();
			} 
		} else
		{
			if (file.is_open())
			{
				filesize = file.tellg();
				int numBytes = (numSamples<0) ? static_cast<int>(filesize) : numSamples*sizeof(byteSize);
				
				//samples.resize(numBytes/sizeof(complex<float>));
				file.seekg (0, std::ios::beg);
				if(dt==REAL_INT16_BIG_ENDIAN)
				{
					uint8_t intReal[2];					
					int16_t lEReal;					
					samples.resize(numBytes/byteSize);
					for (int i = 0; i < samples.size(); i++)
					{
						file.read(reinterpret_cast<char*>(&intReal[0]), 2);
						lEReal = intReal[1] << 8 | intReal[0];						
						samples[i] = static_cast<float>(lEReal)*kS16F32;
					}						
				}
				else if(dt==REAL_FLOAT32)
				{
					samples.resize(numBytes/byteSize);
					uint8_t f32Real[4];					
					for (int i = 0; i < samples.size(); i++)
					{
						file.read ( reinterpret_cast<char*>(&f32Real[0]), 4);						
						uint32_t tempReal = 0;						
						tempReal = (uint32_t)((f32Real[0] << 24) | (f32Real[1] << 16) | (f32Real[2] <<  8) | f32Real[3]);						
						samples[i] = *((float *) &tempReal);

					}															
				}					
				file.close();    
				res = samples.size();
			} 
		}				 
		return res;
	}
	
	template <typename T1, typename T2>
	inline void WriteSamples(std::string outfile, T1 &samples, int numSamples=-1)
	{
		int samplesToWrite;
		if(numSamples < 0)
			samplesToWrite = samples.size();
		else
			samplesToWrite = numSamples;
		std::ofstream ob (outfile, std::ios::out|std::ios::binary|std::ios::ate);		
		ob.write(reinterpret_cast<const char*>(samples.data()), samplesToWrite*sizeof(T2));
	}

	
} // namespace processing::io


