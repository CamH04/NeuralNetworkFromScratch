#ifndef __BACK_PROP__H
#define __BACK_PROP__H

#include <vector>
#include <cmath>
#include <layer.h>

using std::vector;

class back_prop{
    public:
        void CalculateGradients(std::vector<layer> Layers , std::vector<double> targets);
        void UpdateParameters(std::vector<layer> Layers, double LearnRate);
};
#endif


