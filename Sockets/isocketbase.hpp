#ifndef ISOCKETINTERFACE_HPP
#define ISOCKETINTERFACE_HPP
#include "iprotocolinterface.hpp"

class ISocketBase
{
public:
    ISocketBase(IProtocolInterface *prot, int socketId)
    {
        _prot=prot;
        _id=socketId;
    }

    virtual void Decode(uint8_t *frame, uint8_t length)=0;
protected:
    enum class OperationResult
    {
        Success,
        Error,
        Timeout,
        None
    };

    int _id;
    IProtocolInterface *_prot;
    OperationResult _lastOperation = OperationResult::None;
};

#endif // ISOCKETINTERFACE_HPP
