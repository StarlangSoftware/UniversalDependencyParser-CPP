//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#ifndef UNIVERSALDEPENDENCYPARSER_RANDOMSCORINGORACLE_H
#define UNIVERSALDEPENDENCYPARSER_RANDOMSCORINGORACLE_H


#include "ScoringOracle.h"

class RandomScoringOracle : public ScoringOracle{
public:
    double score(State state);
};


#endif //UNIVERSALDEPENDENCYPARSER_RANDOMSCORINGORACLE_H
