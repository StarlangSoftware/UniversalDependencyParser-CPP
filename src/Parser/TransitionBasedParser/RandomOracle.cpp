//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#include "RandomOracle.h"

RandomOracle::RandomOracle(const Model& model, int windowSize) : Oracle(model, windowSize){
}

Decision *RandomOracle::makeDecision(const State &state) {
    int command = random() % 3;
    int relation = random() % 59;
    switch (command) {
        case 0:
            return new Decision(Command::LEFTARC, UniversalDependencyRelation::universalDependencyTags[relation], 0);
        case 1:
            return new Decision(Command::RIGHTARC, UniversalDependencyRelation::universalDependencyTags[relation], 0);
        case 2:
            return new Decision(Command::SHIFT, UniversalDependencyType::DEP, 0);
    }
    return nullptr;
}

vector<Decision> RandomOracle::scoreDecisions(const State &state, TransitionSystem transitionSystem) {
    return vector<Decision>();
}
