//
// Created by aren on 9/14/17.
//

#ifndef DECOCLI_BMDECO_H
#define DECOCLI_BMDECO_H

#include "Deco.h"

class BMDeco: public Deco{
public:
    /// Functions
    explicit BMDeco();
    BMDeco(const BMDeco& deco);

    double GetCeiling();
    int GetNoDecoTime();
    BMDeco::DecoStop GetNextDecoStop();
    std::vector<DecoStop> GetDecoSchedule();
};


#endif //DECOCLI_BMDECO_H
