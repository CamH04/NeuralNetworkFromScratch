#include "neural_network.h"

neural_network::neural_network() {
}

neural_network::neural_network(const std::vector<int>& layersData){
    int numLayers = layersData.size();
    Layers.reserve(numLayers);
    if (numLayers == 0)
        return;
    Layers.emplace_back(layersData[0], 0); //input layer
    for (int i = 1; i < numLayers; ++i) {
        //hidden and output layer
        Layers.emplace_back(layersData[i], layersData[i - 1]);
    }
}

