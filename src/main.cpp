#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <thread>

#include <mnist.h>
#include <neural_network.h>
#include <network_state.h>
#include <frd_prop.h>
#include <back_prop.h>

int main() {
    MNIST mnist;
    frd_prop forward;
    back_prop backward;
    NetworkState networkState;

    std::vector<Sample> trainSamples = mnist.LoadImages(
        "training_data/train-images-idx3-ubyte",
        "training_data/train-labels-idx1-ubyte",
        60000
    );

    std::vector<Sample> testSamples = mnist.LoadImages(
        "training_data/t10k-images-idx3-ubyte",
        "training_data/t10k-labels-idx1-ubyte",
        10000
    );

    //input(784), hidden(350), output(10)
    std::vector<int> layersData = {784, 350, 10};
    neural_network neuralNetwork(layersData);

    std::ostringstream layersDataStream;
    for (size_t i = 0; i < neuralNetwork.Layers.size(); ++i) {
        layersDataStream << "[" << neuralNetwork.Layers[i].Neurons.size() << "]";
        if (i != neuralNetwork.Layers.size() -1)
            layersDataStream << "_";
    }
    std::string layersDataStr = layersDataStream.str();

    double bestResult = 0.0;

    for (int epoch = 1; epoch < 100; ++epoch) {
        // training
        for (size_t i = 0; i < trainSamples.size(); ++i) {
            forward.ForwardPropagation(neuralNetwork.Layers, trainSamples[i].Data);
            backward.CalculateGradients(neuralNetwork.Layers, trainSamples[i].Targets);
            backward.UpdateParameters(neuralNetwork.Layers, 0.9);
        }
        double result = 0.0;
        size_t matchCount = 0;
        for (const auto& sample : testSamples) {
            std::vector<double> outputs = forward.ForwardPropagation(neuralNetwork.Layers, sample.Data);
            auto maxIt = std::max_element(outputs.begin(), outputs.end());
            int predictedDigit = std::distance(outputs.begin(), maxIt);
            auto expectedMaxIt = std::max_element(sample.Targets.begin(), sample.Targets.end());
            int expectedDigit = std::distance(sample.Targets.begin(), expectedMaxIt);

            if (predictedDigit == expectedDigit)
                ++matchCount;
        }
        result = static_cast<double>(matchCount) / testSamples.size();

        std::cout << std::endl;
        std::cout << "Layers " << layersDataStr
                  << "   Epoch " << epoch
                  << "   Accuracy " << std::fixed << std::setprecision(4) << (result * 100.0) << "%"
                  << "   ";
        if (bestResult > 0) {
            if (result > bestResult)
                std::cout << "BETTER";
            else
                std::cout << "WORST";
        }
        std::cout << "   -- ";
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " --" << std::endl;
        if (result > bestResult) {
            bestResult = result;
            std::thread saveThread([&networkState, &neuralNetwork, epoch, bestResult, &layersDataStr]() {
                networkState.SaveNetworkState(neuralNetwork, "models", epoch, bestResult, layersDataStr);
            });
            saveThread.detach();
        }
    }

    std::cout << "-----------  TRAINING COMPLETED.  -----------" << std::endl;

    return 0;
}
