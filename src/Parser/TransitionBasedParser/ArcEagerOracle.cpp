//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#include "ArcEagerOracle.h"
#include "InstanceGenerator.h"
#include "SimpleInstanceGenerator.h"

ArcEagerOracle::ArcEagerOracle(const Model &model, int windowSize) : Oracle(model, windowSize) {
}

Decision *ArcEagerOracle::makeDecision(const State &state) {
    string best;
    InstanceGenerator* instanceGenerator = new SimpleInstanceGenerator();
    Instance* instance = instanceGenerator->generate(state, this->windowSize, "");
    best = findBestValidEagerClassInfo(commandModel.predictProbability(instance), state);
    Candidate* decisionCandidate = getDecisionCandidate(best);
    if (decisionCandidate->getCommand() == Command::SHIFT) {
        return new Decision(Command::SHIFT, UniversalDependencyType::DEP, 0.0);
    } else if (decisionCandidate->getCommand() == Command::REDUCE) {
        return new Decision(Command::REDUCE, UniversalDependencyType::DEP, 0.0);
    }
    return new Decision(decisionCandidate->getCommand(), decisionCandidate->getUniversalDependencyType(), 0.0);
}

vector<Decision> ArcEagerOracle::scoreDecisions(const State &state, TransitionSystem transitionSystem) {
    return vector<Decision>();
}
