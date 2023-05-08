
Waveviewer is a tool to analyze signals. Builds on Linux and Mac, mainly tested on Ubuntu.

## Installation and dependencies ##

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

Install Waveviewer:  
git clone --recursive-submodules https://github.com/brageee/Waveviewer  
cd Waveviewer  
mkdir build  
cd build  
cmake ..  
make  

Run waveviewer  
./waveviewer

## Config ##
A config file is created under build/waveviewer.ini . This is a work in progress. At the moment you can set whether or not the default path when running the program should update as you select a new input file (update to the directory of the input file).

## Usage ##
To be documented.
