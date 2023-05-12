//
// Created by Olcay Taner YILDIZ on 12.05.2023.
//

#ifndef UNIVERSALDEPENDENCYPARSER_TRANSITIONPARSER_H
#define UNIVERSALDEPENDENCYPARSER_TRANSITIONPARSER_H

#include <Universal/UniversalDependencyTreeBankSentence.h>
#include <Universal/UniversalDependencyTreeBankWord.h>
#include <Universal/UniversalDependencyTreeBankCorpus.h>
#include <DataSet/DataSet.h>
#include "Oracle.h"
#include "Agenda.h"

class TransitionParser {
private:
    bool checkStates(const Agenda& agenda) const;
    vector<Candidate*> constructCandidates(TransitionSystem transitionSystem, const State& state);
protected:
    UniversalDependencyTreeBankSentence* createResultSentence(UniversalDependencyTreeBankSentence* universalDependencyTreeBankSentence);
    State initialState(UniversalDependencyTreeBankSentence* sentence);
    void moveFromWordListToStack(vector<StackWord*>& wordList, vector<StackWord*>& stack);
public:
    TransitionParser();
    DataSet simulateParseOnCorpus(const UniversalDependencyTreeBankCorpus& corpus, int windowSize);
    virtual vector<Instance*> simulateParse(UniversalDependencyTreeBankSentence* sentence, int windowSize) = 0;
    virtual UniversalDependencyTreeBankSentence* dependencyParse(UniversalDependencyTreeBankSentence* universalDependencyTreeBankSentence, Oracle* oracle) = 0;
    State dependencyParseWithBeamSearch(ScoringOracle* oracle, int beamSize, UniversalDependencyTreeBankSentence* universalDependencyTreeBankSentence, TransitionSystem transitionSystem);
    UniversalDependencyTreeBankCorpus dependencyParseCorpus(const UniversalDependencyTreeBankCorpus& universalDependencyTreeBankCorpus, Oracle* oracle);
};


#endif //UNIVERSALDEPENDENCYPARSER_TRANSITIONPARSER_H
