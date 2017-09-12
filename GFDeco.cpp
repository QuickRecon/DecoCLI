//
// Created by aren on 9/12/17.
//

#include "GFDeco.h"

/// Compartment Tables (Taken from Subsurface Deco.c)
const double GFDeco::buehlmann_N2_a[] = { 1.1696, 1.0, 0.8618, 0.7562,
                                        0.62, 0.5043, 0.441, 0.4,
                                        0.375, 0.35, 0.3295, 0.3065,
                                        0.2835, 0.261, 0.248, 0.2327 };

const double GFDeco::buehlmann_N2_b[] = { 0.5578, 0.6514, 0.7222, 0.7825,
                                        0.8126, 0.8434, 0.8693, 0.8910,
                                        0.9092, 0.9222, 0.9319, 0.9403,
                                        0.9477, 0.9544, 0.9602, 0.9653 };

const double GFDeco::buehlmann_N2_halflife[] = { 5.0, 8.0, 12.5, 18.5,
                                               27.0, 38.3, 54.3, 77.0,
                                               109.0, 146.0, 187.0, 239.0,
                                               305.0, 390.0, 498.0, 635.0 };

const double GFDeco::buehlmann_He_a[] = { 1.6189, 1.383, 1.1919, 1.0458,
                                        0.922, 0.8205, 0.7305, 0.6502,
                                        0.595, 0.5545, 0.5333, 0.5189,
                                        0.5181, 0.5176, 0.5172, 0.5119 };

const double GFDeco::buehlmann_He_b[] = { 0.4770, 0.5747, 0.6527, 0.7223,
                                        0.7582, 0.7957, 0.8279, 0.8553,
                                        0.8757, 0.8903, 0.8997, 0.9073,
                                        0.9122, 0.9171, 0.9217, 0.9267 };

const double GFDeco::buehlmann_He_halflife[] = { 1.88, 3.02, 4.72, 6.99,
                                               10.21, 14.48, 20.53, 29.11,
                                               41.20, 55.19, 70.69, 90.34,
                                               115.29, 147.42, 188.24, 240.03 };



GFDeco::gas::gas(double FrN2, double FrO2, double FrHe) {
    this->FrN2 = FrN2;
    this->FrO2 = FrO2;
    this->FrHe = FrHe;
}

GFDeco::DecoStop::DecoStop(double Depth, double Time) {
    this->Depth = Depth;
    this->Time = Time;
}

void GFDeco::SetPartialPressures(double depth) {
    this->pA = depth;
    this->ppN2 = gases.data()[0].FrN2 * (this->pA - this->ppWv);
    this->ppHe = gases.data()[0].FrHe * (this->pA - this->ppWv);
    this->ppO2 = gases.data()[0].FrO2 * (this->pA - this->ppWv);
}

double GFDeco::GetCeiling() {
    this->LimitingTissueIndex = 0;
    for(int i = 0; i < 16; i++){
        double currentCeiling = 0;
        bool inLimits = false;
        while(!inLimits){
            double Pn = this->Pn[i];
            double Ph = this->Ph[i];
            double pA = this->pA;
            double MaxGF = GetGFPoint(currentCeiling);
            double TheoreticalGF = ((Pn+Ph)-pA)/(this->GetMValue(i,currentCeiling)-pA);

            if(TheoreticalGF < MaxGF && !isnanf(TheoreticalGF)){
                inLimits = true;
            } else{
                currentCeiling += 0.1;
            }
        }
        this->TissueAccentCeiling[i] = currentCeiling;

        if(this->TissueAccentCeiling[i] > this->TissueAccentCeiling[LimitingTissueIndex]){
            LimitingTissueIndex = i;
        }
    }
    double ceiling = this->TissueAccentCeiling[LimitingTissueIndex];
    this->AccentCeiling = ceiling;
    return ceiling;
}

double GFDeco::GetMValue(int TissueIndex, double depth){
    SetPartialPressures(depth);
    double ppN2 = this->ppN2;
    double aN2 = GFDeco::buehlmann_N2_a[TissueIndex];
    double bN2 = GFDeco::buehlmann_N2_b[TissueIndex];

    double ppHe = this->ppHe;
    double aHe = GFDeco::buehlmann_He_a[TissueIndex];
    double bHe = GFDeco::buehlmann_He_b[TissueIndex];

    double a = ((aN2 * ppN2) + (aHe * ppHe))/(ppN2 + ppHe);
    double b = ((bN2 * ppN2) + (bHe * ppHe))/(ppN2 + ppHe);

    return (depth/b) + a;
}

double GFDeco::GetGFPoint(double depth) {
    double GFHigh = this->GFHigh;
    double GFLow = this->GFLow;
    double LowDepth = this->MaximumDepth;

    return GFHigh - ((GFHigh - GFLow)/LowDepth) * depth;
}

int GFDeco::GetNoDecoTime() {
    int noStopTime = 0;
    bool inLimits = true;
    while(inLimits){
        GFDeco GFDecoSim = GFDeco(*this);
        GFDecoSim.AddBottom(noStopTime);
        inLimits = GFDecoSim.GetCeiling() < 1;
        noStopTime++;
    }
    noStopTime -= 1;
    return noStopTime;
}

GFDeco::DecoStop GFDeco::GetNextDecoStop() {
    // Round GFDeco depth to next multiple of 3m (return as bar)
    double StopDepth = MeterToBar(ceil(BarToMeter(GFDeco::GetCeiling())/3)*3);

    int StopTime = 0;
    bool inLimits = false;
    while(!inLimits){
        GFDeco GFDecoSim = GFDeco(*this);
        GFDecoSim.AddDecent(StopDepth, -MeterToBar(GFDeco::AccentRate));
        GFDecoSim.AddBottom(StopTime);
        inLimits = GFDecoSim.GetCeiling() < StopDepth - 0.3;
        StopTime++;
    }
    return {StopDepth, StopTime};
}

std::vector<GFDeco::DecoStop> GFDeco::GetDecoSchedule() {
    std::vector<GFDeco::DecoStop> Schedule;
    GFDeco GFDecoSim = GFDeco(*this);
    while(GFDecoSim.GetCeiling() > 1){
        GFDeco::DecoStop stop = GFDecoSim.GetNextDecoStop();
        Schedule.emplace_back(stop);
        GFDecoSim.AddDecent(stop.Depth, -MeterToBar(GFDeco::AccentRate));
        GFDecoSim.AddBottom(stop.Time);
    }
    return Schedule;
}

void GFDeco::AddDecent(double depth, double DecentRate) {
    DecentRate -= 1;
    if(depth > this->MaximumDepth){
        this->MaximumDepth = depth;
    }
    double DeltaDepth = depth - this->Depth;
    for(int i = 0; i < 16; i++){
        /// General Values
        double t = DeltaDepth/DecentRate;

        /// Calculate Nitrogen
        double Pn;
        double ppN2 = this->ppN2;
        double CurrentPn = this->Pn[i];
        double RN2 = DecentRate * this->gases[CurrentGas].FrN2;
        double kN2 = log(2)/GFDeco::buehlmann_N2_halflife[i];
        Pn = ppN2 + RN2*(t-(1/kN2))-(ppN2 - CurrentPn - (RN2/kN2)) * exp(-kN2*t);

        /// Calculate Helium
        double Ph;
        double ppHe = this->ppHe;
        double CurrentPh = this->Ph[i];
        double RHe = DecentRate * this->gases[CurrentGas].FrHe;
        double kHe = log(2)/GFDeco::buehlmann_He_halflife[i];
        Ph = ppHe + RHe*(t-(1/kHe))-(ppHe - CurrentPh - (RHe/kHe)) * exp(-kHe*t);

        /// Set Loading
        SetGasLoadings(Pn, Ph, i);
    }
    SetPartialPressures(depth);
    this->Depth = depth;
}

void GFDeco::AddBottom(double time) {
    for(int i = 0; i < 16; i++){
        /// Calculate Nitrogen
        double Pn;
        double CurrentPn = this->Pn[i];
        double ppN2 = this->ppN2;
        double halftimeN2 = GFDeco::buehlmann_N2_halflife[i];
        Pn = CurrentPn + (ppN2-CurrentPn)*(1-pow(2,-time/halftimeN2));

        /// Calculate Helium
        double Ph;
        double CurrentPh = this->Ph[i];
        double ppHe = this->ppHe;
        double halftimeHe = GFDeco::buehlmann_He_halflife[i];
        Ph = CurrentPh + (ppHe-CurrentPh)*(1-pow(2,-time/halftimeHe));

        /// Set Loading
        SetGasLoadings(Pn, Ph, i);
    }
}

GFDeco::GFDeco(double ppWv) {
    this->ppWv = ppWv;
    this->Depth = 1; //1 bar at atmospheric pressure
    this->AccentCeiling = -1000;

    /// Configure default gas (Air)
    gases.emplace_back(GFDeco::gas(0.79, 0.21, 0));

    SetPartialPressures(1);
    /// Create gas compartments
    for(int i=0; i < 16; i++){
        this->TissueAccentCeiling[i] = -1000;
        this->SetGasLoadings(this->ppN2, 0, i);
    }
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

    for(int i = 0; i < 16; i++){
        this->TissueAccentCeiling[i] = GFDeco.TissueAccentCeiling[i];
        this->Pn[i] = GFDeco.Pn[i];
        this->Ph[i] = GFDeco.Ph[i];
        this->Pt[i] = GFDeco.Pt[i];
    }
}

void GFDeco::SetGasLoadings(double Pn, double Ph, int compartmentIndex) {
    this->Pn[compartmentIndex] = Pn;
    this->Ph[compartmentIndex] = Ph;
    this->Pt[compartmentIndex] = Pn + Ph;
}
