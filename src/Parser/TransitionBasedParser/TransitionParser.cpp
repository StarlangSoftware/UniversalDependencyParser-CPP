//
// Created by Olcay Taner YILDIZ on 12.05.2023.
//

#include "TransitionParser.h"

TransitionParser::TransitionParser() = default;

/**
 * Creates a new {@link UniversalDependencyTreeBankSentence} with the same words as the input sentence,
 * but with null heads, effectively cloning the sentence structure without dependencies.
 * @param universalDependencyTreeBankSentence the sentence to be cloned
 * @return a new {@link UniversalDependencyTreeBankSentence} with copied words but no dependencies
 */
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

/**
 * Simulates parsing a corpus of sentences, returning a dataset of instances created by parsing each sentence.
 * @param corpus the corpus to be parsed
 * @param windowSize the size of the window used in parsing
 * @return a {@link DataSet} containing instances from parsing each sentence in the corpus
 */
DataSet TransitionParser::simulateParseOnCorpus(const UniversalDependencyTreeBankCorpus& corpus, int windowSize) {
    DataSet dataSet = DataSet();
    for (int i = 0; i < corpus.sentenceCount(); i++) {
        dataSet.addInstanceList(simulateParse((UniversalDependencyTreeBankSentence*) corpus.getSentence(i), windowSize));
    }
    return dataSet;
}

/**
 * Checks if there are any states in the agenda that still have words to process or have more than one item in the stack.
 * @param agenda the agenda containing the states
 * @return true if there are states to process, false otherwise
 */
bool TransitionParser::checkStates(const Agenda& agenda) const{
    for (auto& iterator : agenda.agenda) {
        if (iterator.first.wordListSize() > 0 || iterator.first.stackSize() > 1) {
            return true;
        }
    }
    return false;
}

/**
 * Initializes the parsing state with a stack containing one empty {@link StackWord} and a word list containing all words in the sentence.
 * @param sentence the sentence to initialize the state with
 * @return a {@link State} representing the starting point for parsing
 */
State TransitionParser::initialState(UniversalDependencyTreeBankSentence *sentence) {
    vector<StackWord*> wordList;
    for (int i = 0; i < sentence->wordCount(); i++) {
        wordList.emplace_back(new StackWord((UniversalDependencyTreeBankWord*) sentence->getWord(i), i + 1));
    }
    vector<StackWord*> stack;
    stack.emplace_back(new StackWord());
    return State(stack, wordList, vector<StackRelation*>());
}

/**
 * Constructs possible parsing candidates based on the current state and transition system.
 * @param transitionSystem the transition system used (ARC_STANDARD or ARC_EAGER)
 * @param state the current parsing state
 * @return a list of possible {@link Candidate} actions to be applied
 */
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

/**
 * Performs dependency parsing with beam search to find the best parse for a given sentence.
 * @param oracle the scoring oracle used for guiding the search
 * @param beamSize the size of the beam for beam search
 * @param universalDependencyTreeBankSentence the sentence to be parsed
 * @param transitionSystem the transition system used (ARC_STANDARD or ARC_EAGER)
 * @return the best parsing state from the beam search
 */
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

/**
 * Parses a corpus of sentences using the given oracle and returns a new corpus with the parsed sentences.
 * @param universalDependencyTreeBankCorpus the corpus to be parsed
 * @param oracle the oracle used for guiding the parsing process
 * @return a {@link UniversalDependencyTreeBankCorpus} containing the parsed sentences
 */
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
