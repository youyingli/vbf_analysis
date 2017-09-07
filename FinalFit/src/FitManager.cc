#include "vbf_analysis/FinalFit/interface/FitManager.h"
#include "RooRealVar.h"

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
    _dhist = nullptr;
}

FitManager::~FitManager ()
{
    delete _varset;
    delete _dhist;
}

void FitManager::SetDataSetFitted (RooDataSet* dataset, bool isbinned, int nbin)
{
    if (isbinned) {
        delete _dhist;
        RooRealVar* var = (RooRealVar*) _varset->first();
        var->setBins(nbin);
        _nbin = nbin;
        _dhist = new RooDataHist("dhist", "", RooArgSet(*var), *dataset);
    } else {
        _dset = dataset;
    }
}

void FitManager::ImportTH1 (TH1* histfitted) 
{
    delete _dhist;
    _dhist = new RooDataHist("dhist", "", RooArgList(*_varset), histfitted);
}

void FitManager::UseUnBinnedFitting (double minvar, double maxvar)
{
    int stat = -1;
    int count = 1;
    while (stat != 0) {
        if (count>5) {
            cout << "Don't find the stable fitting for " << _pdf->GetName() << " PDF!" << endl;
            cout << "Please change the values or the range of parameters in " << _pdf->GetName() << " PDF !!" << endl;
            exit(0);
        }
        _fitres = _pdf->fitTo(*_dset, Minimizer("Minuit2","minimize"), Save(true), SumW2Error(true), Strategy(1), Range(minvar, maxvar));
        stat = _fitres->status();
        if (stat != 0) _params->assignValueOnly(_fitres->randomizePars());
        count++;
    }
}

void FitManager::Use1DBinnedFitting (double minvar, double maxvar)
{
    if (_varset->getSize() != 1) {
        cout << "Binned fit only supports 1 dimensional fitting!" << endl;
        exit(0);
    }
    int stat = -1;
    int count = 1;
    RooRealVar* var = (RooRealVar*) _varset->first();
    var->setRange("range", minvar, maxvar);
    while (stat != 0) {
        if (count>5) {
            cout << "Don't find the stable fitting for " << _pdf->GetName() << " PDF!" << endl;
            cout << "Please change the values or the range of parameters in " << _pdf->GetName() << " PDF !!" << endl;
            exit(0);
        }
        _fitres = _pdf->chi2FitTo(*_dhist, Minimizer("Minuit2","minimize"), Save(true), DataError(RooAbsData::Auto), Strategy(1), Range("range"));
        stat = _fitres->status();
        if (stat != 0) _params->assignValueOnly(_fitres->randomizePars());
        count++;
    }
    _fullrange = var->getMax() - var->getMin();
    _fitrange = maxvar - minvar;
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
    int nbin = _fitrange / (_fullrange / _nbin);
    int nfreeparams = _fitres->floatParsFinal().getSize();
    cout << "Chi2 : " << _fitres->minNll() << " D.O.F : " << nbin - nfreeparams << endl;
    return _fitres->minNll() / (nbin - nfreeparams);
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
