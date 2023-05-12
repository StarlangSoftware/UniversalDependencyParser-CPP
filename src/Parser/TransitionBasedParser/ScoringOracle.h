//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#ifndef UNIVERSALDEPENDENCYPARSER_SCORINGORACLE_H
#define UNIVERSALDEPENDENCYPARSER_SCORINGORACLE_H


#include "State.h"

class ScoringOracle {
public:
    virtual double score(State state) = 0;
};


#endif //UNIVERSALDEPENDENCYPARSER_SCORINGORACLE_H
