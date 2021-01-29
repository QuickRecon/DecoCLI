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


#ifndef DECOCLI_GFDECO_H
#define DECOCLI_GFDECO_H

#include "Deco.h"

class GFDeco : public Deco {
public:
    /// Functions
    GFDeco();
    ~GFDeco();

    GFDeco(const GFDeco &deco);

    double GetCeiling() override;

    double GetNoDecoTime() override;

    Deco::DecoStop GetNextDecoStop();

    std::vector<DecoStop> GetDecoSchedule() override;

    double GetGFPoint(double depth);

    double FirstStopDepth;
};

#endif //DECOCLI_GFDECO_H
