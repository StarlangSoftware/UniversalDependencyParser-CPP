//
// Created by Olcay Taner YILDIZ on 12.05.2023.
//

#include "Agenda.h"
#include <vector>

Agenda::Agenda(int beamSize) {
    this->beamSize = beamSize;
}

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

