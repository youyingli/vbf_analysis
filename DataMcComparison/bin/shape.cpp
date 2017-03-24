#include <iostream>
#include "TFile.h"
#include "vbf_analysis/Utils/interface/PlotLibrary.h"

using namespace std;

int main(){
        
        const int signum = 5;
        TString path ="/wk_cms/youying/CMSSW_8_0_26_patch1/src/VBFTag/";
        TString sfilename[signum];
        sfilename[0] = "Re_VBFHToGG_M125_13TeV_amcatnlo_pythia8.root"          ;
        sfilename[1] = "Re_GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8.root"   ;
        sfilename[2] = "Re_WHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root";
        sfilename[3] = "Re_ZHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root";
        sfilename[4] = "Re_ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2.root";
        TFile* sfile[signum];
        TH1D* sig[signum];
        for(int i=0;i<signum;i++) {
            sfile[i] = TFile::Open(path+sfilename[i]);
            sig[i] = (TH1D*) sfile[i]->Get("h1_Mgg");
        }
        TH1D* vbf = ARTKIT::newH1LinePlot(sig[0],kRed,1);
        TH1D* ggh = ARTKIT::newH1FillPlot(sig[1],kOrange+1,1001);
        TH1D* vhtmp = (TH1D*)sig[2]->Clone();
        vhtmp->Add(sig[3]);
        TH1D* vh = ARTKIT::newH1FillPlot(vhtmp,kGray+1,1001);
        TH1D* tth = ARTKIT::newH1FillPlot(sig[4],kViolet-1,1001);
        THStack* sdijet_BDT = new THStack("sig","");
        sdijet_BDT ->Add(vh);
        sdijet_BDT ->Add(tth);
        sdijet_BDT ->Add(ggh);

        double total = vbf->Integral() + ggh->Integral() +vh->Integral() +tth->Integral();
        cout << "VBF" <<" : " <<vbf->Integral() <<"("<< vbf->Integral()/total<<")" << endl;
        cout << "GGF" <<" : " <<ggh->Integral() <<"("<< ggh->Integral()/total<<")" << endl;
        cout << "VH"  <<" : " <<vh ->Integral() <<"("<< vh ->Integral()/total<<")" << endl;
        cout << "TTH" <<" : " <<tth->Integral() <<"("<< tth->Integral()/total<<")" << endl;
        cout << "Total : " << total <<endl;

        TCanvas* c1 = new TCanvas("c1","c1",650,600);
        c1->cd();
        TPad*pad1 = ARTKIT::NormalPad();
        pad1->Draw();
        //pad1->SetLogy();
        pad1->cd();
        sdijet_BDT->Draw("histo");
        vbf->Draw("histosame");

        sdijet_BDT->SetMinimum(0.01);
        //sdijet_BDT->SetMaximum(sdijet_BDT->GetMaximum()*500);
        sdijet_BDT->SetMaximum(sdijet_BDT->GetMaximum()*1.4);

        //sdijet_BDT -> GetXaxis()->SetTitle("dijet BDT");
        sdijet_BDT -> GetXaxis()->SetTitle("M_{#gamma#gamma}");
        sdijet_BDT -> GetYaxis()->SetTitle("Events/(25)");
        sdijet_BDT -> GetHistogram() -> SetTitleFont(42,"xyz");
        sdijet_BDT -> GetHistogram() -> SetLabelFont(42,"xyz");
        sdijet_BDT -> GetHistogram() -> SetLabelSize(0.04,"xyz");
        sdijet_BDT -> GetHistogram() -> SetTitleSize(0.04,"xyz");

        TLegend*leg = ARTKIT::newLegend();
        leg->AddEntry(vbf,"VBF(mH = 125 GeV)","L");
        leg->AddEntry(ggh,"ggH(mH = 125 GeV)","F");
        leg->AddEntry(tth,"ttH(mH = 125 GeV)","F");
        leg->AddEntry(vh,"VH(mH = 125 GeV)","F");
        leg->Draw();
        double lumi =35.85;
            TLatex latex;
            latex.SetNDC(true);
            latex.SetTextFont(62);
            latex.SetTextSize(0.045);
            latex.DrawLatex(0.135,0.96,"CMS");
            latex.SetTextFont(42);
            latex.SetTextSize(0.045);
            latex.DrawLatex(0.23,0.96,"Preliminary");
            latex.SetTextSize(0.04);
            latex.DrawLatex(0.61,0.96,Form("#sqrt{s}=13 TeV, L=%.2f fb^{-1}",lumi));
            //latex.SetTextSize(0.03);
            //latex.DrawLatex(0.17,0.875,"Pt_{#gamma1}/M_{#gamma#gamma}>1/4, Pt_{#gamma2}/M_{#gamma#gamma}>1/5");
            //latex.DrawLatex(0.17,0.830,"Pt_{jet1}>30GeV, Pt_{jet2}>20GeV");
            //latex.DrawLatex(0.17,0.785,"M_{jj}>100GeV");
            //
            latex.SetTextSize(0.05);
            latex.SetTextFont(62);
            latex.DrawLatex(0.17,0.875,"VBFTag");


        c1->Update();
        c1->Print("/wk_cms/youying/CMSSW_8_0_26_patch1/src/mgg.pdf");




return 0;
}
