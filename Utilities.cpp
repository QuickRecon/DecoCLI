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

#include "Utilities.h"

double BarToMeter(double bar) {
    return 10*(bar-1);
}

double MeterToBar(double meter) {
    return 0.1*meter+1;
}

template<typename Out>

void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

void ShowLicense(){
    std::string License =
            "Program for calculating decompression stops and no stop times using buehlmann and buehlmann derived algorithms\n\n"
            "Copyright (C) 2017 Aren Leishman\n"
            "\n"
            "This program is free software; you can redistribute it and/or modify\n"
            "it under the terms of the GNU General Public License as published by\n"
            "the Free Software Foundation; either version 2 of the License, or\n"
            "(at your option) any later version.\n"
            "\n"
            "This program is distributed in the hope that it will be useful,\n"
            "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
            "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
            "GNU General Public License for more details.\n"
            "\n"
            "You should have received a copy of the GNU General Public License\n"
            "along with this program.  If not, see <http://www.gnu.org/licenses/>.";

    std::cout << License << std::endl << std::endl;
}

void ShowLimitedLicense(){
    std::string License =
            "DecoCLI Copyright (C) 2017 Aren Leishman\n\n"
            "This program comes with ABSOLUTELY NO WARRANTY.\n"
            "This is free software, and you are welcome to redistribute it\n"
            "under certain conditions; use the --ShowLicense option for details";
    std::cout << License << std::endl << std::endl;
}

void ShowUsage(const std::string executable){
    std::string Usage =
            "This program is designed to perform (diving) decompression calculations.\n"
            "\n"
            "General use: " + executable + "<General Parameters> <Model Parameters>"
            "General Parameters:\n"
            "--ShowLicense  :   Show the extended license prompt\n"
            "--HideLicense  :   Hide the automatic license prompt\n"
            "--quiet        :   Suppress unnecessary outputs\n"
            "\n"
            "Dive Parameters:\n"
            "G\n"
            "   Adds a gas to the gas list, takes the form of G<FrN2>:<FrO2>:<FrHe>,\n"
            "   air (expressed as G0.21:0.79:0) is automatically added as the zeroth element\n"
            "   and gases can be accessed in the order they are added.\n"
            "D\n"
            "   Add depths and times to the model, takes the form D<Depth1>,<Time1>:<Depth2>,<Time2>...\n"
            "   For example a dive that goes to 10 meters for 10 minutes then goes to 60 meters for 5 minutes\n"
            "   would be expressed as D10,10:60,5";
    std::cout << Usage << std::endl;
}