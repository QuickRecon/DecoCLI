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


#include "GFDeco.h"

double GFDeco::GetCeiling() {
    this->LimitingTissueIndex = 0;
    for (int i = 0; i < 16; i++) {
        double Pn = this->Pn[i];
        double aN2 = GFDeco::buhlmann_N2_a[i];
        double bN2 = GFDeco::buhlmann_N2_b[i];

        double Ph = this->Ph[i];
        double aHe = GFDeco::buhlmann_He_a[i];
        double bHe = GFDeco::buhlmann_He_b[i];

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

double GFDeco::GetGFPoint(double depth) {
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

GFDeco::GFDeco() {
    FirstStopDepth = -1;
}

GFDeco::GFDeco(const GFDeco &GFDeco) {
    FirstStopDepth = GFDeco.FirstStopDepth;
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

double GFDeco::GetNoDecoTime() {
    double noStopTime = 0;
    bool inLimits = true;
    while (inLimits) {
        auto* DecoSim = new GFDeco(*this);
        DecoSim->AddBottom(noStopTime);
        inLimits = DecoSim->GetCeiling() < 1;
        noStopTime++;
        delete DecoSim;
        if(noStopTime > 999){return 999;}
    }
    noStopTime -= 1;
    return noStopTime;
}

GFDeco::DecoStop GFDeco::GetNextDecoStop() {
    // Round deco depth to next multiple of 3m (return as bar)
    double StopDepth = MeterToBar(ceil(BarToMeter(GFDeco::GetCeiling()) / 3) * 3);
    double StopTime = 0;
    bool inLimits = false;
    int gas;
    while (!inLimits) {
        StopTime += 1;
        auto* decoSim = new GFDeco(*this);
        gas = BestGas(StopDepth, 1.62);
        decoSim->SwitchGas(gas);
        decoSim->AddDecent(StopDepth, MeterToBar(GFDeco::AccentRate));
        decoSim->AddBottom(StopTime);
        decoSim->GetCeiling();
        inLimits = decoSim->GetCeiling() < StopDepth-(MeterToBar(3)-1.0);
        delete decoSim;
    }
    return {StopDepth, StopTime ,gas};
}

std::vector<Deco::DecoStop> GFDeco::GetDecoSchedule() {
    std::vector<GFDeco::DecoStop> Schedule;
    this->FirstStopDepth = GetCeiling();
    auto* decoSim = new GFDeco(*this);
    while (decoSim->GetCeiling() > 1.031) {
        GFDeco::DecoStop stop = decoSim->GetNextDecoStop();
        Schedule.emplace_back(stop);
        decoSim->SwitchGas(stop.Gas);
        decoSim->AddDecent(stop.Depth, MeterToBar(GFDeco::AccentRate));
        decoSim->AddBottom(stop.Time);
    }
    delete decoSim;
    return Schedule;
}

GFDeco::~GFDeco() {
    gases.clear();
}

