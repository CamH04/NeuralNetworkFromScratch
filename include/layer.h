#ifndef __LAYER__H
#define __LAYER__H

#include <vector>
#include <neuron.h>

using std::vector;

//Layers : input - hidden - output

class layer{
    public:
        vector<neuron> Neurons;
};

#endif

