#ifndef __PLOTOBJECT__
#define __PLOTOBJECT__

#include "TCanvas.h"
#include "TPad.h"
#include "TH1F.h"
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
    extern TH1F* RatioPlot(TH1F* hNumerator, TH1F* hDenominator, const std::string& xTitle, const std::string& yTitle);
    extern TLine* NewHorizontalLine(TH1F* plot, double y, Color_t color, Style_t style, Width_t width);
};

#endif
