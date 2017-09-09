//
// Created by aren on 9/7/17.
//

#ifndef DECOCLI_DECO_H
#define DECOCLI_DECO_H


class Deco {
    /// gas parameters
    double ppN2;        // partial pressure Nitrogen
    double ppHe;        // parital pressure Helium
    double ppO2;        // partial pressure Oxygen
    struct gas {
        double FrN2;    // Fractional percent Nitrogen
        double FrO2;    // Fractional percent Oxygen
        double FrHe;    // Fractional percent Helium
    };

    /// Environmental Parameters
    double pA;          // Ambient pressure
    double ppWv;        // Partial pressure water vapor

};


#endif //DECOCLI_DECO_H
