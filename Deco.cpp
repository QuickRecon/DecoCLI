//
// Created by aren on 9/7/17.
// Algorithm based of http://www.lizardland.co.uk/DIYDeco.html
//

#include "Deco.h"

/// Compartment Tables (Taken from Subsurface Deco.c)
const double Deco::buehlmann_N2_a[] = { 1.1696, 1.0, 0.8618, 0.7562,
                                  0.62, 0.5043, 0.441, 0.4,
                                  0.375, 0.35, 0.3295, 0.3065,
                                  0.2835, 0.261, 0.248, 0.2327 };

const double Deco::buehlmann_N2_b[] = { 0.5578, 0.6514, 0.7222, 0.7825,
                                  0.8126, 0.8434, 0.8693, 0.8910,
                                  0.9092, 0.9222, 0.9319, 0.9403,
                                  0.9477, 0.9544, 0.9602, 0.9653 };

const double Deco::buehlmann_N2_halflife[] = { 5.0, 8.0, 12.5, 18.5,
                                           27.0, 38.3, 54.3, 77.0,
                                           109.0, 146.0, 187.0, 239.0,
                                           305.0, 390.0, 498.0, 635.0 };

const double Deco::buehlmann_He_a[] = { 1.6189, 1.383, 1.1919, 1.0458,
                                  0.922, 0.8205, 0.7305, 0.6502,
                                  0.595, 0.5545, 0.5333, 0.5189,
                                  0.5181, 0.5176, 0.5172, 0.5119 };

const double Deco::buehlmann_He_b[] = { 0.4770, 0.5747, 0.6527, 0.7223,
                                  0.7582, 0.7957, 0.8279, 0.8553,
                                  0.8757, 0.8903, 0.8997, 0.9073,
                                  0.9122, 0.9171, 0.9217, 0.9267 };

const double Deco::buehlmann_He_halflife[] = { 1.88, 3.02, 4.72, 6.99,
                                           10.21, 14.48, 20.53, 29.11,
                                           41.20, 55.19, 70.69, 90.34,
                                           115.29, 147.42, 188.24, 240.03 };

double Deco::BarToMeter(double bar) {
    return 10*(bar-1);
}

double Deco::MeterToBar(double meter) {
    return 0.1*meter+1;
}

Deco::gas::gas(double FrN2, double FrO2, double FrHe) {
    this->FrN2 = FrN2;
    this->FrO2 = FrO2;
    this->FrHe = FrHe;
}

void Deco::SetPartialPressures(double depth) {
    this->pA = depth;
    this->ppN2 = gases.data()[0].FrN2 * this->pA;
    this->ppHe = gases.data()[0].FrHe * this->pA;
    this->ppO2 = gases.data()[0].FrO2 * this->pA;
}

double Deco::GetCeiling() {
    this->LimitingTissueIndex = 0;
    for(int i = 0; i < 16; i++){
        double Pn = this->Pn[i];
        double aN2 = Deco::buehlmann_N2_a[i];
        double bN2 = Deco::buehlmann_N2_b[i];

        double Ph = this->Ph[i];
        double aHe = Deco::buehlmann_He_a[i];
        double bHe = Deco::buehlmann_He_b[i];

        double a = ((aN2 * Pn) + (aHe * Ph))/(Pn + Ph);
        double b = ((bN2 * Pn) + (bHe * Ph))/(Pn + Ph);

        this->TissueAccentCeiling[i] = ((Pn + Ph)-a)*b;

        if(this->TissueAccentCeiling[i] > this->TissueAccentCeiling[LimitingTissueIndex]){
            LimitingTissueIndex = i;
        }
    }
    double ceiling = this->TissueAccentCeiling[LimitingTissueIndex];
    this->AccentCeiling = ceiling;
    return ceiling;
}

void Deco::AddDecent(double depth, double DecentRate) {
    DecentRate -= 1;
    //SetPartialPressures(depth);
    double DeltaDepth = depth - this->depth;
    for(int i = 0; i < 16; i++){
        /// General Values
        double pA = depth;
        double t = DeltaDepth/DecentRate;

        /// Calculate Nitrogen
        double Pn;
        double ppN2 = this->ppN2;
        double CurrentPn = this->Pn[i];
        double RN2 = DecentRate * this->gases[CurrentGas].FrN2;
        double kN2 = log(2)/Deco::buehlmann_N2_halflife[i];
        Pn = ppN2 + RN2*(t-(1/kN2))-(ppN2 - CurrentPn - (RN2/kN2)) * exp(-kN2*t);

        /// Calculate Helium
        double Ph;
        double ppHe = this->ppHe;
        double CurrentPh = this->Ph[i];
        double RHe = DecentRate * this->gases[CurrentGas].FrHe;
        double kHe = log(2)/Deco::buehlmann_He_halflife[i];
        Ph = ppHe + RHe*(t-(1/kHe))-(ppHe - CurrentPh - (RHe/kHe)) * exp(-kHe*t);

        /// Set Loading
        SetGasLoadings(Pn, Ph, i);
    }
    SetPartialPressures(depth);
    this->depth = depth;
}

void Deco::AddBottom(double time) {
    for(int i = 0; i < 16; i++){
        /// Calculate Nitrogen
        double Pn;
        double CurrentPn = this->Pn[i];
        double ppN2 = this->ppN2;
        double halftimeN2 = Deco::buehlmann_N2_halflife[i];
        Pn = CurrentPn + (ppN2-CurrentPn)*(1-pow(2,-time/halftimeN2));

        // Calculate Helium
        double Ph;
        double CurrentPh = this->Ph[i];
        double ppHe = this->ppHe;
        double halftimeHe = Deco::buehlmann_He_halflife[i];
        Ph = CurrentPh + (ppHe-CurrentPh)*(1-pow(2,-time/halftimeHe));

        /// Set Loading
        SetGasLoadings(Pn, Ph, i);
    }
}

Deco::Deco(double ppWv) {
    this->ppWv = ppWv;
    this->depth = 1; //1 bar at atmospheric pressure
    this->AccentCeiling = -1000;

    /// Configure default gas (Air)
    gases.push_back(Deco::gas(0.79, 0.21, 0));

    SetPartialPressures(1);
    /// Create gas compartments
    for(int i=0; i < 16; i++){
        this->TissueAccentCeiling[i] = -1000;
        this->SetGasLoadings(this->ppN2, 0, i);
    }
}

void Deco::SetGasLoadings(double Pn, double Ph, int compartmentIndex) {
    this->Pn[compartmentIndex] = Pn;
    this->Ph[compartmentIndex] = Ph;
    this->Pt[compartmentIndex] = Pn + Ph;
}