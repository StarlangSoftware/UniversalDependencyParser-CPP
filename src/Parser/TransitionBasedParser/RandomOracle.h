//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#ifndef UNIVERSALDEPENDENCYPARSER_RANDOMORACLE_H
#define UNIVERSALDEPENDENCYPARSER_RANDOMORACLE_H


#include "Oracle.h"

class RandomOracle : public Oracle{
public:
    RandomOracle(const Model& model, int windowSize);
    Decision* makeDecision(const State& state);
    vector<Decision> scoreDecisions(const State& state, TransitionSystem transitionSystem);
};


#endif //UNIVERSALDEPENDENCYPARSER_RANDOMORACLE_H
