#include "vbf_analysis/DataMcComparison/interface/LatexDraw.h"
#include "vbf_analysis/Utils/interface/PlotMgrLib.h"

using namespace std;

void AddLatexContent(double lumi, const string& type) 
{
    plotmgr::SetCMSandLUMI(lumi);

    if (type == "") {
        TLatex lex;
        lex.SetNDC(true);
        lex.SetTextFont(42);
        lex.SetTextSize(0.03);
        lex.DrawLatex(0.17,0.875,"Pt_{#gamma1}/M_{#gamma#gamma}>1/4, Pt_{#gamma2}/M_{#gamma#gamma}>1/5");
        lex.DrawLatex(0.17,0.830,"Pt_{jet1}>30GeV, Pt_{jet2}>20GeV");
        lex.DrawLatex(0.17,0.785,"M_{jj}>100GeV");
    }

    if (type == "VBFTag") {
        TLatex lex;
        lex.SetNDC(true);
        lex.SetTextFont(62);
        lex.SetTextSize(0.04);
        lex.DrawLatex(0.17, 0.875, "VBFTag");
        lex.SetTextSize(0.03);
        lex.SetTextFont(42);
        lex.DrawLatex(0.17, 0.830, "100 <  M_{#gamma#gamma} < 180 GeV");
        lex.DrawLatex(0.17, 0.790, "Sideband region");
        lex.DrawLatex(0.17, 0.755, "JEC,JER");
        lex.DrawLatex(0.17, 0.720, "Systematics included");
    }

    if (type == "ZPlusJets") {
        TLatex lex;
        lex.SetNDC(true);
        lex.SetTextFont(62);
        lex.SetTextSize(0.04);
        lex.DrawLatex(0.17, 0.875, "ZPlusJets");
        lex.SetTextSize(0.03);
        lex.SetTextFont(42);
        lex.DrawLatex(0.17, 0.830, "70 <  M_{ee} < 110 GeV");
        lex.DrawLatex(0.17, 0.790, "JEC,JER");
        lex.DrawLatex(0.17, 0.755, "Systematics included");
    }
}
