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

int main(int argc, char* argv[]) {

    GFDeco DecoActual; //initialise Deco Model

    for(int i = 0; i < argc; i++){
        std::string argument(argv[i]);
        std::string FirstLetter = argument.substr(0,1);
        if(argument == "--ShowLicense"){
            ShowLicense();
            return 0;
        } else if(argument ==  "--HideLicense"){
            AutoShowLicense = false;
        } else if( FirstLetter == "G"){
            std::string substring = argument.substr(1,argument.size());
            std::vector<float> parameters = StringSplit(substring, ':');
            DecoActual.AddGas(parameters[0], parameters[1], parameters[3]);
        } else if(argument == "--quite") {
            verbose = false;
            AutoShowLicense = false;
            return 0;
        }
    }
    if(AutoShowLicense){ShowLimitedLicense();};
    if(argc >= 3){

        //depths are in bar, times in min
        DecoActual.AddDecent(MeterToBar(60), MeterToBar(DecoActual.DecentRate));
        DecoActual.AddBottom(30);
        std::cout << "Ceiling:" << BarToMeter(DecoActual.GetCeiling()) << std::endl;
        std::vector<GFDeco::DecoStop> Schedule = DecoActual.GetDecoSchedule();
        for (int i = 0; i < Schedule.size(); i++) {
            std::cout << "Deco Depth(" << i << "): " << BarToMeter(Schedule[i].Depth) << std::endl;
            std::cout << "Deco time(" << i << "): " << Schedule[i].Time << std::endl;
            std::cout << std::endl;
        }
        return 0;
    }

    ShowUsage(argv[0]);

    return 0;
}