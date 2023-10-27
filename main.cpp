#include <stdio.h>
#include <fstream>
#include "MainGui.h"

using namespace gui;

int main(int argc, char const *argv[]) {
    std::string argInputFile = "";

    for (auto i = 1; i < argc; ++i) {
        std::ifstream input_file(argv[i], std::ios::in);
        if (!input_file.is_open()) {
            std::cerr << "Could not open input file '" << argv[i] << "'!\n";
        }
        else {
            argInputFile = argv[i];
        }
    }
    
    MainGui* gui = new MainGui("ImWaveviewer",1500,800, argInputFile);        
    gui->Render();
    
    return 0;
}