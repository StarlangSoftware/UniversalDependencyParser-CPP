//
// Created by Olcay Taner YILDIZ on 11.05.2023.
//

#ifndef UNIVERSALDEPENDENCYPARSER_INSTANCEGENERATOR_H
#define UNIVERSALDEPENDENCYPARSER_INSTANCEGENERATOR_H


#include <Instance/Instance.h>
#include "State.h"

class InstanceGenerator {
private:
    void addAttributeForFeatureType(UniversalDependencyTreeBankWord* word, vector<Attribute*>& attributes, const string& featureType);
protected:
    void addEmptyAttributes(vector<Attribute*>& attributes);
    void addFeatureAttributes(UniversalDependencyTreeBankWord* word, vector<Attribute*>& attributes);
public:
    virtual Instance* generate(const State& state, int windowSize, const string& command) = 0;
};


#endif //UNIVERSALDEPENDENCYPARSER_INSTANCEGENERATOR_H
