#ifndef IOBSERVERS_HPP
#define IOBSERVERS_HPP

#include <string>
#include "Sockets/protocoldefs.hpp"
#include <vector>
class IMeasurementObserver
{
public:
    virtual void UpdateFast(const Se3::UsbProtocol::DeviceTime *deviceTime, float laf, float laeq1s, float las, float lcPeak, float lcf, float lcs) = 0;
    virtual void UpdateSlow(float leq1min, float leq15min, float leq30min, uint8_t* peaks) = 0;
};

class IDeviceNameObserver
{
public:
    virtual void Update(std::string name) = 0;
};

class IDeviceVersionObserver
{
public:
    virtual void Update(const Se3::UsbProtocol::VersionInfo *pInfo) = 0;
};

class IDeviceFilereadObserver
{
public:
    virtual void Update(bool success, std::vector<uint8_t> data) = 0;
};

class IDeviceConnectivityConfigObserver
{
public:
    virtual void Update(bool success, Se3::UsbProtocol::ConnectivityConfig config) = 0;
};

class IDeviceConnectedObserver
{
public:
    virtual void Update(bool connected) = 0;
};

class IMqttTopic
{
public:
    virtual void Update(std::string topic, std::vector<uint8_t> data)=0;
    virtual void ConnectionChange(bool isConnected)=0;
};

#endif // IOBSERVERS_HPP
