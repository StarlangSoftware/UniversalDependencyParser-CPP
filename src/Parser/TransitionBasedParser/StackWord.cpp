//
// Created by Olcay Taner YILDIZ on 10.05.2023.
//

#include "StackWord.h"

StackWord::StackWord() {
    word = new UniversalDependencyTreeBankWord();
    toWord = 0;
}

StackWord::StackWord(StackWord &copy) {
    this->word = new UniversalDependencyTreeBankWord(*copy.word);
    this->toWord = copy.toWord;
}

StackWord::StackWord(UniversalDependencyTreeBankWord *word, int toWord) {
    this->word = word;
    this->toWord = toWord;
}

UniversalDependencyTreeBankWord *StackWord::getWord() const{
    return word;
}

int StackWord::getToWord() const {
    return toWord;
}
