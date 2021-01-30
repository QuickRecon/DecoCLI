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

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <iterator>

#ifndef DECOCLI_UTILITIES_H
#define DECOCLI_UTILITIES_H

double WaterDensity = 9.7694411879640484;

double BarToMeter(double bar);

double MeterToBar(double meter);

void ShowLicense();

void ShowLimitedLicense();

void ShowUsage(std::string executable);

std::vector<std::string> split(const std::string &s, char delimiter);

#endif //DECOCLI_UTILITIES_H
