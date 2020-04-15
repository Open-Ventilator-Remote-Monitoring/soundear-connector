#ifndef METERSOCKET_H
#define METERSOCKET_H

#include <stdint.h>
#include "iprotocolinterface.hpp"
#include "isocketbase.hpp"
#include "Observers/observers.hpp"

#include <mutex>
#include <condition_variable>
#include <vector>

class MeterSocket: public ISocketBase
{
public:
    MeterSocket(IProtocolInterface *prot, int id);

    void RequestName();
    void RequestVersion();
    void Decode(uint8_t *frame, uint8_t length);

    void Attach(IMeasurementObserver* observer);
    void Attach(IDeviceNameObserver* observer);
    void Attach(IDeviceVersionObserver* observer);
private:
    enum class Properties: uint8_t
    {
        firmwareVersion = 0x01,
        config = 0x02,
        measurement = 0x03,
        time = 0x04,
        rms =0x05,
        micCalibration=0x06,
        leq=0x07,
        usbStatus = 0x08,
        hardwareVariant = 0x09,
        name =			0x0A,
        location =		0x0B,
        refreshName= 	0x0C,
        usbActive=      0x0D,
        testData =0x20,

        ok = 0x7F,
        error = 0x7E
    };

    std::vector<IMeasurementObserver*> _observerMeas;
    std::vector<IDeviceNameObserver*> _observerName;
    std::vector<IDeviceVersionObserver*> _observerVersion;

    virtual void NotifyMeasurements(Se3::UsbProtocol::DeviceTime *deviceTime, float laf, float laeq1s, float las, float lcPeak, float lcf, float lcs);
    virtual void NotifySlow(float leq30min, float leq15min, float leq60min, uint8_t* peaks);
    virtual void NotifyName(std::string name);
    virtual void NotifyVersion(Se3::UsbProtocol::VersionInfo *pInfo);

    std::mutex _reqresp_mutex;
    std::condition_variable _responseCheck;
    bool _responseRxed=false;
    bool _responseTimeout=false;

    bool BlockAwaitResponse();
    bool TransmitCommandBlocking(Se3::UsbProtocol::Se3Command &pCmd);
};

#endif // METERSOCKET_H
