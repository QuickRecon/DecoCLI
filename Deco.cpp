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

#include "Deco.h"

/// Compartment Tables (Taken from Capra)
const double Deco::buhlmann_N2_a[] = {1.2599, 1.0000, 0.8618, 0.7562, 0.6200, 0.5043, 0.4410, 0.4000, 0.3750, 0.3500, 0.3295, 0.3065,
                                      0.2835, 0.2610, 0.2480, 0.2327};

const double Deco::buhlmann_N2_b[] = {0.5050, 0.6514, 0.7222, 0.7825, 0.8126, 0.8434, 0.8693, 0.8910, 0.9092, 0.9222, 0.9319, 0.9403,
                                      0.9477, 0.9544, 0.9602, 0.9653};

const double Deco::buhlmann_N2_halflife[] = {4.0, 8.0, 12.5, 18.5, 27.0, 38.3, 54.3, 77.0, 109.0, 146.0, 187.0, 239.0, 305.0, 390.0, 498.0,
                                             635.0};

const double Deco::buhlmann_He_a[] = {1.7424, 1.3830, 1.1919, 1.0458, 0.9220, 0.8205, 0.7305, 0.6502, 0.5950, 0.5545, 0.5333, 0.5189,
                                      0.5181, 0.5176, 0.5172, 0.5119};

const double Deco::buhlmann_He_b[] = {0.4245, 0.5747, 0.6527, 0.7223, 0.7582, 0.7957, 0.8279, 0.8553, 0.8757, 0.8903, 0.8997, 0.9073,
                                      0.9122, 0.9171, 0.9217, 0.9267};

const double Deco::buhlmann_He_halflife[] = {1.51, 3.02, 4.72, 6.99, 10.21, 14.48, 20.53, 29.11, 41.20, 55.19, 70.69, 90.34, 115.29, 147.42,
                                             188.24, 240.03};


Deco::Gas::Gas(double FrN2, double FrO2, double FrHe) {
    this->FrN2 = FrN2;
    this->FrO2 = FrO2;
    this->FrHe = FrHe;
}

Deco::Gas::Gas() {
    this->FrN2 = 0.0;
    this->FrO2 = 0.0;
    this->FrHe = 0.0;
}


Deco::DecoStop::DecoStop(double Depth, double Time, int Gas) {
    this->Depth = Depth;
    this->Time = Time;
    this->Gas = Gas;
}

Deco::DecoStop::DecoStop() {
    Depth = 0;
    Time = 0;
    Gas = 0;
}

void Deco::SetPartialPressures(double depth) {
    this->pA = depth;
    this->ppN2 = Gases[CurrentGas].FrN2 * (this->pA - this->ppWv);
    this->ppHe = Gases[CurrentGas].FrHe * (this->pA - this->ppWv);
    this->ppO2 = Gases[CurrentGas].FrO2 * (this->pA - this->ppWv);
}

void Deco::AddDecent(double depth, double decentRate) {
    if (depth > this->MaximumDepth) {
        this->MaximumDepth = depth;
    }
    double DeltaDepth = depth - this->Depth;
    for (int i = 0; i < 16; i++) {
        /// General Values
        double t = fabs(DeltaDepth) / (fabs(decentRate) - 1);

        /// Calculate Nitrogen
        double pn;
        double ppn2 = this->ppN2;
        double CurrentPn = this->Pn[i];
        double RN2 = BarToMeter(decentRate)/(10.0) * this->Gases[CurrentGas].FrN2;
        double kN2 = log(2.0) / Deco::buhlmann_N2_halflife[i];
        pn = ppn2 + RN2 * (t - (1.0 / kN2)) - (ppn2 - CurrentPn - (RN2 / kN2)) * exp(-kN2 * t);

        /// Calculate Helium
        double ph;
        double pphe = this->ppHe;
        double CurrentPh = this->Ph[i];
        double RHe = decentRate * this->Gases[CurrentGas].FrHe;
        double kHe = log(2.0) / Deco::buhlmann_He_halflife[i];
        ph = pphe + RHe * (t - (1.0 / kHe)) - (pphe - CurrentPh - (RHe / kHe)) * exp(-kHe * t);

        /// Set Loading
        SetGasLoadings(pn, ph, i);
    }
    SetPartialPressures(depth);
    this->Depth = depth;
}

void Deco::AddBottom(double time) {
    for (int i = 0; i < 16; i++) {
        /// Calculate Nitrogen
        double pn;
        double CurrentPn = this->Pn[i];
        double ppN2 = this->ppN2;
        double halftimeN2 = Deco::buhlmann_N2_halflife[i];
        pn = CurrentPn + (ppN2 - CurrentPn) * (1.0 - pow(2.0, -time / halftimeN2));

        /// Calculate Helium
        double ph;
        double CurrentPh = this->Ph[i];
        double ppHe = this->ppHe;
        double halftimeHe = Deco::buhlmann_He_halflife[i];
        ph = CurrentPh + (ppHe - CurrentPh) * (1.0 - pow(2.0, -time / halftimeHe));

        /// Set Loading
        SetGasLoadings(pn, ph, i);
    }
}

Deco::Deco() {
    this->Depth = 1; //1 bar at atmospheric pressure
    SetppWv(0.06257);
    /// Configure default Gas (Air)
    AddGas(0.79, 0.21, 0);

    SetPartialPressures(1);
    /// Create Gas compartments
    for (int i = 0; i < 16; i++) {
        this->SetGasLoadings(this->ppN2, 0, i);
    }
    FirstStopDepth = -1;
}

Deco::Deco(const Deco &Deco) {
    FirstStopDepth = Deco.FirstStopDepth;
    this->Gases = Deco.Gases;
    this->Depth = Deco.Depth;
    this->MaximumDepth = Deco.MaximumDepth;

    this->AccentCeiling = Deco.AccentCeiling;
    this->CurrentGas = Deco.CurrentGas;
    this->LimitingTissueIndex = Deco.LimitingTissueIndex;

    this->ppN2 = Deco.ppN2;
    this->ppHe = Deco.ppHe;
    this->ppO2 = Deco.ppO2;

    this->pA = Deco.pA;
    this->ppWv = Deco.ppWv;

    for (int i = 0; i < 16; i++) {
        this->TissueAccentCeiling[i] = Deco.TissueAccentCeiling[i];
        this->Pn[i] = Deco.Pn[i];
        this->Ph[i] = Deco.Ph[i];
        this->Pt[i] = Deco.Pt[i];
    }
}

void Deco::SetGasLoadings(double pn, double ph, int compartmentIndex) {
    this->Pn[compartmentIndex] = pn;
    this->Ph[compartmentIndex] = ph;
    this->Pt[compartmentIndex] = pn + ph;
}

void Deco::SetppWv(double ppwv) {
    this->ppWv = ppwv;
}

void Deco::AddGas(double FrN2, double FrO2, double FrHe) {
    Gases.emplace_back(Deco::Gas(FrN2, FrO2, FrHe));
}

void Deco::SwitchGas(int gasIndex) {
    CurrentGas = gasIndex;
}

int Deco::BestGas(double depth, double threshold) const{
    int bestGas = 0;
    for (int i = 0; i < Gases.size(); i++) {
        if(Gases[i].FrO2 * depth <= threshold && Gases[i].FrO2 >= Gases[bestGas].FrO2)
        {
            bestGas = i;
        }
    }
    return bestGas;
}

Deco::~Deco() {
    Gases.clear();
}

double Deco::GetCeiling() {
    this->LimitingTissueIndex = 0;
    for (int i = 0; i < 16; i++) {
        double Pn = this->Pn[i];
        double aN2 = Deco::buhlmann_N2_a[i];
        double bN2 = Deco::buhlmann_N2_b[i];

        double Ph = this->Ph[i];
        double aHe = Deco::buhlmann_He_a[i];
        double bHe = Deco::buhlmann_He_b[i];

        double a = ((aN2 * Pn) + (aHe * Ph)) / (Pn + Ph);
        double b = ((bN2 * Pn) + (bHe * Ph)) / (Pn + Ph);

        double gf = GetGFPoint(Depth);
        this->TissueAccentCeiling[i] = ((Pn + Ph) - (a * gf)) / (gf / b + 1.0 - gf);

        if (this->TissueAccentCeiling[i] > this->TissueAccentCeiling[LimitingTissueIndex]) {
            LimitingTissueIndex = i;
        }
    }
    double ceiling = this->TissueAccentCeiling[LimitingTissueIndex];
    this->AccentCeiling = ceiling;
    return ceiling;
}

double Deco::GetGFPoint(double depth) const{
    double GFHigh = this->GFHigh;
    double GFLow = this->GFLow;
    double LowDepth = this->FirstStopDepth;

    if (FirstStopDepth == -1) {
        return GFLow;
    }
    else {
        if(depth < LowDepth) {
            double GF = GFHigh + ((GFHigh - GFLow) / (0.0 - BarToMeter(LowDepth))) * (BarToMeter(depth));
            return GF;
        }
        else if (depth <= 1)
        {
            return GFHigh;
        }
        else {
            return GFLow;
        }
    }
}

double Deco::GetNoDecoTime() const{
    double noStopTime = 0;
    bool inLimits = true;
    while (inLimits) {
        auto* DecoSim = new Deco(*this);
        DecoSim->AddBottom(noStopTime);
        inLimits = DecoSim->GetCeiling() < 1;
        noStopTime++;
        delete DecoSim;
        if(noStopTime > 999){return 999;}
    }
    noStopTime -= 1;
    return noStopTime;
}

Deco::DecoStop Deco::GetNextDecoStop() {
    return GetNextDecoStop(0.0);
}

Deco::DecoStop Deco::GetNextDecoStop(double startTime) {
    // Round deco depth to next multiple of 3m (return as bar)
    double StopDepth = MeterToBar(ceil(BarToMeter(this->GetCeiling()) / 3) * 3);
    double StopTime = startTime;
    bool inLimits = false;
    int gas;
    while (!inLimits) {
        StopTime += 1;
        auto* decoSim = new Deco(*this);
        gas = BestGas(StopDepth, decoPPO2);
        decoSim->SwitchGas(gas);
        decoSim->AddDecent(StopDepth, MeterToBar(Deco::AccentRate));
        decoSim->AddBottom(StopTime);
        decoSim->GetCeiling();
        inLimits = decoSim->GetCeiling() < StopDepth-(MeterToBar(3)-1.0);
        delete decoSim;
    }
    return {StopDepth, StopTime ,gas};
}

std::vector<Deco::DecoStop> Deco::GetDecoSchedule(){
    std::vector<Deco::DecoStop> Schedule;
    this->FirstStopDepth = GetCeiling();
    auto* decoSim = new Deco(*this);
    while (decoSim->GetCeiling() > 1.031) {
        Deco::DecoStop stop;
        if (!Schedule.empty() && Schedule.back().Gas == BestGas(Schedule.back().Depth-3, decoPPO2) )
        {
            stop = decoSim->GetNextDecoStop(Schedule.back().Time);
        }
        else
        {
            stop = decoSim->GetNextDecoStop();
        }
        Schedule.emplace_back(stop);
        decoSim->SwitchGas(stop.Gas);
        decoSim->AddDecent(stop.Depth, MeterToBar(Deco::AccentRate));
        decoSim->AddBottom(stop.Time);
    }
    delete decoSim;
    return Schedule;
}
