//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#include "InstanceGenerator.h"
#include "Attribute/DiscreteIndexedAttribute.h"

void
InstanceGenerator::addAttributeForFeatureType(UniversalDependencyTreeBankWord* word, vector<Attribute*>& attributes,
                                              const string &featureType) {
    string feature = word->getFeatureValue(featureType);
    int numberOfValues = UniversalDependencyTreeBankFeatures::numberOfValues("tr", featureType) + 1;
    if (!feature.empty()) {
        attributes.emplace_back(new DiscreteIndexedAttribute(feature, UniversalDependencyTreeBankFeatures::featureValueIndex("tr", featureType, feature) + 1, numberOfValues));
    } else {
        attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, numberOfValues));
    }
}

void InstanceGenerator::addEmptyAttributes(vector<Attribute *>& attributes) {
    attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, UniversalDependencyTreeBankFeatures::numberOfValues("tr", "PronType") + 1));
    attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, UniversalDependencyTreeBankFeatures::numberOfValues("tr", "NumType") + 1));
    attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, UniversalDependencyTreeBankFeatures::numberOfValues("tr", "Number") + 1));
    attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, UniversalDependencyTreeBankFeatures::numberOfValues("tr", "Case") + 1));
    attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, UniversalDependencyTreeBankFeatures::numberOfValues("tr", "Definite") + 1));
    attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, UniversalDependencyTreeBankFeatures::numberOfValues("tr", "Degree") + 1));
    attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, UniversalDependencyTreeBankFeatures::numberOfValues("tr", "VerbForm") + 1));
    attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, UniversalDependencyTreeBankFeatures::numberOfValues("tr", "Mood") + 1));
    attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, UniversalDependencyTreeBankFeatures::numberOfValues("tr", "Tense") + 1));
    attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, UniversalDependencyTreeBankFeatures::numberOfValues("tr", "Aspect") + 1));
    attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, UniversalDependencyTreeBankFeatures::numberOfValues("tr", "Voice") + 1));
    attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, UniversalDependencyTreeBankFeatures::numberOfValues("tr", "Evident") + 1));
    attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, UniversalDependencyTreeBankFeatures::numberOfValues("tr", "Polarity") + 1));
    attributes.emplace_back(new DiscreteIndexedAttribute("null", 0, UniversalDependencyTreeBankFeatures::numberOfValues("tr", "Person") + 1));
}

void InstanceGenerator::addFeatureAttributes(UniversalDependencyTreeBankWord* word, vector<Attribute*>& attributes) {
    addAttributeForFeatureType(word, attributes, "PronType");
    addAttributeForFeatureType(word, attributes, "NumType");
    addAttributeForFeatureType(word, attributes, "Number");
    addAttributeForFeatureType(word, attributes, "Case");
    addAttributeForFeatureType(word, attributes, "Definite");
    addAttributeForFeatureType(word, attributes, "Degree");
    addAttributeForFeatureType(word, attributes, "VerbForm");
    addAttributeForFeatureType(word, attributes, "Mood");
    addAttributeForFeatureType(word, attributes, "Tense");
    addAttributeForFeatureType(word, attributes, "Aspect");
    addAttributeForFeatureType(word, attributes, "Voice");
    addAttributeForFeatureType(word, attributes, "Evident");
    addAttributeForFeatureType(word, attributes, "Polarity");
    addAttributeForFeatureType(word, attributes, "Person");
}
