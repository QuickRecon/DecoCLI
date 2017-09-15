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

int main(int argc, char* argv[]) {
    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i],"--ShowLicense") == 0){
            ShowLicense();
            return 0;
        } else if(strcmp(argv[i], "--HideLicense") == 0){
            AutoShowLicense = false;
        }
    }
    if(AutoShowLicense){ShowLimitedLicense();};
    if(argc >= 3){
        GFDeco DecoActual;
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

    ShowUsage();

    return 0;
}