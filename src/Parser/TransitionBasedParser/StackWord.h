//
// Created by Olcay Taner YILDIZ on 10.05.2023.
//

#ifndef UNIVERSALDEPENDENCYPARSER_STACKWORD_H
#define UNIVERSALDEPENDENCYPARSER_STACKWORD_H


#include <Universal/UniversalDependencyTreeBankWord.h>

class StackWord {
private:
    UniversalDependencyTreeBankWord* word;
    int toWord;
public:
    StackWord();
    StackWord(StackWord& copy);
    StackWord(UniversalDependencyTreeBankWord* word, int toWord);
    UniversalDependencyTreeBankWord* getWord() const;
    int getToWord() const;
};


#endif //UNIVERSALDEPENDENCYPARSER_STACKWORD_H
