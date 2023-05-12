//
// Created by Olcay Taner YILDIZ on 10.05.2023.
//

#include "Decision.h"

Decision::Decision(Command command, UniversalDependencyType universalDependencyType, double point) : Candidate(command, universalDependencyType) {
    this->point = point;
}

double Decision::getPoint() const{
    return point;
}
