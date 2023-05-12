//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#include "RandomScoringOracle.h"
#include <random>

double RandomScoringOracle::score(State state) {
    return random() / RAND_MAX;
}
