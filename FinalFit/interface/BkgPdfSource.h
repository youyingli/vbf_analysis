#ifndef __BKGPDFSOURCE__
#define __BKGPDFSOURCE__

#include <string>

#include "RooRealVar.h"
#include "RooAbsPdf.h"

extern RooAbsPdf* EnvelopePdf (RooRealVar* var, std::string pdfType, int order);

#endif
