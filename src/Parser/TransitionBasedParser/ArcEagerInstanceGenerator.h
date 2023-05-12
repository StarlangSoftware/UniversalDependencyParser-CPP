//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#ifndef UNIVERSALDEPENDENCYPARSER_ARCEAGERINSTANCEGENERATOR_H
#define UNIVERSALDEPENDENCYPARSER_ARCEAGERINSTANCEGENERATOR_H


#include "InstanceGenerator.h"

class ArcEagerInstanceGenerator : public InstanceGenerator{
private:
    bool suitable(UniversalDependencyTreeBankWord* word);
public:
    Instance* generate(const State& state, int windowSize, const string& command);
};


#endif //UNIVERSALDEPENDENCYPARSER_ARCEAGERINSTANCEGENERATOR_H
