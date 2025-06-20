#ifndef __NEURON__H
#define __NEURON__H

#include <vector>

using std::vector;

// Wight : how much influence a neurons output has on the next neuron
// Bias :  shifts the output of a neuron for a better fit for the NN


class neuron{
    public:
        double Bias;
        vector<double> Weights;
        double Output;
        double Gradient;

        double InitializeRandom();

        neuron();
        neuron(int previousLayerNeuronsNumber);

};

#endif
