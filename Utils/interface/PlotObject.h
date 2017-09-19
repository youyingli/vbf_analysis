#ifndef __PLOTOBJECT__
#define __PLOTOBJECT__

#include "TCanvas.h"
#include "TPad.h"
#include "TH1.h"
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
    extern TH1* RatioPlot(TH1* hNumerator, TH1* hDenominator, const std::string& xTitle, const std::string& yTitle);
    extern TLine* NewHorizontalLine(TH1* plot, double y, Color_t color, Style_t style, Width_t width);
};

#endif
