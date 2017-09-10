#include <iostream>
#include "Deco.h"

int main() {
    Deco test(0);
    //depths are in bar, times in minuets
    test.AddDecent(Deco::MeterToBar(30),Deco::MeterToBar(18));
    //test.AddBottom(10);
    std::cout << "Ceiling:" << Deco::BarToMeter(test.GetCeiling());
    return 0;
}