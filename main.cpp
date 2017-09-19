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

//
#include <iostream>
#include "GFDeco.h"
#include "BMDeco.h"
#include <cstring>

bool AutoShowLicense = true;
bool verbose = true;

int main(int argc, char *argv[]) {

    Deco* DecoActual;

    if(argc == 1){
        ShowUsage(argv[0]);
        return 0;
    }
    std::string mode(argv[1]);
    if(mode == "GF"){
        DecoActual = new GFDeco;
    } else if(mode == "BM"){
        DecoActual = new BMDeco;
    } else {
        DecoActual = new GFDeco;
        std::cout << "No mode given, defaulting to GF mode. Run with --help for details." << std::endl;
    }

    for (int i = 2; i < argc; i++) {
        std::string argument(argv[i]);
        std::string FirstLetter = argument.substr(0, 1);
        if (argument == "--ShowLicense") {
            ShowLicense();
            return 0;
        } else if (argument == "--HideLicense") {
            AutoShowLicense = false;
        } else if (FirstLetter == "G") {
            std::string substring = argument.substr(1, argument.size());
            std::vector<std::string> parameters = split(substring, ':');
            double FrO2 = stod(parameters[0]);
            double FrN2 = stod(parameters[1]);
            double FrHe = stod(parameters[2]);
            DecoActual->AddGas(FrO2,FrN2,FrHe);
        } else if (argument == "--quite") {
            verbose = false;
            AutoShowLicense = false;
            return 0;
        } else if (FirstLetter == "D"){
            std::string substring = argument.substr(1, argument.size());
            std::vector<std::string> parameters = split(substring, ':');
            for (const auto &parameter : parameters) {
                std::vector<std::string> depths = split(parameter,',');
                double depth = stod(depths[0]);
                double time = stod(depths[1]);
                DecoActual->AddDecent(MeterToBar(depth), MeterToBar(DecoActual->DecentRate));
                DecoActual->AddBottom(time);
            }
        } else if (argument == "--help"){
            ShowUsage(argv[0]);
        }
    }
    if (AutoShowLicense) { ShowLimitedLicense(); };
    if (argc >= 3) {
        if(BarToMeter(DecoActual->GetCeiling()) > 0){
            std::vector<GFDeco::DecoStop> Schedule = DecoActual->GetDecoSchedule();
            for (int i = 0; i < Schedule.size(); i++) {
                std::cout << "Deco Depth(" << i << "): " << BarToMeter(Schedule[i].Depth) << std::endl;
                std::cout << "Deco time(" << i << "): " << Schedule[i].Time << std::endl;
                std::cout << std::endl;
            }
        } else {
            std::cout << "Remaining No Stop Time: " << DecoActual->GetNoDecoTime();
        }
        return 0;
    }

    ShowUsage(argv[0]);

    return 0;
}