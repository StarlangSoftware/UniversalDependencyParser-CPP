//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#ifndef UNIVERSALDEPENDENCYPARSER_SIMPLEINSTANCEGENERATOR_H
#define UNIVERSALDEPENDENCYPARSER_SIMPLEINSTANCEGENERATOR_H


#include "InstanceGenerator.h"

class SimpleInstanceGenerator : public InstanceGenerator{
public:
    SimpleInstanceGenerator();
    Instance* generate(const State& state, int windowSize, const string& command) override;
};


#endif //UNIVERSALDEPENDENCYPARSER_SIMPLEINSTANCEGENERATOR_H
