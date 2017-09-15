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

#include <iostream>
#include "GFDeco.h"
#include "BMDeco.h"

int main(int argc, char* argv[]) {
    if(argc == 2){
        if(strcmp(argv[1],"ShowLicense") == 0){
            ShowLicense();
            return 0;
        }
    }

    ShowLimitedLicense();
    GFDeco test;
    //depths are in bar, times in min
    test.AddDecent(MeterToBar(60), MeterToBar(test.DecentRate));
    test.AddBottom(30);
    std::cout << "Ceiling:" << BarToMeter(test.GetCeiling()) << std::endl;
    std::vector<GFDeco::DecoStop> Schedule = test.GetDecoSchedule();
    for (int i = 0; i < Schedule.size(); i++) {
        std::cout << "Deco Depth(" << i << "): " << BarToMeter(Schedule[i].Depth) << std::endl;
        std::cout << "Deco time(" << i << "): " << Schedule[i].Time << std::endl;
        std::cout << std::endl;
    }
    return 0;
}