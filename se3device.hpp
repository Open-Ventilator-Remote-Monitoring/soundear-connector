#ifndef SE3DEVICE_HPP
#define SE3DEVICE_HPP

#include "DeviceBase.hpp"
#include <stdio.h>
#include <thread>
#include <Sockets/iprotocolinterface.hpp>
#include "hidapi/hidapi.h"
#include "Observers/observers.hpp"
#include "Sockets/metersocket.hpp"
#include "Sockets/filesocket.hpp"

class SE3device: public DeviceBase, public IProtocolInterface, public IDeviceFilereadObserver
{
public:
    SE3device();
    virtual bool Open();
    //virtual void Decode(uint8_t* frame, uint8_t length);
    virtual bool SendFrame(Se3::UsbProtocol::Se3Command cmdFrame);
    bool SendFrame(std::vector<uint8_t> cmdFrame);
    void Close();
    void Run();

    void RequestVersion();
    void RequestName();
    void RequestConnectivityConfig();
    void ResetDevice();

    void Attach(IMeasurementObserver* observer);
    void Attach(IDeviceNameObserver* observer);
    void Attach(IDeviceVersionObserver* observer);
    void Attach(IDeviceConnectivityConfigObserver *observer);
    void Attach(IDeviceConnectedObserver *observer);

    void NotifyConnectivityConfig(bool success, Se3::UsbProtocol::ConnectivityConfig config);
    void NotifyDeviceConnected(bool connected);

    virtual void Update(bool success, std::vector<uint8_t> data);

    private:
    int _device;
    std::thread* _receiver;
    bool _terminate=false;
    std::vector<IDeviceConnectivityConfigObserver*> _observerConCfg;
    std::vector<IDeviceConnectedObserver*> _observerDeviceConnected;
    std::string deleteme;

    hid_device *handle;
    MeterSocket _meterSocket;
    FileSocket _fileSocket;
};

#endif // SE3DEVICE_HPP
