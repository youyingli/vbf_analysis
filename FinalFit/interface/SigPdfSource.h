#ifndef __SIGPDFSOURCE__
#define __SIGPDFSOURCE__

#include <string>

#include "RooRealVar.h"
#include "RooAbsPdf.h"

extern RooAbsPdf* MultiGaussians (std::string signame, RooRealVar* mass, int nGaussians, int mh);
extern RooAbsPdf* MultiCrystalBall (std::string signame, RooRealVar* mass, int nCrystalBall, int mh);

#endif
