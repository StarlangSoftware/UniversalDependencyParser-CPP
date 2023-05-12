//
// Created by Olcay Taner YILDIZ on 12.05.2023.
//

#include "TransitionParser.h"

TransitionParser::TransitionParser() = default;

UniversalDependencyTreeBankSentence *
TransitionParser::createResultSentence(UniversalDependencyTreeBankSentence *universalDependencyTreeBankSentence) {
    auto* sentence = new UniversalDependencyTreeBankSentence();
    for (int i = 0; i < universalDependencyTreeBankSentence->wordCount(); i++) {
        auto* word = (UniversalDependencyTreeBankWord*) universalDependencyTreeBankSentence->getWord(i);
        sentence->addWord(new UniversalDependencyTreeBankWord(word->getId(),
                                                              word->getName(),
                                                              word->getLemma(),
                                                              word->getUpos(),
                                                              word->getXpos(),
                                                              word->getFeatures(),
                                                              nullptr,
                                                              word->getDeps(),
                                                              word->getMisc()));
    }
    return sentence;
}

DataSet TransitionParser::simulateParseOnCorpus(const UniversalDependencyTreeBankCorpus& corpus, int windowSize) {
    DataSet dataSet = DataSet();
    for (int i = 0; i < corpus.sentenceCount(); i++) {
        dataSet.addInstanceList(simulateParse((UniversalDependencyTreeBankSentence*) corpus.getSentence(i), windowSize));
    }
    return dataSet;
}

bool TransitionParser::checkStates(const Agenda& agenda) const{
    for (auto& iterator : agenda.agenda) {
        if (iterator.first.wordListSize() > 0 || iterator.first.stackSize() > 1) {
            return true;
        }
    }
    return false;
}

State TransitionParser::initialState(UniversalDependencyTreeBankSentence *sentence) {
    vector<StackWord*> wordList;
    for (int i = 0; i < sentence->wordCount(); i++) {
        wordList.emplace_back(new StackWord((UniversalDependencyTreeBankWord*) sentence->getWord(i), i + 1));
    }
    vector<StackWord*> stack;
    stack.emplace_back(new StackWord());
    return State(stack, wordList, vector<StackRelation*>());
}

vector<Candidate *> TransitionParser::constructCandidates(TransitionSystem transitionSystem, const State &state) {
    if (state.stackSize() == 1 && state.wordListSize() == 0) {
        return vector<Candidate*>();
    }
    vector<Candidate*> subsets;
    if (state.wordListSize() > 0) {
        subsets.emplace_back(new Candidate(Command::SHIFT, UniversalDependencyType::DEP));
    }
    if (transitionSystem == TransitionSystem::ARC_EAGER && state.stackSize() > 0) {
        subsets.emplace_back(new Candidate(Command::REDUCE, UniversalDependencyType::DEP));
    }
    for (const auto & universalDependencyType : UniversalDependencyRelation::universalDependencyTypes) {
        UniversalDependencyType type = UniversalDependencyRelation::getDependencyTag(universalDependencyType);
        if (transitionSystem == TransitionSystem::ARC_STANDARD && state.stackSize() > 1) {
            subsets.emplace_back(new Candidate(Command::LEFTARC, type));
            subsets.emplace_back(new Candidate(Command::RIGHTARC, type));
        } else if (transitionSystem == TransitionSystem::ARC_EAGER && state.stackSize() > 0 && state.wordListSize() > 0) {
            subsets.emplace_back(new Candidate(Command::LEFTARC, type));
            subsets.emplace_back(new Candidate(Command::RIGHTARC, type));
        }
    }
    return subsets;
}

State TransitionParser::dependencyParseWithBeamSearch(ScoringOracle* oracle, int beamSize,
                                                      UniversalDependencyTreeBankSentence *universalDependencyTreeBankSentence,
                                                      TransitionSystem transitionSystem) {
    UniversalDependencyTreeBankSentence* sentence = createResultSentence(universalDependencyTreeBankSentence);
    State initial = initialState(sentence);
    Agenda agenda = Agenda(beamSize);
    agenda.updateAgenda(oracle, State(initial));
    while (checkStates(agenda)) {
        for (const auto& iterator : agenda.agenda) {
            vector<Candidate*> subsets = constructCandidates(transitionSystem, iterator.first);
            for (Candidate* subset : subsets) {
                Command command = subset->getCommand();
                UniversalDependencyType type = subset->getUniversalDependencyType();
                State cloneState = State(iterator.first);
                cloneState.apply(command, type, transitionSystem);
                agenda.updateAgenda(oracle, State(cloneState));
            }
        }
    }
    return agenda.best();
}

UniversalDependencyTreeBankCorpus
TransitionParser::dependencyParseCorpus(const UniversalDependencyTreeBankCorpus &universalDependencyTreeBankCorpus,
                                        Oracle* oracle) {
    UniversalDependencyTreeBankCorpus corpus = UniversalDependencyTreeBankCorpus();
    for (int i = 0; i < universalDependencyTreeBankCorpus.sentenceCount(); i++) {
        auto* sentence = (UniversalDependencyTreeBankSentence*) universalDependencyTreeBankCorpus.getSentence(i);
        corpus.addSentence(dependencyParse(sentence, oracle));
    }
    return corpus;
}

void TransitionParser::moveFromWordListToStack(vector<StackWord *> &wordList, vector<StackWord *> &stack) {
    StackWord* word = wordList[0];
    wordList.erase(wordList.begin());
    stack.emplace_back(word);
}
