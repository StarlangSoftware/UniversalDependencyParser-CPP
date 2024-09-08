//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#include "ArcEagerInstanceGenerator.h"
#include "Attribute/DiscreteIndexedAttribute.h"

/**
 * Checks if the given word has a valid relation.
 * @param word The UniversalDependencyTreeBankWord to check.
 * @return true if the relation is valid, false otherwise.
 */
bool ArcEagerInstanceGenerator::suitable(UniversalDependencyTreeBankWord *word) {
    return word->getRelation() != nullptr;
}

/**
 * Generates an Instance object based on the provided state, window size, and command.
 * The Instance is populated with attributes derived from the words in the state.
 * @param state The state used to generate the instance.
 * @param windowSize The size of the window used to extract words from the state.
 * @param command The command associated with the instance.
 * @return The generated Instance object.
 */
Instance *ArcEagerInstanceGenerator::generate(const State &state, int windowSize, const string &command) {
    auto* instance = new Instance(command);
    vector<Attribute*> attributes;
    for (int i = 0; i < windowSize; i++) {
        UniversalDependencyTreeBankWord* word = state.getStackWord(i);
        if (word == nullptr) {
            attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, 18));
            addEmptyAttributes(attributes);
            attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, 59));
        } else {
            if (word->getName() == "root") {
                attributes.emplace_back(new DiscreteIndexedAttribute("root", 0, 18));
                addEmptyAttributes(attributes);
                attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, 59));
            } else {
                attributes.emplace_back(new DiscreteIndexedAttribute(UniversalDependencyTreeBankFeatures::getPosString(word->getUpos()), UniversalDependencyTreeBankFeatures::posIndex(UniversalDependencyTreeBankFeatures::getPosString(word->getUpos())) + 1, 18));
                addFeatureAttributes(word, attributes);
                if (suitable(word)) {
                    attributes.emplace_back(new DiscreteIndexedAttribute(word->getRelation()->to_string(), UniversalDependencyTreeBankFeatures::dependencyIndex(word->getRelation()->to_string()) + 1, 59));
                } else {
                    attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, 59));
                }
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
