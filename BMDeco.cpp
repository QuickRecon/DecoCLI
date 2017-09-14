//
// Created by aren on 9/14/17.
//

#include "BMDeco.h"

double BMDeco::GetCeiling() {
    this->LimitingTissueIndex = 0;
    for(int i = 0; i < 16; i++){
        double Pn = this->Pn[i];
        double aN2 = BMDeco::buehlmann_N2_a[i];
        double bN2 = BMDeco::buehlmann_N2_b[i];

        double Ph = this->Ph[i];
        double aHe = BMDeco::buehlmann_He_a[i];
        double bHe = BMDeco::buehlmann_He_b[i];

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
int BMDeco::GetNoDecoTime() {
    int noStopTime = 0;
    bool inLimits = true;
    while(inLimits){
        BMDeco decoSim = BMDeco(*this);
        decoSim.AddBottom(noStopTime);
        inLimits = decoSim.GetCeiling() < 1;
        noStopTime++;
    }
    noStopTime -= 1;
    return noStopTime;
}

BMDeco::DecoStop BMDeco::GetNextDecoStop() {
    // Round deco depth to next multiple of 3m (return as bar)
    double StopDepth = MeterToBar(ceil(BarToMeter(BMDeco::GetCeiling())/3)*3);

    int StopTime = 0;
    bool inLimits = false;
    while(!inLimits){
        BMDeco decoSim = BMDeco(*this);
        decoSim.AddDecent(StopDepth, -MeterToBar(BMDeco::AccentRate));
        decoSim.AddBottom(StopTime);
        inLimits = decoSim.GetCeiling() < StopDepth - 0.3;
        StopTime++;
    }
    return {StopDepth, StopTime};
}

std::vector<BMDeco::DecoStop> BMDeco::GetDecoSchedule() {
    std::vector<BMDeco::DecoStop> Schedule;
    BMDeco decoSim = BMDeco(*this);
    while(decoSim.GetCeiling() > 1){
        BMDeco::DecoStop stop = decoSim.GetNextDecoStop();
        Schedule.emplace_back(stop);
        decoSim.AddDecent(stop.Depth, -MeterToBar(BMDeco::AccentRate));
        decoSim.AddBottom(stop.Time);
    }
    return Schedule;
}

BMDeco::BMDeco() {
    this->Depth = 1; //1 bar at atmospheric pressure
    this->AccentCeiling = -1000;

    /// Configure default gas (Air)
    gases.emplace_back(BMDeco::gas(0.79, 0.21, 0));

    SetPartialPressures(1);
    /// Create gas compartments
    for(int i=0; i < 16; i++){
        this->TissueAccentCeiling[i] = -1000;
        this->SetGasLoadings(this->ppN2, 0, i);
    }
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

    for(int i = 0; i < 16; i++){
        this->TissueAccentCeiling[i] = deco.TissueAccentCeiling[i];
        this->Pn[i] = deco.Pn[i];
        this->Ph[i] = deco.Ph[i];
        this->Pt[i] = deco.Pt[i];
    }
}

