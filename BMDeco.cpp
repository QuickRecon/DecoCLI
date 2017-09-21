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


#include "BMDeco.h"

double BMDeco::GetCeiling() {
    this->LimitingTissueIndex = 0;
    for (int i = 0; i < 16; i++) {
        double Pn = this->Pn[i];
        double aN2 = BMDeco::buehlmann_N2_a[i];
        double bN2 = BMDeco::buehlmann_N2_b[i];

        double Ph = this->Ph[i];
        double aHe = BMDeco::buehlmann_He_a[i];
        double bHe = BMDeco::buehlmann_He_b[i];

        double a = ((aN2 * Pn) + (aHe * Ph)) / (Pn + Ph);
        double b = ((bN2 * Pn) + (bHe * Ph)) / (Pn + Ph);

        this->TissueAccentCeiling[i] = ((Pn + Ph) - a) * b;

        if (this->TissueAccentCeiling[i] > this->TissueAccentCeiling[LimitingTissueIndex]) {
            LimitingTissueIndex = i;
        }
    }
    double ceiling = this->TissueAccentCeiling[LimitingTissueIndex];
    this->AccentCeiling = ceiling;
    return ceiling;
}

BMDeco::BMDeco(const BMDeco &deco) {

    this->gases = deco.gases;
    this->Depth = deco.Depth;

    this->AccentCeiling = deco.AccentCeiling;
    this->CurrentGas = deco.CurrentGas;
    this->LimitingTissueIndex = deco.LimitingTissueIndex;

    this->ppN2 = deco.ppN2;
    this->ppHe = deco.ppHe;
    this->ppO2 = deco.ppO2;

    this->pA = deco.pA;
    this->ppWv = deco.ppWv;

    for (int i = 0; i < 16; i++) {
        this->TissueAccentCeiling[i] = deco.TissueAccentCeiling[i];
        this->Pn[i] = deco.Pn[i];
        this->Ph[i] = deco.Ph[i];
        this->Pt[i] = deco.Pt[i];
    }
}

