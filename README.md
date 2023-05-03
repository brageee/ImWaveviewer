
ImWaveviewer is a tool to analyze signals. 

Dependencies:
-Liquid-dsp 
-FFTW
-GLFW
-OpenGl3

Install deps from apt:
sudo apt install cmake autoconf libglfw3-dev libfftw3-dev 

Install liquid-dsp
git clone https://github.com/jgaeddert/liquid-dsp
cd liquid-dsp
./bootstrap
./configure
make
sudo make install

Clone ImWaveviewer with --recursive-submodules and build with cmake.
