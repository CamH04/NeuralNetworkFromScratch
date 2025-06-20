#ifndef __NEURALNETWORK__H
#define __NEURALNETWORK__H

#include <vector>
#include <layer.h>
#include <neuron.h>

class neural_network{
    public:
        vector<layer> Layers;
        neural_network();
        neural_network(const std::vector<int>& layersData);

};

#endif


