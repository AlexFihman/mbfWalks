#include <iostream>
#include "MonotoneBooleanFunction.h"

MonotoneBooleanFunction::MonotoneBooleanFunction(int dim, std::mt19937& rng) : dimension(dim), min_cuts(rng), weight(0) {
    functionArray = new bool[1 << dim](); // Initialize all values to false
    for (int i=0; i< (1 << dim); i++) functionArray[i] = false;
    updateMinCuts();
}

MonotoneBooleanFunction::~MonotoneBooleanFunction() {
    delete[] functionArray;
}

bool MonotoneBooleanFunction::getFunctionValue(int index) const {
    return functionArray[index];
}

void MonotoneBooleanFunction::setFunctionValue(int index, bool value) {
    functionArray[index] = value;
    updateMinCuts();
}

void MonotoneBooleanFunction::flip(int index) {
    functionArray[index] = !functionArray[index];
    weight += functionArray[index]? 1 : -1;
    updateMinCutsFast(index);
}

bool MonotoneBooleanFunction::checkMinCut(int index) const {
    for (int k=0; k < dimension; k++) {
      int idx2 = index ^ (1 << k);
      //std::cout << "idx2: " << idx2 << "\t" << "value: " << functionArray[index] << std::endl;
      if (idx2 < index && functionArray[idx2]) return false;
      if (idx2 > index && !functionArray[idx2]) return false;
    }
    return true;
}

void MonotoneBooleanFunction::updateMinCuts() {
    for (int i=0; i< (1 << dimension); i++) {
      if (checkMinCut(i)) {
        min_cuts.insert(i);
      } else {
        min_cuts.remove(i);
      }
    }
}

void MonotoneBooleanFunction::updateMinCutsFast(int index) {
    for (int k=0; k < dimension; k++) {
      int idx2 = index ^ (1 << k);   
      if (checkMinCut(idx2)) {
        min_cuts.insert(idx2);
      } else {
        min_cuts.remove(idx2);
      }
    }
}

int MonotoneBooleanFunction::getRandomMinCut() const {
    return min_cuts.getRandomElement();
}

void MonotoneBooleanFunction::printMinCuts() const {
    min_cuts.print();
}

int MonotoneBooleanFunction::getWeight() const {
    return weight;
}

int MonotoneBooleanFunction::minCutSize() const {
    return min_cuts.getSize();
}