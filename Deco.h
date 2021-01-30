//  Program for calculating decompression stops and no stop times using buhlmann and buhlmann derived algorithms
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
    struct Gas {
        double FrN2;        // Fractional percent Nitrogen
        double FrO2;        // Fractional percent Oxygen
        double FrHe;        // Fractional percent Helium
        Gas();
        Gas(double FrN2, double FrO2, double FrHe);
    };

    std::vector<Gas> gases;

    struct DecoStop {
        DecoStop();

        double Depth;
        double Time;
        int Gas;
        DecoStop(double Depth, double Time, int gas);
    };

    /// Public Dive parameters
    double DecentRate = 30; // Decent Rate in meters
    double AccentRate = -18;// Accent Rate in meters
    int CurrentGas = 0;     // Index of current Gas

    /// Functions
    explicit Deco();
    ~Deco();

    Deco(const Deco &deco);

    void SetGasLoadings(double pn, double ph, int compartmentIndex);

    void SetPartialPressures(double depth);

    void SetppWv(double ppwv);

    double GetCeiling();

    virtual double GetNoDecoTime();

    std::vector<DecoStop> GetDecoSchedule();

    void AddDecent(double depth, double decentRate);

    void AddBottom(double time);

    void AddGas(double FrN2, double FrO2, double FrHe);

    Deco::DecoStop GetNextDecoStop();

    double GetGFPoint(double depth);

    double FirstStopDepth;

    /// Dive Parameters
    double Depth = 1;
    double MaximumDepth = 1;
    double TissueAccentCeiling[16]; //In Bar
    double AccentCeiling;   // In meters
    int LimitingTissueIndex;
    double GFHigh = 0.7;    // Gradient Factor High
    double GFLow = 0.4;     // Gradient Factor Low

    /// Gas parameters
    double ppN2;            // partial pressure Nitrogen
    double ppHe;            // partial pressure Helium
    double ppO2;            // partial pressure Oxygen

    /// Gas Loadings
    double Pn[16];          // Nitrogen Gas loading
    double Ph[16];          // Helium Gas loading
    double Pt[16];          // Total Gas loading

    /// Environmental Parameters
    double pA = 1;              // Ambient pressure
    double ppWv = 0;            // Partial pressure water vapor

    /// Compartment Tables (Taken from Subsurface Deco.c)
    static const double buhlmann_N2_a[];
    static const double buhlmann_N2_b[];
    static const double buhlmann_N2_halflife[];

    static const double buhlmann_He_a[];
    static const double buhlmann_He_b[];
    static const double buhlmann_He_halflife[];

    void SwitchGas(int gasIndex);

    int BestGas(double depth, double threshold);

    DecoStop GetNextDecoStop(double startTime);

    double decoPPO2 = 1.62;
};


#endif //DECOCLI_DECO_H
