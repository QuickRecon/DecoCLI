#include <iostream>
#include "Deco.h"

int main() {
    Deco test(0);
    //depths are in bar, times in min
    test.AddDecent(Deco::MeterToBar(30),Deco::MeterToBar(test.DecentRate));
    std::cout << "Nst:" << test.GetNoDecoTime() << std::endl;
    test.AddBottom(50);

    std::cout << "Deco Depth: " << Deco::BarToMeter(test.GetNextDecoStop().Depth) << std::endl;
    std::cout << "Deco time: " << test.GetNextDecoStop().Time << std::endl;
    return 0;
}