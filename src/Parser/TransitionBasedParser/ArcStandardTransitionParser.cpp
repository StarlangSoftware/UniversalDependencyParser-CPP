//
// Created by Olcay Taner YILDIZ on 12.05.2023.
//

#include "ArcStandardTransitionParser.h"
#include "InstanceGenerator.h"
#include "SimpleInstanceGenerator.h"

ArcStandardTransitionParser::ArcStandardTransitionParser() : TransitionParser(){

}

bool ArcStandardTransitionParser::checkForMoreRelation(const vector<StackWord *> &wordList, int id) {
    for (StackWord* word : wordList) {
        if (word->getWord()->getRelation()->to() == id) {
            return false;
        }
    }
    return true;
}

vector<Instance *>
ArcStandardTransitionParser::simulateParse(UniversalDependencyTreeBankSentence *sentence, int windowSize) {
    UniversalDependencyTreeBankWord *top, *beforeTop;
    UniversalDependencyRelation *topRelation, *beforeTopRelation;
    InstanceGenerator* instanceGenerator = new SimpleInstanceGenerator();
    vector<Instance*> instanceList;
    vector<StackWord*> wordList;
    vector<StackWord*> stack;
    for (int j = 0; j < sentence->wordCount(); j++) {
        wordList.emplace_back(new StackWord((UniversalDependencyTreeBankWord*) sentence->getWord(j), j + 1));
    }
    stack.emplace_back(new StackWord());
    State state = State(stack, wordList, vector<StackRelation*>());
    if (!wordList.empty()) {
        instanceList.emplace_back(instanceGenerator->generate(state, windowSize, "SHIFT"));
        moveFromWordListToStack(wordList, stack);
        if (wordList.size() > 1) {
            instanceList.emplace_back(instanceGenerator->generate(state, windowSize, "SHIFT"));
            moveFromWordListToStack(wordList, stack);
        }
        while (!wordList.empty() || stack.size() > 1) {
            top = stack[stack.size() - 1]->getWord();
            topRelation = top->getRelation();
            if (stack.size() > 1) {
                beforeTop = stack[stack.size() - 2]->getWord();
                beforeTopRelation = beforeTop->getRelation();
                if (beforeTop->getId() == topRelation->to() && checkForMoreRelation(wordList, top->getId())) {
                    instanceList.emplace_back(instanceGenerator->generate(state, windowSize, "RIGHTARC(" + topRelation->to_string() + ")"));
                    stack.pop_back();
                } else if (top->getId() == beforeTopRelation->to()) {
                    instanceList.emplace_back(instanceGenerator->generate(state, windowSize, "LEFTARC(" + beforeTopRelation->to_string() + ")"));
                    stack.erase(stack.end() - 2);
                } else {
                    if (!wordList.empty()) {
                        instanceList.emplace_back(instanceGenerator->generate(state, windowSize, "SHIFT"));
                        moveFromWordListToStack(wordList, stack);
                    } else {
                        break;
                    }
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
    }
    return instanceList;
}

UniversalDependencyTreeBankSentence *
ArcStandardTransitionParser::dependencyParse(UniversalDependencyTreeBankSentence *universalDependencyTreeBankSentence,
                                             Oracle* oracle) {
    UniversalDependencyTreeBankSentence* sentence = createResultSentence(universalDependencyTreeBankSentence);
    State state = initialState(sentence);
    while (state.wordListSize() > 0 || state.stackSize() > 1) {
        Decision* decision = oracle->makeDecision(state);
        switch (decision->getCommand()) {
            case Command::SHIFT:
                state.applyShift();
                break;
            case Command::LEFTARC:
                state.applyLeftArc(decision->getUniversalDependencyType());
                break;
            case Command::RIGHTARC:
                state.applyRightArc(decision->getUniversalDependencyType());
                break;
            default:
                break;
        }
    }
    return sentence;
}
