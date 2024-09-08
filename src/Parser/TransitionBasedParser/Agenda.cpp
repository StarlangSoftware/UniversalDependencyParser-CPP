//
// Created by Olcay Taner YILDIZ on 12.05.2023.
//

#include "Agenda.h"
#include <vector>

Agenda::Agenda(int beamSize) {
    this->beamSize = beamSize;
}

/**
 * Updates the agenda with a new state if it is better than the worst state
 * currently in the agenda or if there is room in the agenda.
 * @param oracle The ScoringOracle used to score the state.
 * @param current The state to be added to the agenda.
 */
void Agenda::updateAgenda(ScoringOracle* oracle, const State& current) {
    if (agenda.contains(current)) {
        return;
    }
    double point = oracle->score(current);
    if (agenda.size() < beamSize) {
        agenda.emplace(current, point);
    } else {
        State worst;
        double worstValue = INT_MAX;
        for (auto &iterator: agenda) {
            if (iterator.second < worstValue) {
                worstValue = iterator.second;
                worst = iterator.first;
            }
        }
        if (point > worstValue) {
            agenda.erase(worst);
            agenda.emplace(current, point);
        }
    }
}

/**
 * Retrieves the best state from the agenda based on the highest score.
 * @return The state with the highest score in the agenda.
 */
State Agenda::best() {
    State best;
    double bestValue = INT_MAX;
    for (auto &iterator: agenda) {
        if (iterator.second > bestValue) {
            bestValue = iterator.second;
            best = iterator.first;
        }
    }
    return best;
}

