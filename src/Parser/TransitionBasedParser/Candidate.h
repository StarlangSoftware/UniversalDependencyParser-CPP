//
// Created by Olcay Taner YILDIZ on 10.05.2023.
//

#ifndef UNIVERSALDEPENDENCYPARSER_CANDIDATE_H
#define UNIVERSALDEPENDENCYPARSER_CANDIDATE_H


#include "Command.h"
#include "Universal/UniversalDependencyType.h"

class Candidate {
private:
    Command command;
    UniversalDependencyType universalDependencyType;
public:
    Candidate(Command command, UniversalDependencyType universalDependencyType);
    Command getCommand() const;
    UniversalDependencyType getUniversalDependencyType() const;
};


#endif //UNIVERSALDEPENDENCYPARSER_CANDIDATE_H
