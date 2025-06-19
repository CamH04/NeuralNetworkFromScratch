#include<back_prop.h>


/*
 targets represents target values of network output
*/
void back_prop::CalculateGradients(std::vector<layer> Layers, std::vector<double> targets) {
    int outputLayerIndex = Layers.size() - 1;
    for (size_t i = 0; i < Layers[outputLayerIndex].Neurons.size(); ++i) {
        neuron& n = Layers[outputLayerIndex].Neurons[i];
        double output = n.Output;
        n.Gradient = (output - targets[i]) * (output * (1.0 - output));
    }
    for (int layerIndex = outputLayerIndex - 1; layerIndex >= 1; --layerIndex) {
        layer& currentLayer = Layers[layerIndex];
        layer& nextLayer = Layers[layerIndex + 1];
        for (size_t i = 0; i < currentLayer.Neurons.size(); ++i) {
            neuron& n = currentLayer.Neurons[i];
            double sumGradientTimesWeight = 0.0;
            for (size_t j = 0; j < nextLayer.Neurons.size(); ++j) {
                neuron& nextNeuron = nextLayer.Neurons[j];
                sumGradientTimesWeight += nextNeuron.Weights[i] * nextNeuron.Gradient;
            }
            n.Gradient = sumGradientTimesWeight * (n.Output * (1.0 - n.Output));
        }
    }
}


void back_prop::UpdateParameters(std::vector<layer> Layers, double LearnRate) {
    for (size_t layerIndex = 1; layerIndex < Layers.size(); ++layerIndex) {
        layer& currentLayer = Layers[layerIndex];
        const layer& previousLayer = Layers[layerIndex - 1];
        for (size_t i = 0; i < currentLayer.Neurons.size(); ++i) {
            neuron& n = currentLayer.Neurons[i];
            for (size_t j = 0; j < previousLayer.Neurons.size(); ++j) {
                n.Weights[j] -= LearnRate * n.Gradient * previousLayer.Neurons[j].Output;
            }
            n.Bias -= LearnRate * n.Gradient;
        }
    }
}
