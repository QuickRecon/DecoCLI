#include <iostream>
#include "Deco.h"

int main() {
    Deco test(0);
    //depths are in bar, times in min
    test.AddDecent(Deco::MeterToBar(30),Deco::MeterToBar(test.DecentRate));
    test.AddBottom(50);
    std::vector<Deco::DecoStop> Schedule  = test.GetDecoSchedule();
    for(int i = 0; i < Schedule.size(); i++){
        std::cout << "Deco Depth(" << i << "): " << Deco::BarToMeter(Schedule[i].Depth) << std::endl;
        std::cout << "Deco time(" << i << "): " << Schedule[i].Time << std::endl;
        std::cout << std::endl;
    }
    return 0;
}