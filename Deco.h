//
// Created by aren on 9/7/17.
//

#ifndef DECOCLI_DECO_H
#define DECOCLI_DECO_H

#include <vector>
#include "math.h"

class Deco {
    public:
        /// Functions
        Deco(double ppWv);

        void SetGasLoadings(double Pn, double Ph, int compartmentIndex);
        void SetPartialPressures(double depth);

        double GetCeiling();

        void AddDecent(double depth, double time);
        void AddBottom(double time);

        /// Gas storage
        struct gas {
            double FrN2;    // Fractional percent Nitrogen
            double FrO2;    // Fractional percent Oxygen
            double FrHe;    // Fractional percent Helium
            gas(double FrN2, double FrO2, double FrHe);
        };
        std::vector<gas> gases;

    private:
        /// Dive Parameters
        double depth;
        double TissueAccentCeiling[16];
        double AccentCeiling;
        int LimitingTissueIndex;

        /// gas parameters
        double ppN2;        // partial pressure Nitrogen
        double ppHe;        // parital pressure Helium
        double ppO2;        // partial pressure Oxygen

        /// Gas Loadings
        double Pn[16];          // Nitrogen gas loading
        double Ph[16];          // Helium gas loading
        double Pt[16];          // Total gas loading

        /// Environmental Parameters
        double pA;          // Ambient pressure
        double ppWv;        // Partial pressure water vapor

        /// Compartment Tables (Taken from Subsurface Deco.c)
        static const double buehlmann_N2_a[];
        static const double buehlmann_N2_b[];
        static const double buehlmann_N2_halflife[];

        static const double buehlmann_He_a[];
        static const double buehlmann_He_b[];
        static const double buehlmann_He_halflife[];
};


#endif //DECOCLI_DECO_H
