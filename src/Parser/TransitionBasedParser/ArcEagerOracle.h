//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#ifndef UNIVERSALDEPENDENCYPARSER_ARCEAGERORACLE_H
#define UNIVERSALDEPENDENCYPARSER_ARCEAGERORACLE_H


#include "Oracle.h"

class ArcEagerOracle : public Oracle{
public:
    ArcEagerOracle(const Model& model, int windowSize);
    Decision* makeDecision(const State& state) override;
    vector<Decision> scoreDecisions(const State& state, TransitionSystem transitionSystem) override;
};


#endif //UNIVERSALDEPENDENCYPARSER_ARCEAGERORACLE_H
