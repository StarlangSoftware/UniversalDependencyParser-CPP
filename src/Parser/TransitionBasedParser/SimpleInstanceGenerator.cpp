//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#include "SimpleInstanceGenerator.h"
#include "Attribute/DiscreteIndexedAttribute.h"

SimpleInstanceGenerator::SimpleInstanceGenerator() = default;

/**
 * Generates an instance based on the state, window size, and command.
 * @param state The current state of the parser, which includes the stack and word list.
 * @param windowSize The size of the window used for feature extraction.
 * @param command The command to be associated with the generated instance.
 * @return The generated {@link Instance} object with attributes based on the state and command.
 */
Instance *SimpleInstanceGenerator::generate(const State &state, int windowSize, const string &command) {
    auto* instance = new Instance(command);
    vector<Attribute*> attributes;
    for (int i = 0; i < windowSize; i++) {
        UniversalDependencyTreeBankWord* word = state.getStackWord(i);
        if (word == nullptr) {
            attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, 18));
            addEmptyAttributes(attributes);
        } else {
            if (word->getName() == "root") {
                attributes.emplace_back(new DiscreteIndexedAttribute("root", 0, 18));
                addEmptyAttributes(attributes);
            } else {
                attributes.emplace_back(new DiscreteIndexedAttribute(UniversalDependencyTreeBankFeatures::getPosString(word->getUpos()), UniversalDependencyTreeBankFeatures::posIndex(UniversalDependencyTreeBankFeatures::getPosString(word->getUpos())) + 1, 18));
                addFeatureAttributes(word, attributes);
            }
        }
    }
    for (int i = 0; i < windowSize; i++) {
        UniversalDependencyTreeBankWord* word = state.getWordListWord(i);
        if (word != nullptr) {
            attributes.emplace_back(new DiscreteIndexedAttribute(UniversalDependencyTreeBankFeatures::getPosString(word->getUpos()), UniversalDependencyTreeBankFeatures::posIndex(UniversalDependencyTreeBankFeatures::getPosString(word->getUpos())) + 1, 18));
            addFeatureAttributes(word, attributes);
        } else {
            attributes.emplace_back(new DiscreteIndexedAttribute("root", 0, 18));
            addEmptyAttributes(attributes);
        }
    }
    for (Attribute* attribute : attributes) {
        instance->addAttribute(attribute);
    }
    return instance;
}
