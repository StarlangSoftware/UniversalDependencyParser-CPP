//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#include "Oracle.h"

Oracle::Oracle(const Model &model, int windowSize) {
    this->commandModel = model;
    this->windowSize = windowSize;
}

string Oracle::findBestValidEagerClassInfo(const map<string, double> &probabilities, const State &state) {
    double bestValue = 0.0;
    string best;
    for (auto & iterator : probabilities) {
        if (iterator.second > bestValue) {
            if (iterator.first == "SHIFT" || iterator.first == "RIGHTARC") {
                if (state.wordListSize() > 0) {
                    best = iterator.first;
                    bestValue = iterator.second;
                }
            } else if (state.stackSize() > 1) {
                if (!(iterator.first == "REDUCE" && state.getPeek()->getRelation() == nullptr)) {
                    best = iterator.first;
                    bestValue = iterator.second;
                }
            }
        }
    }
    return best;
}

string Oracle::findBestValidStandardClassInfo(const map<string, double> &probabilities, const State &state) {
    double bestValue = 0.0;
    string best;
    for (auto & iterator : probabilities) {
        if (iterator.second > bestValue) {
            if (iterator.first == "SHIFT") {
                if (state.wordListSize() > 0) {
                    best = iterator.first;
                    bestValue = iterator.second;
                }
            } else if (state.stackSize() > 1) {
                best = iterator.first;
                bestValue = iterator.second;
            }
        }
    }
    return best;
}

Candidate* Oracle::getDecisionCandidate(const string &best) {
    string command, relation;
    UniversalDependencyType type;
    if (best.find('(') != string::npos){
        command = best.substr(0, best.find('('));
        relation = best.substr(best.find('(') + 1, best.find(')') - best.find('(') - 1);
        type = UniversalDependencyRelation::getDependencyTag(relation);
    } else {
        command = best;
        type = UniversalDependencyType::DEP;
    }
    if (command == "SHIFT") {
        return new Candidate(Command::SHIFT, type);
    }  else {
        if (command == "REDUCE") {
            return new Candidate(Command::REDUCE, type);
        } else {
            if (command == "LEFTARC") {
                return new Candidate(Command::LEFTARC, type);
            } else {
                if (command == "RIGHTARC") {
                    return new Candidate(Command::RIGHTARC, type);
                } else {
                    return nullptr;
                }
            }
        }
    }
}
