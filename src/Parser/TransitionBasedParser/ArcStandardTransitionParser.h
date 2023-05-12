//
// Created by Olcay Taner YILDIZ on 12.05.2023.
//

#ifndef UNIVERSALDEPENDENCYPARSER_ARCSTANDARDTRANSITIONPARSER_H
#define UNIVERSALDEPENDENCYPARSER_ARCSTANDARDTRANSITIONPARSER_H


#include "TransitionParser.h"

class ArcStandardTransitionParser : public TransitionParser{
private:
    bool checkForMoreRelation(const vector<StackWord*>& wordList, int id);
public:
    ArcStandardTransitionParser();
    vector<Instance*> simulateParse(UniversalDependencyTreeBankSentence* sentence, int windowSize) override;
    UniversalDependencyTreeBankSentence* dependencyParse(UniversalDependencyTreeBankSentence* universalDependencyTreeBankSentence, Oracle* oracle) override;
};


#endif //UNIVERSALDEPENDENCYPARSER_ARCSTANDARDTRANSITIONPARSER_H
