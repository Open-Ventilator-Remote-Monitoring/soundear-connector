#ifndef IDATAPROVIDER_HPP
#define IDATAPROVIDER_HPP

#include <stdint.h>
#include <string>

#include "ise3commands.hpp"

class DeviceBase
{
public:

    virtual ~DeviceBase()
    {}

    virtual bool Open()=0;
    //virtual void Decode(uint8_t* frame, uint8_t length)=0;

    void SetCmdToDeviceHandler(ISe3ReceiveCommands* rxHandler)
    {
        _rxHandler=rxHandler;
    }

    void SetCmdFromDeviceHandler(ISe3TransmitCommands* txHandler)
    {
        _txHandler=txHandler;
    }

private:


    ISe3ReceiveCommands *_rxHandler;
    ISe3TransmitCommands *_txHandler;
};

#endif // IDATAPROVIDER_HPP
