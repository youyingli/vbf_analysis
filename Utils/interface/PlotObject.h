#ifndef __PLOTOBJECT__
#define __PLOTOBJECT__

#include "TCanvas.h"
#include "TPad.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TLine.h"

namespace ARTKIT{

    extern TCanvas* signalCanvas();
    extern TPad* TopPad();
    extern TPad* BottomPad();
    extern TPad* NormalPad();
    extern TH1D* newH1LinePlot( TH1D* RAWplot, Color_t color, Style_t style );
    extern TH1D* newH1FillPlot( TH1D* RAWplot, Color_t color, Style_t style );
    extern TH1D* newH1PointPlot( TH1D* RAWplot );
    extern TLegend* newLegend();
    extern TH1D* ratioPlot(TH1D* hNumerator, TH1D* hDenominator, std::string xTitle, std::string yTitle);
    extern TLine* horizontalLine(TH1D* plot, double y, Color_t color, Style_t style, Width_t width);

};

#endif
