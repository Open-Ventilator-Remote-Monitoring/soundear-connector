#ifndef HELPERS_HPP
#define HELPERS_HPP
#include <string>
#include "Sockets/protocoldefs.hpp"

class Helpers
{
public:
    Helpers();

    bool GetIpAddress(std::string nicName, uint8_t* ip);
    bool GetSubnet(std::string nicName, uint8_t* ip);
    bool GetGateway(std::string nicName, uint8_t* ip);
    bool GetDhcpEnabled(std::string nicName);

    bool FindInFile(std::string file, std::string item, int *sof, std::string &contents);
    bool IndexOf(std::string source, std::string item, int* sof);

    bool EqualFiles(std::string fileA, std::string fileB);
    void ReplaceFile(std::string fileSource, std::string fileDest);

    std::string Exec(const char* cmd);

    bool Replace(std::string& str, const std::string& from, const std::string& to);
    bool FindEthSection(std::string interface, int* sofWlan, int* lengthWlan);
    bool FindWlanSection(std::string interface, int* sofWlan, int* lengthWlan);

    void UpdateWLAN(Se3::UsbProtocol::ConnectivityConfig config);
    void UpdateEth(Se3::UsbProtocol::ConnectivityConfig config);
    void UpdateModem(Se3::UsbProtocol::ConnectivityConfig config);

    const std::string wpaFilename = "/etc/wpa_supplicant.conf";
    const std::string ifFilename ="/etc/network/interfaces";
    const std::string wvdialFilename ="/etc/wvdial.conf";

    const std::string tmpWpaFilename ="/tmp/wpa_supplicant.conf";
    const std::string tmpIfFilename ="/tmp/interfaces";
    const std::string tmpWvdialFilename ="/tmp/wvdial.conf";
private:
    std::string _nicConfigPath = "/etc/network/interfaces";
};

#endif // HELPERS_HPP
