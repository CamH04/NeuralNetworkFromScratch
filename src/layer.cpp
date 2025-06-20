#include <layer.h>

layer::layer() {//deafult constructer
}

layer::layer(int neuronsNumber, int previousLayerNeuronsNumber) {
    Neurons.reserve(neuronsNumber);
    for (int i = 0; i < neuronsNumber; ++i) {
        Neurons.emplace_back(previousLayerNeuronsNumber);
    }
}
