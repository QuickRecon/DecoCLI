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

double BarToMeter(double bar) {
    return 10*(bar-1);
}

double MeterToBar(double meter) {
    return 0.1*meter+1;
}

void ShowLicense(){
    std::string License =
            "    Program for calculating decompression stops and no stop times using buehlmann and buehlmann derived algorithms\n\n"
            "    Copyright (C) 2017 Aren Leishman\n"
            "\n"
            "    This program is free software; you can redistribute it and/or modify\n"
            "    it under the terms of the GNU General Public License as published by\n"
            "    the Free Software Foundation; either version 2 of the License, or\n"
            "    (at your option) any later version.\n"
            "\n"
            "    This program is distributed in the hope that it will be useful,\n"
            "    but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
            "    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
            "    GNU General Public License for more details.\n"
            "\n"
            "    You should have received a copy of the GNU General Public License along\n"
            "    with this program; if not, write to the Free Software Foundation, Inc.,\n"
            "    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.";

    std::cout << License << std::endl << std::endl;
}

void ShowLimitedLicense(){
    std::string License =
            "    DecoCLI  Copyright (C) 2017 Aren Leishman\n"
            "    This program comes with ABSOLUTELY NO WARRANTY.\n"
            "    This is free software, and you are welcome to redistribute it\n"
            "    under certain conditions; use the ShowLicense option for details";
    std::cout << License << std::endl << std::endl;
}

void ShowUsage(){
    std::string Usage =
            "This program is designed to perform (diving) decompression calculations.\n"
            "\n"
            "General Parameters:\n"
            "--ShowLicense  :   Show the extended license prompt\n"
            "--HideLicense  :   Hide the automatic license prompt\n";
    std::cout << Usage << std::endl;
}