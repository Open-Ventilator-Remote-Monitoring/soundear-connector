#ifndef COMMON_HPP
#define COMMON_HPP
#include "Sockets/protocoldefs.hpp"
#include <math.h>
#include <string.h>
#include <vector>
#include <mutex>

class HighResRecord
{
public:
    HighResRecord()
    {}
    void SetNumberOfMeasurements(int count);
    bool AddMeasurement(Se3::UsbProtocol::DeviceTimeShort *time, float laf, float las, float laeq1s, float peakMax, float lcf, float lcs);
    std::vector<uint8_t>* GetRawRecord();

private:

    void CreateRawRecord();
    Se3::UsbProtocol::DeviceTimeShort _deviceTimeStart;
    Se3::UsbProtocol::DeviceTimeShort _lastDeviceTime;
    uint16_t _tmpLaf;
    uint16_t _tmpLas;
    uint16_t _tmpLAeq;
    uint16_t _tmpLCpeak;
    uint16_t _tmpLcf;
    uint16_t _tmpLcs;

    std::vector<uint16_t> _LAFmax;
    std::vector<uint16_t> _LAS;
    std::vector<uint16_t> _LAeq1s;
    std::vector<uint16_t> _LCpeakMax;
    std::vector<uint16_t> _LCFmax;
    std::vector<uint16_t> _LCS;

    std::vector<uint8_t> _rawBuffer;

    int _currentCount=0;
    int _maxCount=1;
    std::mutex _recMutex;
};

#endif // COMMON_HPP
