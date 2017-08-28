#ifndef __PLOTOBJECT__
#define __PLOTOBJECT__

#include "TCanvas.h"
#include "TPad.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TLine.h"

#include <string>

namespace plotmgr
{
    extern TCanvas* NewCanvas();
    extern TPad* NewTopPad();
    extern TPad* NewBottomPad();
    extern TPad* NewNormalPad();
    extern TLegend* NewLegend();
    extern TH1D* RatioPlot(TH1D* hNumerator, TH1D* hDenominator, const std::string& xTitle, const std::string& yTitle);
    extern TLine* NewHorizontalLine(TH1D* plot, double y, Color_t color, Style_t style, Width_t width);
};

#endif
