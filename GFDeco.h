//
// Created by aren on 9/12/17.
//

#ifndef DECOCLI_GFDECO_H
#define DECOCLI_GFDECO_H

#include "Deco.h"

class GFDeco: public Deco {
public:
    /// Public Dive parameters
    double GFHigh = 0.8;    // Gradient Factor High
    double GFLow = 0.3;     // Gradient Factor Low

    /// Functions
    explicit GFDeco();
    GFDeco(const GFDeco& deco);
    double GetCeiling();
    int GetNoDecoTime();

    GFDeco::DecoStop GetNextDecoStop();
    std::vector<DecoStop> GetDecoSchedule();
    double GetMValue(int TissueIndex, double depth);
    double GetGFPoint(double depth);
};

#endif //DECOCLI_GFDECO_H
