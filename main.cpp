#include <iostream>
#include "GFDeco.h"

int main() {
    GFDeco test(0);
    //depths are in bar, times in min
    test.AddDecent(MeterToBar(30),MeterToBar(test.DecentRate));
    test.AddBottom(50);
    std::cout << "Ceiling:" << BarToMeter(test.GetCeiling()) << std::endl;
    std::vector<GFDeco::DecoStop> Schedule  = test.GetDecoSchedule();
    for(int i = 0; i < Schedule.size(); i++){
        std::cout << "Deco Depth(" << i << "): " << BarToMeter(Schedule[i].Depth) << std::endl;
        std::cout << "Deco time(" << i << "): " << Schedule[i].Time << std::endl;
        std::cout << std::endl;
    }
    return 0;
}