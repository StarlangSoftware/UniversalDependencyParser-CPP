//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#include "Oracle.h"

/**
 * Constructs an Oracle with the given model and window size.
 * @param model the model used for making predictions
 * @param windowSize the size of the window used in parsing
 */
Oracle::Oracle(const Model &model, int windowSize) {
    this->commandModel = model;
    this->windowSize = windowSize;
}

/**
 * Finds the best valid parsing action for the ARC_EAGER transition system based on probabilities.
 * Ensures the action is applicable given the current state.
 * @param probabilities a map of actions to their associated probabilities
 * @param state the current parsing state
 * @return the best action as a string, or an empty string if no valid action is found
 */
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

/**
 * Finds the best valid parsing action for the ARC_STANDARD transition system based on probabilities.
 * Ensures the action is applicable given the current state.
 * @param probabilities a map of actions to their associated probabilities
 * @param state the current parsing state
 * @return the best action as a string, or an empty string if no valid action is found
 */
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

/**
 * Converts a string representation of the best action into a {@link Candidate} object.
 * @param best the best action represented as a string, possibly with a dependency type in parentheses
 * @return a {@link Candidate} object representing the action, or null if the action is unknown
 */
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
