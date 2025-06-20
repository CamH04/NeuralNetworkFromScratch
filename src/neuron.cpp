#include <neuron.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>

using std::sqrt;
using std::log;
using std::cos;
using std::vector;
using std::mt19937;
using std::uniform_real_distribution;

neuron::neuron() { //deafult constructer
}
neuron::neuron(int previousLayerNeuronsNumber) {
    if (previousLayerNeuronsNumber > 0) {
        Weights.resize(previousLayerNeuronsNumber);

        for (int i = 0; i < previousLayerNeuronsNumber; ++i) {
            Weights[i] = InitializeRandom() / sqrt(previousLayerNeuronsNumber);
        }
        Bias = InitializeRandom()/ sqrt(previousLayerNeuronsNumber);
    }
}

double neuron::InitializeRandom() {
    static thread_local std::mt19937 generator(std::random_device{}());
    static thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double x1 = 1.0 - distribution(generator);
    double x2 = 1.0 - distribution(generator);

    return sqrt(-2.0 * log(x1)) * cos(2.0 * M_PI * x2);
}
