//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#ifndef UNIVERSALDEPENDENCYPARSER_STATE_H
#define UNIVERSALDEPENDENCYPARSER_STATE_H


#include "StackWord.h"
#include "StackRelation.h"
#include "Command.h"
#include "TransitionSystem.h"
#include <vector>

class State {
private:
    vector<StackWord*> stack;
    vector<StackWord*> wordList;
    vector<StackRelation*> relations;
    string typeString(UniversalDependencyType type);
public:
    State(const vector<StackWord*>& stack, const vector<StackWord*>& wordList, const vector<StackRelation*>& relations);
    State(const State& copy);
    State();
    void applyShift();
    void applyLeftArc(UniversalDependencyType type);
    void applyRightArc(UniversalDependencyType type);
    void applyArcEagerLeftArc(UniversalDependencyType type);
    void applyArcEagerRightArc(UniversalDependencyType type);
    void applyReduce();
    void apply(Command command, UniversalDependencyType type, TransitionSystem transitionSystem);
    int relationSize() const;
    int wordListSize() const;
    int stackSize() const;
    UniversalDependencyTreeBankWord* getStackWord(int index) const;
    UniversalDependencyTreeBankWord* getPeek() const;
    UniversalDependencyTreeBankWord* getWordListWord(int index) const;
    StackRelation* getRelation(int index) const;
    bool operator==(const State &anotherState) const{
        return (stack == anotherState.stack) && (wordList == anotherState.wordList) && (relations == anotherState.relations);
    }
    bool operator<(const State &anotherState) const {
        if (stack.size() < anotherState.stackSize()){
            return false;
        } else {
            if (stack.size() > anotherState.stackSize()){
                return true;
            } else {
                if (wordList.size() < anotherState.wordListSize()){
                    return false;
                } else {
                    return true;
                }
            }
        }
    }
};


#endif //UNIVERSALDEPENDENCYPARSER_STATE_H
