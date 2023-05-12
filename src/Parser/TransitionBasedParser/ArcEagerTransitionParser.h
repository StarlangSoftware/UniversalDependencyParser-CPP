//
// Created by Olcay Taner YILDIZ on 12.05.2023.
//

#ifndef UNIVERSALDEPENDENCYPARSER_ARCEAGERTRANSITIONPARSER_H
#define UNIVERSALDEPENDENCYPARSER_ARCEAGERTRANSITIONPARSER_H


#include "TransitionParser.h"

class ArcEagerTransitionParser : public TransitionParser{
public:
    ArcEagerTransitionParser();
    vector<Instance*> simulateParse(UniversalDependencyTreeBankSentence* sentence, int windowSize);
    UniversalDependencyTreeBankSentence* dependencyParse(UniversalDependencyTreeBankSentence* universalDependencyTreeBankSentence, Oracle* oracle);
};


#endif //UNIVERSALDEPENDENCYPARSER_ARCEAGERTRANSITIONPARSER_H
