#include <vector>
#include <complex>

#include "imgui.h"
#include "imgui_internal.h"
//#include "implot.h"
#include "Structs.h"

#include "IO.h"

using namespace std;

typedef vector<std::complex<float>> vcf;
typedef vector<float> vf;

namespace gui
{
    void ShowError(bool &showError, string &errorMsg);
    
    void ShowFrequencyShiftMenu(double* shift, bool &show, bool &configured);

    void ShowResampleMenu(double* outputRate, bool &show, bool &configured);

    void ShowFilterMenu(FilterVars &taps, bool &show, bool &configured);

    void ShowDDCMenu(double* centerFreq, double* outputRate,  double* cutoff, double* attenuation, bool &show, bool &configured);

    void ShowOFDMDemodMenu(OFDMDemodVars &vars, bool &ofdmEnableCustomSampleReference, bool &showOFDMDemodWindow, bool &ofdmDemodConfigured);
    
    void ShowReconfigureWarning(bool &parse);
}
