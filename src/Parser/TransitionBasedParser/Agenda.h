//
// Created by Olcay Taner YILDIZ on 12.05.2023.
//

#ifndef UNIVERSALDEPENDENCYPARSER_AGENDA_H
#define UNIVERSALDEPENDENCYPARSER_AGENDA_H


#include "State.h"
#include "ScoringOracle.h"
#include <map>
#include <unordered_set>

class Agenda {
private:
    int beamSize;
public:
    map<State, double> agenda;
    Agenda(int beamSize);
    void updateAgenda(ScoringOracle* oracle, const State& current);
    State best();
};


#endif //UNIVERSALDEPENDENCYPARSER_AGENDA_H
