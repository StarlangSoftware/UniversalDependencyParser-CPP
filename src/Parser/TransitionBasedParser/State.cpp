//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#include "State.h"

State::State(const vector<StackWord*> &stack, const vector<StackWord*> &wordList,
             const vector<StackRelation*>& relations) {
    this->stack = stack;
    this->wordList = wordList;
    this->relations = relations;
}

State::State()= default;

void State::applyShift() {
    if (!wordList.empty()) {
        StackWord* word = wordList.at(0);
        stack.emplace_back(word);
        wordList.erase(wordList.begin());
    }
}

void State::applyLeftArc(UniversalDependencyType type) {
    if (stack.size() > 1) {
        UniversalDependencyTreeBankWord* beforeLast = stack[stack.size() - 2]->getWord();
        int index = stack[stack.size() - 1]->getToWord();
        string typeString = Word::replaceAll(UniversalDependencyRelation::universalDependencyTypes[static_cast<int>(type)], "_", ":");
        beforeLast->setRelation(new UniversalDependencyRelation(index, typeString));
        stack.erase(stack.end() - 2);
        relations.emplace_back(new StackRelation(beforeLast, new UniversalDependencyRelation(index, typeString)));
    }
}

void State::applyRightArc(UniversalDependencyType type) {
    if (stack.size() > 1) {
        UniversalDependencyTreeBankWord* last = stack[stack.size() - 1]->getWord();
        int index = stack[stack.size() - 2]->getToWord();
        last->setRelation(new UniversalDependencyRelation(index, typeString(type)));
        stack.pop_back();
        relations.emplace_back(new StackRelation(last, new UniversalDependencyRelation(index, typeString(type))));
    }
}

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
    return Word::replaceAll(UniversalDependencyRelation::universalDependencyTypes[static_cast<int>(type)], "_", ":");
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

void State::applyReduce() {
    if (!stack.empty()) {
        stack.pop_back();
    }
}

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

int State::relationSize() const{
    return relations.size();
}

int State::wordListSize() const{
    return wordList.size();
}

int State::stackSize() const{
    return stack.size();
}

UniversalDependencyTreeBankWord *State::getStackWord(int index) const{
    int size = stack.size() - 1;
    if (size - index < 0) {
        return nullptr;
    }
    return stack[size - index]->getWord();
}

UniversalDependencyTreeBankWord *State::getPeek() const{
    if (!stack.empty()) {
        return stack[stack.size() - 1]->getWord();
    }
    return nullptr;
}

UniversalDependencyTreeBankWord *State::getWordListWord(int index) const{
    if (index > wordList.size() - 1) {
        return nullptr;
    }
    return wordList[index]->getWord();
}

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
