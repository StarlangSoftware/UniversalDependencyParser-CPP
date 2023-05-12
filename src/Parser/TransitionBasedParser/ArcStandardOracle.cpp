//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#include "ArcStandardOracle.h"
#include "InstanceGenerator.h"
#include "SimpleInstanceGenerator.h"

ArcStandardOracle::ArcStandardOracle(const Model &model, int windowSize) : Oracle(model, windowSize) {

}

Decision *ArcStandardOracle::makeDecision(const State &state) {
    string best;
    InstanceGenerator* instanceGenerator = new SimpleInstanceGenerator();
    Instance* instance = instanceGenerator->generate(state, this->windowSize, "");
    best = findBestValidStandardClassInfo(commandModel.predictProbability(instance), state);
    Candidate* decisionCandidate = getDecisionCandidate(best);
    if (decisionCandidate->getCommand() == Command::SHIFT) {
        return new Decision(Command::SHIFT, UniversalDependencyType::DEP, 0.0);
    }
    return new Decision(decisionCandidate->getCommand(), decisionCandidate->getUniversalDependencyType(), 0.0);
}

vector<Decision> ArcStandardOracle::scoreDecisions(const State &state, TransitionSystem transitionSystem) {
    return vector<Decision>();
}
