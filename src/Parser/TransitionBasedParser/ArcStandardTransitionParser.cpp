//
// Created by Olcay Taner YILDIZ on 12.05.2023.
//

#include "ArcStandardTransitionParser.h"
#include "InstanceGenerator.h"
#include "SimpleInstanceGenerator.h"

ArcStandardTransitionParser::ArcStandardTransitionParser() : TransitionParser(){

}

/**
 * Checks if there are more relations with a specified ID in the list of words.
 * @param wordList The list of words to check.
 * @param id The ID to check for.
 * @return True if no more relations with the specified ID are found; false otherwise.
 */
bool ArcStandardTransitionParser::checkForMoreRelation(const vector<StackWord *> &wordList, int id) {
    for (StackWord* word : wordList) {
        if (word->getWord()->getRelation()->to() == id) {
            return false;
        }
    }
    return true;
}

/**
 * Simulates the parsing process for a given sentence using the Arc Standard parsing algorithm.
 * @param sentence The sentence to be parsed.
 * @param windowSize The size of the window used for feature generation.
 * @return An ArrayList of {@link Instance} objects representing the parsed actions.
 */
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

/**
 * Performs dependency parsing on the given sentence using the provided oracle.
 * @param universalDependencyTreeBankSentence The sentence to be parsed.
 * @param oracle The oracle used to make parsing decisions.
 * @return The parsed sentence with dependency relations established.
 */
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
