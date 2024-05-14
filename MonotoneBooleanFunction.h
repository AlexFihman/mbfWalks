#pragma once

#include "ShortList.h"

class MonotoneBooleanFunction {
private:
    int dimension; // Dimension of the boolean function
    int weight;
    bool* functionArray; // Array to store the boolean function
    ShortList min_cuts; // ShortList to store the minimum cuts
    bool checkMinCut(int index) const;
    void updateMinCuts();
    void updateMinCutsFast(int index);

public:
    MonotoneBooleanFunction(int dim, std::mt19937& rng);

    ~MonotoneBooleanFunction();

    bool getFunctionValue(int index) const;

    void setFunctionValue(int index, bool value);

    void flip(int index);

    int getRandomMinCut() const;

    void printMinCuts() const;

    int getWeight() const;

    int minCutSize() const;
};
