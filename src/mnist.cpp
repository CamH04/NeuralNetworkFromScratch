#include "mnist.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>

using std::ifstream;
using std::ios;

vector<Sample> MNIST::LoadImages(const string& imgFileName, const string& labelFileName, int imgCount) {
    ifstream imageReader(imgFileName, ios::binary);
    ifstream labelReader(labelFileName, ios::binary);

    if (!imageReader || !labelReader) {
       if (!imageReader.is_open()) {
            std::cout << "Error: Could not open image file: " << imgFileName << std::endl;
        }
        if (!labelReader.is_open()) {
            std::cout << "Error: Could not open label file: " << labelFileName << std::endl;
        }
        if (!imageReader || !labelReader) {
            throw std::runtime_error("Failed to open MNIST dataset files.");
        }
    }

    char byte4[4];

    // skip magic number and number of images
    imageReader.read(byte4, 4);
    imageReader.read(byte4, 4);

    // skip rows and cols (we assume 28x28 = 784)
    imageReader.read(byte4, 4);
    imageReader.read(byte4, 4);

    // skip label file magic number and label count
    labelReader.read(byte4, 4);
    labelReader.read(byte4, 4);

    auto targets = GetTargets();
    vector<Sample> samples(imgCount);

    for (int i = 0; i < imgCount; ++i) {
        samples[i].Data.resize(784);
        char buffer[784];
        imageReader.read(buffer, 784);
        for (int b = 0; b < 784; ++b) {
            samples[i].Data[b] = static_cast<unsigned char>(buffer[b]) / 256.0;
        }

        unsigned char label;
        labelReader.read(reinterpret_cast<char*>(&label), 1);
        samples[i].Label = static_cast<int>(label);
        samples[i].Targets = targets[samples[i].Label];
    }

    return samples;
}

vector<vector<double>> MNIST::GetTargets() {
    vector<vector<double>> targets(10, vector<double>(10, 0.0));
    for (int i = 0; i < 10; ++i) {
        targets[i][i] = 1.0;
    }
    return targets;
}
