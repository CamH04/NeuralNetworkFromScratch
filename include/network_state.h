#ifndef __NETWORK_STATE__H
#define __NETWORK_STATE__H

#include <neural_network.h>
#include <string>

class NetworkState {
public:
    void SaveNetworkState(const neural_network& net, const std::string& folderPath,
                          int epoch, double accuracy, const std::string& layersData);

    bool LoadNetworkState(const std::string& filePath, neural_network& net);

private:
    void DeleteFileAsync(const std::string& filePath);
    void Serialize(std::ostream& out, const neural_network& net);
    bool Deserialize(std::istream& in, neural_network& net);
};

#endif
