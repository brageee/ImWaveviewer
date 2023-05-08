
Waveviewer is a tool to analyze signals. Builds on Linux and Mac, mainly tested on Ubuntu.

<img src="gui/Images/screenshot1.png" alt="Power spectrum" width="600" height="300">

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

### Plot domains ###
Waveviewer currently provides four plot domains to visualize signals: time domain, power spectrum, spectrogram and scatter plot.

### Processing ###
Waveviewer can also perform some processing on signals. For instance filtering or frequency shifting. This is done by right-clicking the signal name at the upper-left corner and choosing an operation. Waveviewer will then perform the chosen operation and display its output in the plot domain(s). Note that Waveviewer only does the processing for the amount of data required to display the output.

<img src="gui/Images/screenshot2.png" alt="Power spectrum" width="400" height="264">