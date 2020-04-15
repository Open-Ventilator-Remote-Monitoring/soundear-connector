#include "se3device.hpp"
#include <iostream>
#include <cstring>
#include <stdio.h>

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <thread>
#include <errno.h>
#include <sstream>
#include <memory>

SE3device::SE3device(): _meterSocket(this,12), _fileSocket(this,15)
{
    _fileSocket.Attach(this);
}

bool SE3device::Open()
{
    bool opened=false;
    handle = hid_open(0x0C17, 0x0901, NULL);
    if (!handle) {
        std::cout<< "\r\nunable to open device"<<std::endl;
        return opened;
    }

    // Set the hid_read() function to be non-blocking.
    hid_set_nonblocking(handle, 0);

    if (handle != nullptr)
    {
        _terminate=false;
        std::cout <<"Start rx'ing"<<std::endl;
        _receiver= new std::thread(&SE3device::Run,this);
        opened=true;
    }
    return opened;
}

bool SE3device::SendFrame(Se3::UsbProtocol::Se3Command cmdFrame)
{
    int res = hid_write(handle, (const uint8_t*)&cmdFrame, 64);
    if (res < 0) {
        printf("Unable to write()\n");
        printf("Error: %ls\n", hid_error(handle));
    }
    return true;
}

bool SE3device::SendFrame(std::vector<uint8_t> cmdFrame)
{
    if (cmdFrame.size()==64)
    {
        int res = hid_write(handle, cmdFrame.data(), 64);
        if (res < 0) {
            printf("Unable to write()\n");
            printf("Error: %ls\n", hid_error(handle));
        }
        return true;
    }
    else
    {
        return false;
    }

}

void SE3device::Close()
{
    _terminate=true;
    if(_receiver!=nullptr && _receiver->joinable())
    {
        _receiver->join();
        delete _receiver;
    }

}

void SE3device::Run()
{
    std::cout<<"Started\n";
    std::int32_t failCount = 0;
    while (!_terminate)
    {
        uint8_t frame[64];
        int ret = hid_read_timeout(handle, frame, sizeof(frame),10000);

        if (ret<=0)
        {
            failCount++;
            if (failCount>20)
            {
                _terminate=true;
            }
        }
        else
        {
            failCount = 0;
        }

        uint8_t socket =frame[1];
        if (socket==12)
        {
            _meterSocket.Decode(&frame[1],63);
        }
        else if (socket ==15)
        {
            _fileSocket.Decode(&frame[1],63);
        }
    }
    std::cout<<"Stopping hid\r\n";
    hid_close(handle);

    /* Free static HIDAPI objects. */
    hid_exit();

    NotifyDeviceConnected(false);
}

void SE3device::RequestVersion()
{
    _meterSocket.RequestVersion();
}

void SE3device::RequestName()
{
    _meterSocket.RequestName();
}

void SE3device::RequestConnectivityConfig()
{
    _fileSocket.ReadFile("1:/connect.bin",nullptr);
}

void SE3device::ResetDevice()
{
    _fileSocket.Reset();
}

void SE3device::Attach(IMeasurementObserver *observer)
{
    _meterSocket.Attach(observer);
}

void SE3device::Attach(IDeviceNameObserver *observer)
{
    _meterSocket.Attach(observer);
}

void SE3device::Attach(IDeviceVersionObserver *observer)
{
    _meterSocket.Attach(observer);
}

void SE3device::Attach(IDeviceConnectivityConfigObserver *observer)
{
    _observerConCfg.push_back(observer);
}

void SE3device::Attach(IDeviceConnectedObserver *observer)
{
    _observerDeviceConnected.push_back(observer);
}

void SE3device::NotifyConnectivityConfig(bool success, Se3::UsbProtocol::ConnectivityConfig config)
{
    for(std::vector<IDeviceConnectivityConfigObserver*>::const_iterator iter = _observerConCfg.begin(); iter != _observerConCfg.end(); ++iter)
    {
        if(*iter != 0)
        {
            (*iter)->Update(success,config);
        }
    }
}

void SE3device::NotifyDeviceConnected(bool connected)
{
    for(std::vector<IDeviceConnectedObserver*>::const_iterator iter = _observerDeviceConnected.begin(); iter != _observerDeviceConnected.end(); ++iter)
    {
        if(*iter != 0)
        {
            (*iter)->Update(connected);
        }
    }
}

void SE3device::Update(bool success, std::vector<uint8_t> data)
{
    Se3::UsbProtocol::ConnectivityConfig  cfg;
    Se3::UsbProtocol::ConnectivityConfigRaw cfgRaw;

    if (data.size() <= sizeof cfgRaw)
    {
        std::copy(data.begin(), data.end(), reinterpret_cast<char*>(&cfgRaw));

        cfg.Wifi = (cfgRaw.Wifi>0);
        cfg.SSID = std::string(cfgRaw.SSID, cfgRaw.SSID + sizeof cfgRaw.SSID);
        //remove trailing zeros
        cfg.SSID = std::string(cfg.SSID.c_str());
        cfg.Password = std::string(cfgRaw.Password, cfgRaw.Password + sizeof cfgRaw.Password );
        //remove trailing zeros
        cfg.Password = std::string(cfg.Password.c_str());

        cfg.Dhcp = (cfgRaw.Dhcp>0);
        std::memcpy(cfg.Ip,cfgRaw.Ip,sizeof cfg.Ip);
        std::memcpy(cfg.Sub,cfgRaw.Sub,sizeof cfg.Sub);
        std::memcpy(cfg.Gw,cfgRaw.Gw,sizeof cfg.Gw);

        cfg.SiteId = std::string(cfgRaw.SiteId, cfgRaw.SiteId + sizeof cfgRaw.SiteId);
        //remove trailing zeros
        cfg.SiteId = std::string(cfg.SiteId.c_str());

        cfg.Url = std::string(cfgRaw.Url, cfgRaw.Url + sizeof cfgRaw.Url);
        //remove trailing zeros
        cfg.Url = std::string(cfg.Url.c_str());

        cfg.User = std::string(cfgRaw.User, cfgRaw.User + sizeof cfgRaw.User);
        //remove trailing zeros
        cfg.User = std::string(cfg.User.c_str());

        cfg.Pass = std::string(cfgRaw.Pass, cfgRaw.Pass + sizeof cfgRaw.Pass);
        //remove trailing zeros
        cfg.Pass = std::string(cfg.Pass.c_str());

        memcpy(&cfg.Port, cfgRaw.Port,4);

        cfg.APN = std::string(cfgRaw.APN, cfgRaw.APN + sizeof cfgRaw.APN);
        //remove trailing zeros
        cfg.APN = std::string(cfg.APN.c_str());

        NotifyConnectivityConfig(success,cfg);
    }
    else
    {
        std::cout << "err size"<<std::endl;
        NotifyConnectivityConfig(false,cfg);
    }
}

