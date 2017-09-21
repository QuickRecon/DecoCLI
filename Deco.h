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

#ifndef DECOCLI_DECO_H
#define DECOCLI_DECO_H

#include <vector>
#include <cmath>
#include "Utilities.h"

class Deco {
public:
    /// Data Structures
    struct gas {
        double FrN2;        // Fractional percent Nitrogen
        double FrO2;        // Fractional percent Oxygen
        double FrHe;        // Fractional percent Helium
        gas(double FrN2, double FrO2, double FrHe);
    };

    std::vector<gas> gases;

    struct DecoStop {
        double Depth;
        int Time;

        DecoStop(double Depth, int Time);
    };

    /// Public Dive parameters
    double DecentRate = 30; // Decent Rate in meters
    double AccentRate = -18;// Accent Rate in meters
    int CurrentGas = 0;     // Index of current gas

    /// Functions
    explicit Deco();

    Deco(const Deco &deco);

    void SetGasLoadings(double Pn, double Ph, int compartmentIndex);

    void SetPartialPressures(double depth);

    void SetppWv(double ppWv);

    virtual double GetCeiling() = 0;

    virtual long GetNoDecoTime() = 0;

    virtual std::vector<DecoStop> GetDecoSchedule() = 0;

    void AddDecent(double depth, double DecentRate);

    void AddBottom(double time);

    void AddGas(double FrN2, double FrO2, double FrHe);

    /// Dive Parameters
    double Depth = 1;
    double MaximumDepth = 1;
    double TissueAccentCeiling[16]; //In Bar
    double AccentCeiling;   // In meters
    int LimitingTissueIndex;
    double GFHigh = 0.8;    // Gradient Factor High
    double GFLow = 0.3;     // Gradient Factor Low

    /// gas parameters
    double ppN2;            // partial pressure Nitrogen
    double ppHe;            // partial pressure Helium
    double ppO2;            // partial pressure Oxygen

    /// Gas Loadings
    double Pn[16];          // Nitrogen gas loading
    double Ph[16];          // Helium gas loading
    double Pt[16];          // Total gas loading

    /// Environmental Parameters
    double pA = 1;              // Ambient pressure
    double ppWv = 0;            // Partial pressure water vapor

    /// Compartment Tables (Taken from Subsurface Deco.c)
    static const double buehlmann_N2_a[];
    static const double buehlmann_N2_b[];
    static const double buehlmann_N2_halflife[];

    static const double buehlmann_He_a[];
    static const double buehlmann_He_b[];
    static const double buehlmann_He_halflife[];
};


#endif //DECOCLI_DECO_H
