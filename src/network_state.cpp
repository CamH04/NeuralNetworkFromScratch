#include "network_state.h"
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

//eg on how to use
/*
neural_network net;

NetworkState state;
state.SaveNetworkState(net, "models", 10, 0.876, "784-64-10");

neural_network loaded;
state.LoadNetworkState("models/Layers-784-64-10--Epoch-10--Accuracy-87.60%--2025-06-22-14-35.binary", loaded);
*/


void NetworkState::SaveNetworkState(const neural_network& net, const std::string& folderPath,
                                    int epoch, double accuracy, const std::string& layersData) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* tm = std::localtime(&now_time);

    std::ostringstream fileNameStream;
    fileNameStream << "Layers-" << layersData
                   << "--Epoch-" << epoch
                   << "--Accuracy-" << std::fixed << std::setprecision(2) << (accuracy * 100)
                   << "%" << "--"
                   << std::put_time(tm, "%Y-%m-%d-%H-%M") << ".binary";

    std::string fileName = fileNameStream.str();
    std::string filePath = folderPath + "/" + fileName;

    if (!fs::exists(folderPath)) {
        fs::create_directories(folderPath);
    }
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        std::string name = entry.path().filename().string();

        if (name.find(layersData) != std::string::npos) {
            std::regex regex("-Accuracy-(\\d+\\.?\\d*)%");
            std::smatch match;
            if (std::regex_search(name, match, regex)) {
                double existingAccuracy = std::stod(match[1]);

                if ((accuracy * 100) > existingAccuracy) {
                    std::thread(&NetworkState::DeleteFileAsync, this, entry.path().string()).detach();
                } else {
                    std::cout << "There is a file with better accuracy for these layer combinations.\n";
                    std::cout << "Existing file name     : " << name << "\n";
                    std::cout << "Existing file accuracy : " << existingAccuracy << "%\n";
                    std::cout << "New accuracy           : " << (accuracy * 100) << "%\n";
                    return;
                }
            }
        }
    }

    try {
        std::ofstream out(filePath, std::ios::binary);
        if (!out) {
            throw std::ios_base::failure("Failed to open file.");
        }

        Serialize(out, net);
        std::cout << "Network parameters saved successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error occurred while saving network parameters: " << e.what() << "\n";
        std::thread(&NetworkState::DeleteFileAsync, this, filePath).detach();
    }
}

bool NetworkState::LoadNetworkState(const std::string& filePath, neural_network& net) {
    try {
        std::ifstream in(filePath, std::ios::binary);
        if (!in.is_open()) {
            std::cerr << "Failed to open file: " << filePath << "\n";
            return false;
        }

        return Deserialize(in, net);
    } catch (const std::exception& e) {
        std::cerr << "Deserialization failed: " << e.what() << "\n";
        return false;
    }
}

void NetworkState::Serialize(std::ostream& out, const neural_network& net) {
    size_t layerCount = net.Layers.size();
    out.write(reinterpret_cast<const char*>(&layerCount), sizeof(size_t));

    for (const auto& layer : net.Layers) {
        size_t neuronCount = layer.Neurons.size();
        out.write(reinterpret_cast<const char*>(&neuronCount), sizeof(size_t));

        for (const auto& neuron : layer.Neurons) {
            out.write(reinterpret_cast<const char*>(&neuron.Bias), sizeof(double));

            size_t weightCount = neuron.Weights.size();
            out.write(reinterpret_cast<const char*>(&weightCount), sizeof(size_t));

            for (double w : neuron.Weights) {
                out.write(reinterpret_cast<const char*>(&w), sizeof(double));
            }
        }
    }
}

bool NetworkState::Deserialize(std::istream& in, neural_network& net) {
    size_t layerCount;
    in.read(reinterpret_cast<char*>(&layerCount), sizeof(size_t));
    net.Layers.clear();
    net.Layers.reserve(layerCount);

    for (size_t l = 0; l < layerCount; ++l) {
        size_t neuronCount;
        in.read(reinterpret_cast<char*>(&neuronCount), sizeof(size_t));

        layer lyr;
        lyr.Neurons.reserve(neuronCount);

        for (size_t n = 0; n < neuronCount; ++n) {
            neuron neu;
            in.read(reinterpret_cast<char*>(&neu.Bias), sizeof(double));

            size_t weightCount;
            in.read(reinterpret_cast<char*>(&weightCount), sizeof(size_t));
            neu.Weights.resize(weightCount);

            for (size_t w = 0; w < weightCount; ++w) {
                in.read(reinterpret_cast<char*>(&neu.Weights[w]), sizeof(double));
            }

            lyr.Neurons.push_back(neu);
        }

        net.Layers.push_back(lyr);
    }

    return true;
}

void NetworkState::DeleteFileAsync(const std::string& filePath) {
    int retryDelayMs = 100;
    int retryCount = 0;
    const int maxRetries = 100;
    bool deleted = false;

    while (!deleted && retryCount < maxRetries) {
        try {
            std::filesystem::remove(filePath);
            deleted = true;
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Deleting ERROR on file: " << filePath << "\n";
            std::cerr << "ERROR: " << e.what() << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(retryDelayMs));
            retryCount++;
        }
    }

    if (deleted)
        std::cout << "Deleted file: " << filePath << "\n";
    else
        std::cerr << "Failed to delete file: " << filePath << "\n";
}
