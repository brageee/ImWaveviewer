
ImWaveviewer is a tool to analyze signals. Builds on Linux and Mac, mainly tested on Ubuntu.

Dependencies:
- Liquid-dsp 
- FFTW
- GLFW
- OpenGl3

Install deps from apt:  
sudo apt install cmake autoconf libglfw3-dev libfftw3-dev 

Install liquid-dsp:  
git clone https://github.com/jgaeddert/liquid-dsp  
cd liquid-dsp  
./bootstrap  
./configure 
make  
sudo make install  

Install ImWaveviewer:  
git clone --recursive-submodules https://github.com/brageee/ImWaveviewer  
cd ImWaveviewer  
mkdir build  
cd build  
cmake ..  
make  

Run waveviewer  
./waveviewer

