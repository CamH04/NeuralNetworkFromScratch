/*
 * for all nurones in a layer:
 *  - calculates the weighted sum of outputs from the previous layers neurons:
 *         sum = bias + sigma(previous_output * weight)
 *  - then applies Activate()  to make output to get a non-liner value
 */



#include<frd_prop.h>

double frd_prop::Activate(double x) {
    return 1.0 / (1.0 + std::exp(-x));
}

std::vector<double> frd_prop::ForwardPropagation(std::vector<layer>& layers, const std::vector<double>& inputs) {
    std::vector<double> outputs;
    for (size_t i = 0; i < inputs.size(); ++i) {
        layers[0].Neurons[i].Output = inputs[i];
    }
    for (size_t i = 1; i < layers.size(); ++i) {
        layer& currentLayer = layers[i];
        const layer& previousLayer = layers[i - 1];

        for (size_t j = 0; j < currentLayer.Neurons.size(); ++j) {
            neuron& currentNeuron = currentLayer.Neurons[j];
            double sum = currentNeuron.Bias;

            for (size_t k = 0; k < previousLayer.Neurons.size(); ++k) {
                sum += previousLayer.Neurons[k].Output * currentNeuron.Weights[k];
            }
            currentNeuron.Output = Activate(sum);
        }
    }
    for (const neuron& n : layers.back().Neurons) {
        outputs.push_back(n.Output);
    }
    return outputs;
}
