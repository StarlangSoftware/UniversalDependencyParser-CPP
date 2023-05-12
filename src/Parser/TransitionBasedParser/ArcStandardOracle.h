//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#ifndef UNIVERSALDEPENDENCYPARSER_ARCSTANDARDORACLE_H
#define UNIVERSALDEPENDENCYPARSER_ARCSTANDARDORACLE_H


#include "Oracle.h"

class ArcStandardOracle : Oracle{
public:
    ArcStandardOracle(const Model& model, int windowSize);
    Decision* makeDecision(const State& state) override;
    vector<Decision> scoreDecisions(const State& state, TransitionSystem transitionSystem) override;
};


#endif //UNIVERSALDEPENDENCYPARSER_ARCSTANDARDORACLE_H
