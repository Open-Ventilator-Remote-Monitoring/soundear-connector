#include "common.hpp"
#include <string.h>
#include <algorithm>

void HighResRecord::SetNumberOfMeasurements(int count)
{
    //lock access to the class member vars
    std::lock_guard<std::mutex> lock(_recMutex);

    _maxCount=count;
    _currentCount=0;

    _LAFmax.clear();
    _LAS.clear();
    _LAeq1s.clear();
    _LCpeakMax.clear();

    _tmpLaf=0;_tmpLas=0;_tmpLAeq=0;_tmpLCpeak=0;
    //lock is release automatically
}

bool HighResRecord::AddMeasurement(Se3::UsbProtocol::DeviceTimeShort *time, float laf, float las,float laeq1s, float peakMax, float lcf, float lcs)
{
    //lock access to the class member vars
    std::lock_guard<std::mutex> lock(_recMutex);

    bool isFull=false;
    _tmpLaf = std::max(static_cast<uint16_t>(laf * 10.0f), _tmpLaf);
    _tmpLas = static_cast<uint16_t>(las * 10.0f);
    _tmpLcf = std::max(static_cast<uint16_t>(lcf * 10.0f), _tmpLcf);
    _tmpLcs = static_cast<uint16_t>(lcs * 10.0f);
    _tmpLAeq = static_cast<uint16_t>(laeq1s * 10.0f);
    _tmpLCpeak = static_cast<uint16_t>(peakMax * 10.0f);


    if (_lastDeviceTime.sec != time->sec)
    {
        if (_currentCount==0)
        {
            memcpy(&_deviceTimeStart,time,sizeof(Se3::UsbProtocol::DeviceTimeShort));
        }
        //Collected 1 sec of data
        _LAFmax.push_back(_tmpLaf);
        _LAS.push_back(_tmpLas);
        _LCFmax.push_back(_tmpLcf);
        _LCS.push_back(_tmpLcs);
        _LAeq1s.push_back(_tmpLAeq);
        _LCpeakMax.push_back(_tmpLCpeak);

        _tmpLaf=0;_tmpLas=0;_tmpLAeq=0;_tmpLCpeak=0;_tmpLcf=0;_tmpLcs=0;

        _currentCount++;
        if (_currentCount>=_maxCount)
        {
            _currentCount=0;
            isFull = true;

            //Todo copy to buffer
            CreateRawRecord();
            //Clear the buffers
            _LAFmax.clear();
            _LAS.clear();
            _LAeq1s.clear();
            _LCpeakMax.clear();
            _LCFmax.clear();
            _LCS.clear();
        }
    }
    memcpy(&_lastDeviceTime,time,sizeof(Se3::UsbProtocol::DeviceTimeShort));

    //lock is release automatically
    return isFull;
}

std::vector<uint8_t> *HighResRecord::GetRawRecord()
{
    return &_rawBuffer;
}

void HighResRecord::CreateRawRecord()
{
    _rawBuffer.clear();

    _rawBuffer.push_back(_deviceTimeStart.year);
    _rawBuffer.push_back(_deviceTimeStart.month);
    _rawBuffer.push_back(_deviceTimeStart.day);
    _rawBuffer.push_back(_deviceTimeStart.hour);
    _rawBuffer.push_back(_deviceTimeStart.min);
    _rawBuffer.push_back(_deviceTimeStart.sec);

    //LAF buffer
    for(auto const& value: _LAFmax)
    {
        uint8_t raw[2];
        memcpy(raw,&value,2);

        for(auto const& r: raw)
        {
            _rawBuffer.push_back(r);
        }
    }

    //LAS buffer
    for(auto const& value: _LAS)
    {
        uint8_t raw[2];
        memcpy(raw,&value,2);

        for(auto const& r: raw)
        {
            _rawBuffer.push_back(r);
        }
    }

    //LAeq1s buffer
    for(auto const& value: _LAeq1s)
    {
        uint8_t raw[2];
        memcpy(raw,&value,2);

        for(auto const& r: raw)
        {
            _rawBuffer.push_back(r);
        }
    }

    //LcPeakbuffer
    for(auto const& value: _LCpeakMax)
    {
        uint8_t raw[2];
        memcpy(raw,&value,2);

        for(auto const& r: raw)
        {
            _rawBuffer.push_back(r);
        }
    }

    //LCF buffer
    for(auto const& value: _LCFmax)
    {
        uint8_t raw[2];
        memcpy(raw,&value,2);
        for(auto const& r: raw)
        {
            _rawBuffer.push_back(r);
        }
    }

    //LCS buffer
    for(auto const& value: _LCS)
    {
        uint8_t raw[2];
        memcpy(raw,&value,2);

        for(auto const& r: raw)
        {
            _rawBuffer.push_back(r);
        }
    }
}
