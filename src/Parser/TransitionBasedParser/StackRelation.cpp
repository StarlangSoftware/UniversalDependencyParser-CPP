//
// Created by Olcay Taner YILDIZ on 10.05.2023.
//

#include "StackRelation.h"

StackRelation::StackRelation(UniversalDependencyTreeBankWord* word, UniversalDependencyRelation* relation) {
    this->word = word;
    this->relation = relation;
}

StackRelation::StackRelation(StackRelation &copy) {
    this->word = new UniversalDependencyTreeBankWord(*copy.word);
    this->relation = copy.relation;
}

UniversalDependencyTreeBankWord *StackRelation::getWord() const {
    return word;
}

UniversalDependencyRelation *StackRelation::getRelation() const {
    return relation;
}
