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

//
#include <iostream>
#include "Deco.h"

bool overrideGF = false;

std::vector<std::string> split(const std::string &s, char delimiter);

int main(int argc, char *argv[]) {

    Deco *DecoActual = new Deco();

    if (argc == 1) {
        return 0;
    }
    std::string mode(argv[1]);

    for (int i = 2; i < argc; i++) {
        std::string argument(argv[i]);
        std::string FirstLetter = argument.substr(0, 1);
        if (FirstLetter == "G") {
            std::string substring = argument.substr(1, argument.size());
            std::vector<std::string> parameters = split(substring, ':');
            double FrO2 = stod(parameters[0]);
            double FrN2 = stod(parameters[1]);
            double FrHe = stod(parameters[2]);
            DecoActual->AddGas(FrO2, FrN2, FrHe);
        } else if (argument == "--quite") {
            return 0;
        } else if (FirstLetter == "D") {
            std::string substring = argument.substr(1, argument.size());
            std::vector<std::string> parameters = split(substring, ':');
            for (const auto &parameter : parameters) {
                std::vector<std::string> depths = split(parameter, ',');
                double depth = stod(depths[0]);
                double time = stod(depths[1]);
                DecoActual->AddDecent(MeterToBar(depth), 1);
                DecoActual->AddDecent(MeterToBar(depth), time);
                //DecoActual->AddBottom(time);
            }
        } else if (argument == "--GFLow") {
            if (mode == "GF") {
                std::string GFLow = argv[i + 1];
                DecoActual->GFLow = stod(GFLow);
                overrideGF = true;
            } else {
                std::cout << "Cannot set gradient factors if not in a mode that supports gradient factors" << std::endl;
            }
        } else if (argument == "--GFHigh") {
            if (mode == "GF") {
                std::string GFHigh = argv[i + 1];
                DecoActual->GFHigh = stod(GFHigh);
                overrideGF = true;
            } else {
                std::cout << "Cannot set gradient factors if not in a mode that supports gradient factors" << std::endl;
            }
        }
    }
    if (!overrideGF) { std::cout << "GRADIENT FACTORS NOT SET, USING 30,80 BY DEFAULT.\n" << std::endl; }

    if (argc >= 3) {
        if (BarToMeter(DecoActual->GetCeiling()) > 1) {
            std::vector<Deco::DecoStop> Schedule = DecoActual->GetDecoSchedule(&NoOp);
            for (int i = 0; i < Schedule.size(); i++) {
                std::cout << "Deco Depth(" << i << "): " << BarToMeter(Schedule[i].Depth) << std::endl;
                std::cout << "Deco time(" << i << "): " << Schedule[i].Time << std::endl;
                std::cout << "Deco Gas (" << i << "): " << DecoActual->Gases[Schedule[i].Gas].FrO2 * 100 << "/"
                          << DecoActual->Gases[Schedule[i].Gas].FrHe * 100 << std::endl;
                std::cout << std::endl;
            }
        } else {
            std::cout << "Remaining No Stop Time: " << DecoActual->GetNoDecoTime(NoOp);
        }
        return 0;
    }
    return 0;
}


template<typename Out>

void split(const std::string &s, char delimiter, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> elements;
    split(s, delimiter, std::back_inserter(elements));
    return elements;
}