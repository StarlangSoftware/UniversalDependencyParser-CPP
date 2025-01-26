//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#include "State.h"
#include "StringUtils.h"

/**
 * Constructs a State object with given stack, wordList, and relations.
 *
 * @param stack     The stack of words in the parser state.
 * @param wordList  The list of words to be processed.
 * @param relations The relations established between words.
 */
State::State(const vector<StackWord*> &stack, const vector<StackWord*> &wordList,
             const vector<StackRelation*>& relations) {
    this->stack = stack;
    this->wordList = wordList;
    this->relations = relations;
}

State::State()= default;

/**
 * Applies the SHIFT operation to the parser state.
 * Moves the first word from the wordList to the stack.
 */
void State::applyShift() {
    if (!wordList.empty()) {
        StackWord* word = wordList.at(0);
        stack.emplace_back(word);
        wordList.erase(wordList.begin());
    }
}

/**
 * Applies the LEFTARC operation to the parser state.
 * Creates a relation from the second-to-top stack element to the top stack element
 * and then removes the second-to-top element from the stack.
 * @param type The type of the dependency relation.
 */
void State::applyLeftArc(UniversalDependencyType type) {
    if (stack.size() > 1) {
        UniversalDependencyTreeBankWord* beforeLast = stack[stack.size() - 2]->getWord();
        int index = stack[stack.size() - 1]->getToWord();
        string typeString = StringUtils::replaceAll(UniversalDependencyRelation::universalDependencyTypes[static_cast<int>(type)], "_", ":");
        beforeLast->setRelation(new UniversalDependencyRelation(index, typeString));
        stack.erase(stack.end() - 2);
        relations.emplace_back(new StackRelation(beforeLast, new UniversalDependencyRelation(index, typeString)));
    }
}

/**
 * Applies the RIGHTARC operation to the parser state.
 * Creates a relation from the top stack element to the second-to-top stack element
 * and then removes the top element from the stack.
 *
 * @param type The type of the dependency relation.
 */
void State::applyRightArc(UniversalDependencyType type) {
    if (stack.size() > 1) {
        UniversalDependencyTreeBankWord* last = stack[stack.size() - 1]->getWord();
        int index = stack[stack.size() - 2]->getToWord();
        last->setRelation(new UniversalDependencyRelation(index, typeString(type)));
        stack.pop_back();
        relations.emplace_back(new StackRelation(last, new UniversalDependencyRelation(index, typeString(type))));
    }
}

/**
 * Applies the ARC_EAGER_LEFTARC operation to the parser state.
 * Creates a relation from the last element of the stack to the first element of the wordList
 * and then removes the top element from the stack.
 *
 * @param type The type of the dependency relation.
 */
void State::applyArcEagerLeftArc(UniversalDependencyType type) {
    if (!stack.empty() && !wordList.empty()) {
        UniversalDependencyTreeBankWord* lastElementOfStack = stack[stack.size() - 1]->getWord();
        int index = wordList[0]->getToWord();
        lastElementOfStack->setRelation(new UniversalDependencyRelation(index, typeString(type)));
        stack.pop_back();
        relations.emplace_back(new StackRelation(lastElementOfStack, new UniversalDependencyRelation(index, typeString(type))));
    }
}

string State::typeString(UniversalDependencyType type) {
    return StringUtils::replaceAll(UniversalDependencyRelation::universalDependencyTypes[static_cast<int>(type)], "_", ":");
}

void State::applyArcEagerRightArc(UniversalDependencyType type) {
    if (!stack.empty() && !wordList.empty()) {
        UniversalDependencyTreeBankWord* firstElementOfWordList = wordList[0]->getWord();
        int index = stack[stack.size() - 1]->getToWord();
        firstElementOfWordList->setRelation(new UniversalDependencyRelation(index, typeString(type)));
        applyShift();
        relations.emplace_back(new StackRelation(firstElementOfWordList, new UniversalDependencyRelation(index,
                                                                                                         typeString(type))));
    }
}

/**
 * Applies the REDUCE operation to the parser state.
 * Removes the top element from the stack.
 */
void State::applyReduce() {
    if (!stack.empty()) {
        stack.pop_back();
    }
}

/**
 * Applies a specific command based on the transition system.
 *
 * @param command The command to be applied (e.g., SHIFT, LEFTARC, RIGHTARC, REDUCE).
 * @param type The type of dependency relation, relevant for ARC operations.
 * @param transitionSystem The transition system (e.g., ARC_STANDARD, ARC_EAGER) that determines which command to apply.
 */
void State::apply(Command command, UniversalDependencyType type, TransitionSystem transitionSystem) {
    switch (transitionSystem) {
        case TransitionSystem::ARC_STANDARD:
            switch (command) {
                case Command::LEFTARC:
                    applyLeftArc(type);
                    break;
                case Command::RIGHTARC:
                    applyRightArc(type);
                    break;
                case Command::SHIFT:
                    applyShift();
                    break;
                default:
                    break;
            }
            break;
        case TransitionSystem::ARC_EAGER:
            switch (command) {
                case Command::LEFTARC:
                    applyArcEagerLeftArc(type);
                    break;
                case Command::RIGHTARC:
                    applyArcEagerRightArc(type);
                    break;
                case Command::SHIFT:
                    applyShift();
                    break;
                case Command::REDUCE:
                    applyReduce();
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

/**
 * Returns the number of relations established in the current state.
 * @return The size of the relations list.
 */
int State::relationSize() const{
    return relations.size();
}

/**
 * Returns the number of words remaining in the wordList.
 * @return The size of the wordList.
 */
int State::wordListSize() const{
    return wordList.size();
}

/**
 * Returns the number of words currently in the stack.
 * @return The size of the stack.
 */
int State::stackSize() const{
    return stack.size();
}

/**
 * Retrieves a specific word from the stack based on its position.
 * @param index The position of the word in the stack.
 * @return The word at the specified position, or null if the index is out of bounds.
 */
UniversalDependencyTreeBankWord *State::getStackWord(int index) const{
    int size = stack.size() - 1;
    if (size - index < 0) {
        return nullptr;
    }
    return stack[size - index]->getWord();
}

/**
 * Retrieves the top word from the stack.
 * @return The top word of the stack, or null if the stack is empty.
 */
UniversalDependencyTreeBankWord *State::getPeek() const{
    if (!stack.empty()) {
        return stack[stack.size() - 1]->getWord();
    }
    return nullptr;
}

/**
 * Retrieves a specific word from the wordList based on its position.
 * @param index The position of the word in the wordList.
 * @return The word at the specified position, or null if the index is out of bounds.
 */
UniversalDependencyTreeBankWord *State::getWordListWord(int index) const{
    if (index > wordList.size() - 1) {
        return nullptr;
    }
    return wordList[index]->getWord();
}

/**
 * Retrieves a specific relation based on its index.
 * @param index The position of the relation in the relations list.
 * @return The relation at the specified position, or null if the index is out of bounds.
 */
StackRelation *State::getRelation(int index) const{
    if (index < relations.size()) {
        return relations[index];
    }
    return nullptr;
}

State::State(const State& copy) {
    for (StackWord* element : stack) {
        if (element->getWord()->getName() != "root") {
            stack.emplace_back(new StackWord(*element));
        } else {
            stack.emplace_back(new StackWord(new UniversalDependencyTreeBankWord(), element->getToWord()));
        }
    }
    for (StackWord* word : wordList) {
        wordList.emplace_back(new StackWord(*word));
    }
    for (StackRelation* relation : relations) {
        if (relation->getWord()->getName() != "root") {
            relations.emplace_back(new StackRelation(*relation));
        } else {
            relations.emplace_back(new StackRelation(new UniversalDependencyTreeBankWord(), relation->getRelation()));
        }
    }
}
