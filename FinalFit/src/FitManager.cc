#include "vbf_analysis/FinalFit/interface/FitManager.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooPlot.h"
#include "RooRandom.h"

#include "TCanvas.h"

#include "boost/algorithm/string/classification.hpp"
#include "boost/algorithm/string/split.hpp"

using namespace std;
using namespace RooFit;

FitManager::FitManager (const RooArgSet& varset, RooAbsPdf* pdf):
    _pdf(pdf)
{
    _varset = new RooArgSet(varset);
    _params = _pdf->getParameters(_varset);
    _params->snapshot(_orisnapshot);
    string varstr = _params->contentsString();
    boost::split(_varstrs, varstr, boost::is_any_of(","));
    _isbinnedfit = false;
    _is1DFit = false;
    if (_varset->getSize() == 1) {
        RooRealVar* var = (RooRealVar*) _varset->first();
        _initobsrange = make_pair(var->getMin(), var->getMax());
        _is1DFit = true;
    }
}

FitManager::~FitManager ()
{
    if (_is1DFit) {
        RooRealVar* var = (RooRealVar*) _varset->first();
        var->setRange(_initobsrange.first, _initobsrange.second);
    }
    delete _varset;
}

void FitManager::SetDataSetFitted (RooDataSet* dataset)
{
    _dset = dataset;
}

void FitManager::UseUnBinnedFitting (double minvar, double maxvar, bool isExtended)
{
    RooRealVar* var = (RooRealVar*) _varset->first();
    _data = UnWgtDataSetBuilder(var, _dset, "unbinned", minvar, maxvar);

    int stat = -1;
    int count = 1;
    while (stat != 0) {
        if (count>5) {
            cout << "Don't find the stable fitting for " << _pdf->GetName() << " PDF!" << endl;
            cout << "Please change the values or the range of parameters in " << _pdf->GetName() << " PDF !!" << endl;
            exit(0);
        }
        const RooCmdArg& arg = isExtended ? Extended(true) : RooCmdArg::none();
        _fitres = _pdf->fitTo(*_data, Minimizer("Minuit2", "minimize"), Save(true), Strategy(1), arg);
        stat = _fitres->status();
        if (stat != 0) _params->assignValueOnly(_fitres->randomizePars());
        count++;
    }
}

void FitManager::Use1DBinnedFitting (int nbin, double minvar, double maxvar)
{
    if (_varset->getSize() != 1) {
        cout << "Binned fit only supports 1 dimensional fitting!" << endl;
        exit(0);
    }

    RooRealVar* var = (RooRealVar*) _varset->first();
    _data = UnWgtDataSetBuilder(var, _dset, "binned", minvar, maxvar, nbin);

    int stat = -1;
    int count = 1;
    while (stat != 0) {
        if (count>5) {
            cout << "Don't find the stable fitting for " << _pdf->GetName() << " PDF!" << endl;
            cout << "Please change the values or the range of parameters in " << _pdf->GetName() << " PDF !!" << endl;
            exit(0);
        }
        _fitres = _pdf->fitTo(*_data, Minimizer("Minuit2","minimize"), Save(true), Strategy(1));
        stat = _fitres->status();
        if (stat != 0) _params->assignValueOnly(_fitres->randomizePars());
        count++;
    }
    _isbinnedfit = true;
}

void FitManager::SetConstParam (const string& paramname, bool flag)
{
    HasParameter(paramname);
    RooRealVar* var = (RooRealVar*) _params->find(paramname.c_str());
    var->setConstant(flag);
}

void FitManager::SetParamVal (const string& paramname, double val)
{
    HasParameter(paramname);
    RooRealVar* var = (RooRealVar*) _params->find(paramname.c_str());
    var->setVal(val);
}

double FitManager::GetParamVal (const string& paramname)
{
    HasParameter(paramname);
    RooRealVar* var = (RooRealVar*) _params->find(paramname.c_str());
    return var->getVal();
}

void FitManager::FixPdfShape (bool flag)
{
    for (const auto& it : _varstrs) {
        RooRealVar* var = (RooRealVar*) _params->find(it.c_str());
        var->setConstant(flag);
    }
}

void FitManager::SetOriginalPdf ()
{
    _params->assignValueOnly(*_orisnapshot);
    FixPdfShape (false);
}

double FitManager::GetChi2OverDof () const
{
    if (!_isbinnedfit) {
        cout << "Please use bin likelihood fit to get this information" << endl;
        exit(0);
    }
    RooRealVar* var = (RooRealVar*) _varset->first();
    RooPlot* chi2plot = var->frame();
    _data->plotOn(chi2plot, Name("data"));
    _pdf->plotOn(chi2plot, Name("pdf"));
    int nfreeparams = _fitres->floatParsFinal().getSize();
    double chi2overdof = chi2plot->chiSquare("pdf", "data", nfreeparams);
    cout << "Chi2 : " << chi2overdof * (_nbin - nfreeparams) << " D.O.F : " << _nbin - nfreeparams << endl;
    return chi2overdof;
}

void FitManager::ShowParamsInfo () 
{
   _fitres->Print("v");
}

void FitManager::HasParameter (const string& name)
{
    bool hasParam = false;
    for (const auto& it : _varstrs) {
        if (it == name) {
            hasParam = true;
            break;
        }
    }
    if (!hasParam) {
        cout << "Don't find '" << name << "' parameter in '" << _pdf->GetName() << "' PDF" << endl; 
        exit(0);
    }
}

RooAbsData* FitManager::UnWgtDataSetBuilder (RooRealVar* var, RooDataSet* dset, string datatype, double minvar, double maxvar, int nbin) 
{
    double min = TMath::Max(minvar, var->getMin());
    double max = TMath::Min(maxvar, var->getMax());
    string varname = var->GetName();
    RooDataSet* reducedate = (RooDataSet*) dset->reduce(Form("%s > %f && %s < %f", varname.c_str(), min, varname.c_str(), max));
    var->setRange(min, max);
    RooAbsData* datamodified = nullptr;

    if (dset->isWeighted()) {
        if (datatype == "unbinned") {
            var->setBins(dset->numEntries());
            RooDataHist datahist("datahist", "", RooArgSet(*var), *reducedate);
            RooHistPdf histpdf("histpdf", "", RooArgSet(*var), datahist);
            RooRandom::randomGenerator()->SetSeed(0);
            datamodified = histpdf.generate(RooArgSet(*var), dset->numEntries());
        } else if (datatype == "binned") {
            var->setBins(nbin);
            RooDataHist datahist("datahist", "", RooArgSet(*var), *reducedate);
            RooHistPdf histpdf("histpdf", "", RooArgSet(*var), datahist);
            datamodified = histpdf.generateBinned(RooArgSet(*var), dset->numEntries(), kTRUE, kFALSE);
        } else {
            cout << "Please input binned or unbinned" << endl;
            exit(0);
        }
    } else {
        datamodified = reducedate;
    }

    return datamodified;
}
