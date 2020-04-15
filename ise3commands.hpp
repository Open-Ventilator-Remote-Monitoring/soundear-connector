#ifndef ISE3COMMANDS_HPP
#define ISE3COMMANDS_HPP

#include <stdint.h>
#include <string>

class ISe3TransmitCommands
{
public:
    virtual void Measurements(float laf, float laeq1s, float las, float lcpeak)=0;
    virtual void Version(uint8_t major,uint8_t minor,uint8_t build)=0;
    virtual void name(std::string name);
};

class ISe3ReceiveCommands
{
public:
    virtual void RequestVersion()=0;
    virtual void RequestName()=0;
};

typedef struct
{
    uint8_t reportId;
    uint8_t socket;
    uint8_t property;
    uint8_t dataLength;
    uint8_t data[60];
} Se3Command;

#endif // ISE3COMMANDS_HPP
