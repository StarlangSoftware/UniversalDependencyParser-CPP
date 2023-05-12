//
// Created by Olcay Taner YILDIZ on 10.05.2023.
//

#ifndef UNIVERSALDEPENDENCYPARSER_DECISION_H
#define UNIVERSALDEPENDENCYPARSER_DECISION_H


#include "Candidate.h"

class Decision : public Candidate{
private:
    double point;
public:
    Decision(Command command, UniversalDependencyType universalDependencyType, double point);
    double getPoint() const;
};


#endif //UNIVERSALDEPENDENCYPARSER_DECISION_H
