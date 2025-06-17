#ifndef __FRD_PROP__H
#define __FRD_PROP__H

#include <vector>
#include <cmath>
#include <layer.h>
#include <neuron.h>


using std::vector;

class frd_prop{
    public:
        inline double Activate(double x);
        std::vector<double> ForwardPropagation(std::vector<layer>& layers, const std::vector<double>& inputs);
};

#endif

