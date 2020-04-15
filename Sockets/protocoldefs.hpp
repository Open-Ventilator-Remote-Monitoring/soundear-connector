#ifndef PROTOCOLDEFS_HPP
#define PROTOCOLDEFS_HPP

#include <stdint.h>
#include <string>

namespace Se3
{
namespace UsbProtocol
{
    const int maxDataLength=60;
    typedef struct
    {
        uint8_t reportId;
        uint8_t socket;
        uint8_t property;
        uint8_t dataLength;
        uint8_t data[maxDataLength];
    } Se3Command;

    typedef struct
    {
        uint8_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t min;
        uint8_t sec;
        uint16_t ms;
    } DeviceTime;

    typedef struct
    {
        uint8_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t min;
        uint8_t sec;
    } DeviceTimeShort;

    typedef struct
    {
        uint8_t major;
        uint8_t minor;
        uint8_t build;
        std::string product;
        int id[3];
    } VersionInfo;

    #pragma pack(push, 1)
    typedef struct
    {
        DeviceTime MeasurementTime;
        uint16_t LAFmax ;
        uint16_t LAeq1sMax;
        uint16_t LASmax;
        uint16_t LCpeak;
        uint16_t LCFmax;
        uint16_t LCSmax;
        uint16_t LAeq1min;
        uint16_t LAeq15min;
        uint16_t LAeq60min;
        uint8_t LCpeakAbove105;
        uint8_t LCpeakAbove110;
        uint8_t LCpeakAbove115;
        uint8_t LCpeakAbove120;
        uint8_t alarmCount;
    } MeasurementFromWirelessV2;
    #pragma pack(pop)

    enum class Commands
    {
        get = 0,
        set = 1
    };

    const int StringLength = 100;
    const int IpLength = 4;
    const int SiteIdLength = 6;

    typedef struct
    {
        uint8_t Wifi;
        char SSID[StringLength];
        char Password[StringLength];
        uint8_t Dhcp;
        uint8_t Ip[IpLength];
        uint8_t Sub[IpLength];
        uint8_t Gw[IpLength];
        char SiteId[StringLength];
        char Url[StringLength];
        char User[StringLength];
        char Pass[StringLength];
        uint8_t Port[4];
        char APN[StringLength];
    } ConnectivityConfigRaw;

    typedef struct
    {
        bool Wifi;
        std::string SSID;
        std::string Password;
        bool Dhcp;
        uint8_t Ip[IpLength];
        uint8_t Sub[IpLength];
        uint8_t Gw[IpLength];
        std::string SiteId;
        std::string Url;
        std::string User;
        std::string Pass;
        int32_t Port;
        std::string APN;
    } ConnectivityConfig;
}
}
#endif // PROTOCOLDEFS_HPP
