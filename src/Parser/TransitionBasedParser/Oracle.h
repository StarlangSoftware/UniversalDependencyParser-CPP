//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#ifndef UNIVERSALDEPENDENCYPARSER_ORACLE_H
#define UNIVERSALDEPENDENCYPARSER_ORACLE_H

#include "Model/Model.h"
#include "Decision.h"
#include "State.h"

class Oracle {
protected:
    Model commandModel;
    int windowSize;
    string findBestValidEagerClassInfo(const map<string, double>& probabilities, const State& state);
    string findBestValidStandardClassInfo(const map<string, double>& probabilities, const State& state);
    Candidate* getDecisionCandidate(const string& best);
public:
    Oracle(const Model& model, int windowSize);
    virtual Decision * makeDecision(const State& state) = 0;
    virtual vector<Decision> scoreDecisions(const State& state, TransitionSystem transitionSystem) = 0;
};


#endif //UNIVERSALDEPENDENCYPARSER_ORACLE_H
