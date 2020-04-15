#include "metersocket.hpp"
#include "protocoldefs.hpp"
#include "protocoldefs.hpp"
#include <cstring>
#include <string>
#include <iostream>

MeterSocket::MeterSocket(IProtocolInterface *prot, int id): ISocketBase(prot,id)
{}

void MeterSocket::Decode(uint8_t *frame, uint8_t length)
{
    if (length<4)
        return;

    Se3::UsbProtocol::Se3Command cmd;
    cmd.reportId=2;
    cmd.socket=frame[0];
    cmd.property=frame[1];
    cmd.dataLength=frame[2];
    std::memcpy(cmd.data,&frame[3],length-3);

    bool setCommand = cmd.property & 0x80;
    Properties property = (Properties)(cmd.property&0x7F);

    if (setCommand)
    {
        switch (property)
        {

            //Measurements!
            case Properties::measurement:
            {
                float LAF=0;
                float LAS=0;
                float LAeq1s=0;
                float LCpeak=0;
                float LCF=0;
                float LCS=0;

                memcpy(&LAF,&cmd.data[0],4);
                memcpy(&LAeq1s,&cmd.data[4],4);
                memcpy(&LAS,&cmd.data[8],4);
                memcpy(&LCpeak,&cmd.data[12],4);

                Se3::UsbProtocol::DeviceTime devTime;
                devTime.year = cmd.data[16];
                devTime.month = cmd.data[17];
                devTime.day = cmd.data[18];
                devTime.hour= cmd.data[19];
                devTime.min = cmd.data[20];
                devTime.sec = cmd.data[21];
                devTime.ms = cmd.data[22] | cmd.data[23]<<8;

                if (cmd.dataLength >= 32)
                {
                    memcpy(&LCF,&cmd.data[24],4);
                    memcpy(&LCS,&cmd.data[28],4);
                }

                NotifyMeasurements(&devTime, LAF, LAeq1s, LAS, LCpeak, LCF, LCS);
            }
            break;
            //Name
            case Properties::name:
            {
                std::string s;//(cmd.data,cmd.dataLength);
                s.assign((const char*)cmd.data,(size_t)cmd.dataLength);
                NotifyName(s);

                _lastOperation = OperationResult::Success;
                _responseRxed = true;
                _responseCheck.notify_one();
            }
            break;
            //version
            case Properties::firmwareVersion:
            {
                Se3::UsbProtocol::VersionInfo version;
                version.major = cmd.data[0];
                version.minor = cmd.data[1];
                version.build = cmd.data[2];
                version.product.assign((const char*)&cmd.data[3],20);
                memcpy(&version.id,&cmd.data[23],12);

                NotifyVersion(&version);

                _lastOperation = OperationResult::Success;
                _responseRxed = true;
                _responseCheck.notify_one();                
            }
            break;
            case Properties::leq:
            {
               float leq15min, leq30min, leq60min;//= BitConverter.ToSingle(cmdFrame.data, 0);


               memcpy(&leq30min,&cmd.data[0],4);
               memcpy(&leq15min,&cmd.data[4],4);
               memcpy(&leq60min,&cmd.data[8],4);

               uint8_t peakCounters[4];

               if (cmd.dataLength >= 16)
               {
                   memcpy(peakCounters,&cmd.data[12],4);
               }

                NotifySlow(leq30min, leq15min, leq60min,peakCounters);
            }
               break;
        default:
            break;
        }
    }
}

void MeterSocket::Attach(IMeasurementObserver *observer)
{
    _observerMeas.push_back(observer);
}

void MeterSocket::Attach(IDeviceNameObserver *observer)
{
    _observerName.push_back(observer);
}

void MeterSocket::Attach(IDeviceVersionObserver *observer)
{
    _observerVersion.push_back(observer);
}

void MeterSocket::NotifyMeasurements(Se3::UsbProtocol::DeviceTime* deviceTime, float laf, float laeq1s, float las, float lcPeak, float lcf, float lcs)
{
    for(std::vector<IMeasurementObserver*>::const_iterator iter = _observerMeas.begin(); iter != _observerMeas.end(); ++iter)
    {
        if(*iter != 0)
        {
            (*iter)->UpdateFast(deviceTime, laf, laeq1s, las, lcPeak, lcf, lcs);
        }
    }
}

void MeterSocket::NotifySlow(float leq30min, float leq15min, float leq60min, uint8_t *peaks)
{
    for(std::vector<IMeasurementObserver*>::const_iterator iter = _observerMeas.begin(); iter != _observerMeas.end(); ++iter)
    {
        if(*iter != 0)
        {
            (*iter)->UpdateSlow(leq30min,leq15min,leq60min,peaks);
        }
    }
}

void MeterSocket::NotifyName(std::string name)
{
    for(std::vector<IDeviceNameObserver*>::const_iterator iter = _observerName.begin(); iter != _observerName.end(); ++iter)
    {
        if(*iter != 0)
        {
            (*iter)->Update(name);
        }
    }
}

void MeterSocket::NotifyVersion(Se3::UsbProtocol::VersionInfo *pInfo)
{
    for(std::vector<IDeviceVersionObserver*>::const_iterator iter = _observerVersion.begin(); iter != _observerVersion.end(); ++iter)
    {
        if(*iter != 0)
        {
            (*iter)->Update(pInfo);
        }
    }
}

void MeterSocket::RequestName()
{
    Se3::UsbProtocol::Se3Command cmd;

    cmd.socket = (uint8_t)_id;
    cmd.reportId=1;
    cmd.property = (uint8_t)Properties::name | ((uint8_t)Se3::UsbProtocol::Commands::get << 7);
    cmd.dataLength = 0;

    //_prot->SendFrame(cmd);
    TransmitCommandBlocking(cmd);
   // AwaitAnswer(5000);
    //Console.WriteLine("Elapsed " + (DateTime.Now - start).TotalMilliseconds);
}

void MeterSocket::RequestVersion()
{
    Se3::UsbProtocol::Se3Command cmd;

    cmd.socket = (uint8_t)_id;
    cmd.reportId=1;
    cmd.property = (uint8_t)Properties::firmwareVersion | ((uint8_t)Se3::UsbProtocol::Commands::get << 7);
    cmd.dataLength = 0;

    //_prot->SendFrame(cmd);
    TransmitCommandBlocking(cmd);
}

bool MeterSocket::BlockAwaitResponse()
{

    while (!_responseTimeout)
    {
        std::unique_lock<std::mutex> locker(_reqresp_mutex);

        while (!_responseRxed) // used to avoid spurious wakeups
        {
            if (_responseCheck.wait_for(locker, std::chrono::seconds(5))==std::cv_status::timeout)
            {
                _responseTimeout=true;
                break;
            }
        }

        if (_responseRxed)
            break;
    }

    return _responseRxed && !_responseTimeout;
}

bool MeterSocket::TransmitCommandBlocking(Se3::UsbProtocol::Se3Command& pCmd)
{
    _responseRxed=false;
    _responseTimeout=false;

    _prot->SendFrame(pCmd);

    return BlockAwaitResponse();
}
