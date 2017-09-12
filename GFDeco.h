//
// Created by aren on 9/12/17.
//

#ifndef DECOCLI_GFDECO_H
#define DECOCLI_GFDECO_H

#include <vector>
#include <cmath>
#include "Utilities.h"

class GFDeco {
public:
        /// Data Structures
        struct gas {
            double FrN2;        // Fractional percent Nitrogen
            double FrO2;        // Fractional percent Oxygen
            double FrHe;        // Fractional percent Helium
            gas(double FrN2, double FrO2, double FrHe);
        };

        std::vector<gas> gases;

        struct DecoStop{
            double Depth;
            double Time;
            DecoStop(double Depth, double Time);
        };

        /// Public Dive parameters
        double DecentRate = 30; // Decent Rate in meters
        double GFHigh = 0.8;    // Gradient Factor High
        double GFLow = 0.3;     // Gradient Factor Low
        double AccentRate = -18;// Accent Rate in meters
        int CurrentGas = 0;     // Index of current gas

        /// Functions
        explicit GFDeco(double ppWv);
        GFDeco(const GFDeco& deco);

        void SetGasLoadings(double Pn, double Ph, int compartmentIndex);
        void SetPartialPressures(double depth);

        double GetCeiling();
        int GetNoDecoTime();
        double GetMValue(int TissueIndex, double depth);
        double GetGFPoint(double depth);
        GFDeco::DecoStop GetNextDecoStop();
        std::vector<DecoStop> GetDecoSchedule();

        void AddDecent(double depth, double DecentRate);
        void AddBottom(double time);
    private:
        /// Dive Parameters
        double Depth = 1;
        double MaximumDepth = 1;
        double TissueAccentCeiling[16]; //In Bar
        double AccentCeiling;   // In meters
        int LimitingTissueIndex;

        /// gas parameters
        double ppN2;            // partial pressure Nitrogen
        double ppHe;            // parital pressure Helium
        double ppO2;            // partial pressure Oxygen

        /// Gas Loadings
        double Pn[16];          // Nitrogen gas loading
        double Ph[16];          // Helium gas loading
        double Pt[16];          // Total gas loading

        /// Environmental Parameters
        double pA;              // Ambient pressure
        double ppWv;            // Partial pressure water vapor

        /// Compartment Tables (Taken from Subsurface Deco.c)
        static const double buehlmann_N2_a[];
        static const double buehlmann_N2_b[];
        static const double buehlmann_N2_halflife[];

        static const double buehlmann_He_a[];
        static const double buehlmann_He_b[];
        static const double buehlmann_He_halflife[];
};


#endif //DECOCLI_GFDECO_H