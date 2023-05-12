//
// Created by Olcay Taner YILDIZ on 10.05.2023.
//

#ifndef UNIVERSALDEPENDENCYPARSER_STACKRELATION_H
#define UNIVERSALDEPENDENCYPARSER_STACKRELATION_H

#include "Universal/UniversalDependencyTreeBankWord.h"

class StackRelation {
private:
    UniversalDependencyTreeBankWord* word;
    UniversalDependencyRelation* relation;
public:
    StackRelation(UniversalDependencyTreeBankWord* word, UniversalDependencyRelation* relation);
    StackRelation(StackRelation& copy);
    UniversalDependencyTreeBankWord* getWord() const;
    UniversalDependencyRelation* getRelation() const;
};


#endif //UNIVERSALDEPENDENCYPARSER_STACKRELATION_H
