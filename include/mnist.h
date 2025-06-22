#ifndef __MNIST__H
#define __MNIST__H

#include <vector>
#include <string>

using std::vector;
using std::string;

struct Sample {
    vector<double> Data;
    vector<double> Targets;
    int Label;
};

class MNIST {
public:
    vector<Sample> LoadImages(const string& imgFileName, const string& labelFileName, int imgCount);

private:
    vector<vector<double>> GetTargets();
};

#endif
