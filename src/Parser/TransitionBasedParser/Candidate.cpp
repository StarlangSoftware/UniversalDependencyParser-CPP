//
// Created by Olcay Taner YILDIZ on 10.05.2023.
//

#include "Candidate.h"

Candidate::Candidate(Command command, UniversalDependencyType universalDependencyType) {
    this->command = command;
    this->universalDependencyType = universalDependencyType;
}

Command Candidate::getCommand() const{
    return command;
}

UniversalDependencyType Candidate::getUniversalDependencyType() const {
    return universalDependencyType;
}
