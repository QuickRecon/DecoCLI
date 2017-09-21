//  Program for calculating decompression stops and no stop times using buehlmann and buehlmann derived algorithms
//  Copyright (C) 2017 Aren Leishman
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>
//


#include "GFDeco.h"

double GFDeco::GetCeiling() {
    this->LimitingTissueIndex = 0;
    for (int i = 0; i < 16; i++) {
        GFDeco DecoSim = GFDeco(*this);
        double currentCeiling = 0;
        bool inLimits = false;
        while (!inLimits) {
            double Pn = DecoSim.Pn[i];
            double Ph = DecoSim.Ph[i];
            double pA = DecoSim.pA;
            double MaxGF = DecoSim.GetGFPoint(currentCeiling);
            double TheoreticalGF = ((Pn + Ph) - pA) / (DecoSim.GetMValue(i, currentCeiling) - pA);

            if (TheoreticalGF < MaxGF && !std::isnan(TheoreticalGF)) {
                inLimits = true;
            } else {
                currentCeiling += 0.1;
            }
        }
        this->TissueAccentCeiling[i] = currentCeiling;

        if (this->TissueAccentCeiling[i] > this->TissueAccentCeiling[LimitingTissueIndex]) {
            LimitingTissueIndex = i;
        }
    }
    double ceiling = this->TissueAccentCeiling[LimitingTissueIndex];
    this->AccentCeiling = ceiling;
    return ceiling;
}

double GFDeco::GetMValue(int TissueIndex, double depth) {
    SetPartialPressures(depth);
    double ppN2 = this->ppN2;
    double aN2 = GFDeco::buehlmann_N2_a[TissueIndex];
    double bN2 = GFDeco::buehlmann_N2_b[TissueIndex];

    double ppHe = this->ppHe;
    double aHe = GFDeco::buehlmann_He_a[TissueIndex];
    double bHe = GFDeco::buehlmann_He_b[TissueIndex];

    double a = ((aN2 * ppN2) + (aHe * ppHe)) / (ppN2 + ppHe);
    double b = ((bN2 * ppN2) + (bHe * ppHe)) / (ppN2 + ppHe);

    return (depth / b) + a;
}

double GFDeco::GetGFPoint(double depth) {
    double GFHigh = this->GFHigh;
    double GFLow = this->GFLow;
    double LowDepth = this->MaximumDepth;

    return GFHigh - ((GFHigh - GFLow) / LowDepth) * depth;
}

GFDeco::GFDeco(const GFDeco &GFDeco) {

    this->gases = GFDeco.gases;
    this->Depth = GFDeco.Depth;
    this->MaximumDepth = GFDeco.MaximumDepth;

    this->AccentCeiling = GFDeco.AccentCeiling;
    this->CurrentGas = GFDeco.CurrentGas;
    this->GFHigh = GFDeco.GFHigh;
    this->GFLow = GFDeco.GFLow;
    this->LimitingTissueIndex = GFDeco.LimitingTissueIndex;

    this->ppN2 = GFDeco.ppN2;
    this->ppHe = GFDeco.ppHe;
    this->ppO2 = GFDeco.ppO2;

    this->pA = GFDeco.pA;
    this->ppWv = GFDeco.ppWv;

    for (int i = 0; i < 16; i++) {
        this->TissueAccentCeiling[i] = GFDeco.TissueAccentCeiling[i];
        this->Pn[i] = GFDeco.Pn[i];
        this->Ph[i] = GFDeco.Ph[i];
        this->Pt[i] = GFDeco.Pt[i];
    }
}