#include <stdio.h>
#include "MainGui.h"

using namespace gui;

int main(int argc, char const *argv[]) {
    MainGui* gui = new MainGui("ImWaveviewer",1500,800);        
    gui->Render();
    
    return 0;
}