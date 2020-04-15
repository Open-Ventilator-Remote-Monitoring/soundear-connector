#ifndef IPROTOCOLINTERFACE_HPP
#define IPROTOCOLINTERFACE_HPP
#include "protocoldefs.hpp"

class IProtocolInterface
{
public:
    /// <summary>
    /// Puts a command frame in the transmission queue
    /// </summary>
    /// <param name="cmdFrame"></param>
    /// <returns>Returns true if the command frame was put in the queue</returns>
    virtual bool SendFrame(Se3::UsbProtocol::Se3Command cmdFrame)=0;
};

#endif // IPROTOCOLINTERFACE_HPP
