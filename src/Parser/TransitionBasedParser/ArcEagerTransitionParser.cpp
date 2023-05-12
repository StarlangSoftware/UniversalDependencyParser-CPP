//
// Created by Olcay Taner YILDIZ on 12.05.2023.
//

#include "ArcEagerTransitionParser.h"
#include "InstanceGenerator.h"
#include "ArcEagerInstanceGenerator.h"

ArcEagerTransitionParser::ArcEagerTransitionParser() : TransitionParser(){
}

vector<Instance *>
ArcEagerTransitionParser::simulateParse(UniversalDependencyTreeBankSentence *sentence, int windowSize) {
    UniversalDependencyTreeBankWord *top, *first;
    UniversalDependencyRelation *topRelation = nullptr, *firstRelation;
    InstanceGenerator* instanceGenerator = new ArcEagerInstanceGenerator();
    vector<Instance*> instanceList;
    map<int, UniversalDependencyTreeBankWord*> wordMap;
    vector<StackWord*> wordList;
    vector<StackWord*> stack;
    for (int j = 0; j < sentence->wordCount(); j++) {
        auto* word = (UniversalDependencyTreeBankWord*) sentence->getWord(j);
        auto* clone = new UniversalDependencyTreeBankWord(*word);
        clone->setRelation(nullptr);
        wordMap.emplace(j + 1, word);
        wordList.emplace_back(new StackWord(clone, j + 1));
    }
    stack.emplace_back(new StackWord());
    State state = State(stack, wordList, vector<StackRelation*>());
    while (!wordList.empty() || stack.size() > 1) {
        if (!wordList.empty()) {
            first = wordList[0]->getWord();
            firstRelation = wordMap[wordList[0]->getToWord()]->getRelation();
        } else {
            first = nullptr;
            firstRelation = nullptr;
        }
        top = stack[stack.size() - 1]->getWord();
        if (top->getName() != "root") {
            topRelation = wordMap[stack[stack.size() - 1]->getToWord()]->getRelation();
        }
        if (stack.size() > 1) {
            if (firstRelation != nullptr && firstRelation->to() == top->getId()) {
                instanceList.emplace_back(instanceGenerator->generate(state, windowSize, "RIGHTARC(" + firstRelation->to_string() + ")"));
                StackWord* word = wordList[0];
                wordList.erase(wordList.begin());
                stack.emplace_back(new StackWord(wordMap[word->getToWord()], word->getToWord()));
            } else if (first != nullptr && topRelation != nullptr && topRelation->to() == first->getId()) {
                instanceList.emplace_back(instanceGenerator->generate(state, windowSize, "LEFTARC(" + topRelation->to_string() + ")"));
                stack.pop_back();
            } else if (!wordList.empty()) {
                instanceList.emplace_back(instanceGenerator->generate(state, windowSize, "SHIFT"));
                moveFromWordListToStack(wordList, stack);
            } else {
                instanceList.emplace_back(instanceGenerator->generate(state, windowSize, "REDUCE"));
                stack.pop_back();
            }
        } else {
            if (!wordList.empty()) {
                instanceList.emplace_back(instanceGenerator->generate(state, windowSize, "SHIFT"));
                moveFromWordListToStack(wordList, stack);
            } else {
                break;
            }
        }
    }
    return instanceList;
}

UniversalDependencyTreeBankSentence *
ArcEagerTransitionParser::dependencyParse(UniversalDependencyTreeBankSentence *universalDependencyTreeBankSentence,
                                          Oracle *oracle) {
    UniversalDependencyTreeBankSentence* sentence = createResultSentence(universalDependencyTreeBankSentence);
    State state = initialState(sentence);
    while (state.wordListSize() > 0 || state.stackSize() > 1) {
        Decision* decision = oracle->makeDecision(state);
        switch (decision->getCommand()) {
            case Command::SHIFT:
                state.applyShift();
                break;
            case Command::LEFTARC:
                state.applyArcEagerLeftArc(decision->getUniversalDependencyType());
                break;
            case Command::RIGHTARC:
                state.applyArcEagerRightArc(decision->getUniversalDependencyType());
                break;
            case Command::REDUCE:
                state.applyReduce();
                break;
            default:
                break;
        }
    }
    return sentence;
}
