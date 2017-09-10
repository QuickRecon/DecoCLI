#include <iostream>
#include "Deco.h"

int main() {
    Deco test(0);
    //depths are in bar, times in min
    test.AddDecent(Deco::MeterToBar(20),Deco::MeterToBar(30));
    std::cout << "Nst:" << test.GetNoDecoTime() << std::endl;
    return 0;
}