#include "filesocket.hpp"
#include <memory>
#include <thread>
#include <iostream>
#include <chrono>
#include <cstring>
#include <unistd.h>

FileSocket::FileSocket(IProtocolInterface *prot, int id): ISocketBase(prot,id)
{

}

void FileSocket::Reset()
{
    Se3::UsbProtocol::Se3Command cmd;
    std::cout<<"reset" << std::endl;
    cmd.socket = (uint8_t)_id;
    cmd.property = (uint8_t)Properties::reboot | ((uint8_t)Se3::UsbProtocol::Commands::set << 7);
    cmd.dataLength = 0;    //add \0 terminator and openMode

    _prot->SendFrame(cmd);
}

bool FileSocket::Open(std::string path, OpenMode openMode)
{
    std::cout<<"\r\nRead " << path << std::endl;
    if (path.length()>= 60 - 2)
        return false;
    Se3::UsbProtocol::Se3Command cmdFrame;
    uint8_t mode = FA_OPEN_EXISTING | FA_READ;

    switch (openMode)
    {
        case OpenMode::CreateAlways:
            mode = FA_CREATE_ALWAYS | FA_WRITE;
            break;
        case OpenMode::CreateNew:
            mode = FA_CREATE_NEW | FA_WRITE;
            break;
        case OpenMode::OpenAlways:
            mode = FA_OPEN_ALWAYS | FA_WRITE;
            break;
        case OpenMode::Rd_OpenExisting:
            mode = FA_OPEN_EXISTING | FA_READ;
            break;
        case OpenMode::Wr_OpenExisting:
            mode = FA_OPEN_EXISTING | FA_WRITE;
            break;
    }
    cmdFrame.socket = (uint8_t)_id;
    cmdFrame.property = (uint8_t)Properties::open | ((uint8_t)Se3::UsbProtocol::Commands::set << 7);
    cmdFrame.dataLength = (uint8_t)(path.length() + 2);    //add \0 terminator and openMode

    std::copy(path.begin(),path.end(),cmdFrame.data);

    int index = path.length();

    cmdFrame.data[index++] = 0;
    cmdFrame.data[index] = mode;

    return TransmitCommandBlocking(&cmdFrame);
}

std::vector<uint8_t> FileSocket::ReadSingleFrame()
{
    Se3::UsbProtocol::Se3Command cmd;
    std::vector<uint8_t> rxData;

    cmd.socket = (uint8_t)_id;
    cmd.property = (uint8_t)Properties::read | ((uint8_t)Se3::UsbProtocol::Commands::set << 7);
    cmd.dataLength = 0;

    if (TransmitCommandBlocking(&cmd))
    {
        for (int i=0;i<_lastRxCmd.dataLength;i++)
        {
            rxData.push_back(_lastRxCmd.data[i]);
        }
    }
    return rxData;
}

bool FileSocket::Close()
{
    Se3::UsbProtocol::Se3Command cmd;
    std::cout<<"close" << std::endl;
    cmd.socket = (uint8_t)_id;
    cmd.property = (uint8_t)Properties::close | ((uint8_t)Se3::UsbProtocol::Commands::set << 7);
    cmd.dataLength = 0;    //add \0 terminator and openMode

    return TransmitCommandBlocking(&cmd);
}

void FileSocket::Decode(uint8_t *frame, uint8_t length)
{
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
            case Properties::ok:
            {
                std::unique_lock<std::mutex> locker(_reqresp_mutex);
                _lastOperation = OperationResult::Success;
                _responseRxed = true;
                _responseCheck.notify_one();
            }
            std::cout << "." << std::endl;
            break;

            case Properties::error:
            {
                std::unique_lock<std::mutex> locker(_reqresp_mutex);
                _lastOperation = OperationResult::Error;
                _responseRxed = true;
                _responseCheck.notify_one();
            }
            std::cout << "\r\nFRES: " << (int)cmd.data[0]<< std::endl;
            break;

            case Properties::read:
            {
                std::unique_lock<std::mutex> locker(_reqresp_mutex);
                std::memcpy(&_lastRxCmd,  &cmd, sizeof(Se3::UsbProtocol::Se3Command) );
                _responseRxed = true;
                _responseCheck.notify_one();
                std::cout << ".";
            }
            break;
            case Properties::size:
            {
                std::unique_lock<std::mutex> locker(_reqresp_mutex);
                _lastOperation = OperationResult::Success;
                _responseRxed = true;
                _responseCheck.notify_one();
            }

            break;
            default:
            break;
        }
    }

}

void FileSocket::NotifyFileRead(bool success, std::vector<uint8_t> filedata)
{
    for(std::vector<IDeviceFilereadObserver*>::const_iterator iter = _observerFileRead.begin(); iter != _observerFileRead.end(); ++iter)
    {
        if(*iter != 0)
        {
            (*iter)->Update (success, filedata);
        }
    }
}

void FileSocket::ReadFile(std::string filename, void (*CompleteCallback)(bool read, std::vector<uint8_t> filedata))
{
    //Create a thread in this context
    std::thread threadRead([=]()
    {
        std::cout << "Reading file: " << filename <<std::endl;

        std::vector<uint8_t> filedata;
        bool success=false;

        Close();

        if (Open(filename,OpenMode::Rd_OpenExisting))
        {
            int read=0;
            do
            {
                std::vector<uint8_t> rxdata = ReadSingleFrame();
                if (rxdata.size()!=0)
                {
                    filedata.insert(filedata.end(), rxdata.begin(), rxdata.end());
                }

                if (rxdata.size() != Se3::UsbProtocol::maxDataLength)
                {
                    //Not a full data frame - hence the file must be read
                    Close();
                    success=true;
                }

                read=rxdata.size();
            } while (read==Se3::UsbProtocol::maxDataLength);
        }
        else
        {
            std::cout << "Failed opening file"<<std::endl;
        }
        if (CompleteCallback!=nullptr)
        {
            CompleteCallback(success,filedata);
        }

        //Notify also all observers
        NotifyFileRead(success,filedata);
    });

    threadRead.detach();
}

void FileSocket::Attach(IDeviceFilereadObserver *observer)
{
    _observerFileRead.push_back(observer);
}

bool FileSocket::BlockAwaitResponse()
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

bool FileSocket::TransmitCommandBlocking(Se3::UsbProtocol::Se3Command *pCmd)
{
    _responseRxed=false;
    _responseTimeout=false;

    _prot->SendFrame(*pCmd);

    return BlockAwaitResponse();
}
