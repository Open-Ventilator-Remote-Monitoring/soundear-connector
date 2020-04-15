#ifndef FILESOCKET_HPP
#define FILESOCKET_HPP
#include <stdint.h>
#include <string>
#include <mutex>
#include <vector>
#include "protocoldefs.hpp"
#include "isocketbase.hpp"
#include <condition_variable>
#include "Observers/observers.hpp"

class FileSocket: public ISocketBase
{
public:
    enum class OpenMode
    {
        CreateNew,      //Create new, fail if exists    (assume write)
        CreateAlways,   //Create new, if exists overwrite (assume write)
        OpenAlways,     //Open, if not exist create (assume write)
        Wr_OpenExisting,    //Open for write, fail if not exist
        Rd_OpenExisting     //Open for read, fail if not exist
    };

    FileSocket(IProtocolInterface *prot, int id);

    void Format();
    void Reset();

    void ReadFile(std::string filename, void (*CompleteCallback)(bool read, std::vector<uint8_t> filedata));

    void Attach(IDeviceFilereadObserver * observer);

    virtual void Decode(uint8_t *frame, uint8_t length);

private:
    enum class Properties : uint8_t
    {
        format = 0x0F,
        open = 0x10,
        close=0x11,
        write=0x12,
        read =0x13,
        size=0x14,
        reboot = 0x20,
        deepFormat=0x21,
        ok = 0x7F,
        error = 0x7E
    };

    enum class FResult
    {
        FR_OK = 0,				/* (0) Succeeded */
        FR_DISK_ERR,			/* (1) A hard error occurred in the low level disk I/O layer */
        FR_INT_ERR,				/* (2) Assertion failed */
        FR_NOT_READY,			/* (3) The physical drive cannot work */
        FR_NO_FILE,				/* (4) Could not find the file */
        FR_NO_PATH,				/* (5) Could not find the path */
        FR_INVALID_NAME,		/* (6) The path name format is invalid */
        FR_DENIED,				/* (7) Access denied due to prohibited access or directory full */
        FR_EXIST,				/* (8) Access denied due to prohibited access */
        FR_INVALID_OBJECT,		/* (9) The file/directory object is invalid */
        FR_WRITE_PROTECTED,		/* (10) The physical drive is write protected */
        FR_INVALID_DRIVE,		/* (11) The logical drive number is invalid */
        FR_NOT_ENABLED,			/* (12) The volume has no work area */
        FR_NO_FILESYSTEM,		/* (13) There is no valid FAT volume */
        FR_MKFS_ABORTED,		/* (14) The f_mkfs() aborted due to any parameter error */
        FR_TIMEOUT,				/* (15) Could not get a grant to access the volume within defined period */
        FR_LOCKED,				/* (16) The operation is rejected according to the file sharing policy */
        FR_NOT_ENOUGH_CORE,		/* (17) LFN working buffer could not be allocated */
        FR_TOO_MANY_OPEN_FILES,	/* (18) Number of open files > _FS_SHARE */
        FR_INVALID_PARAMETER	/* (19) Given parameter is invalid */
    };

    const uint8_t FA_READ = 0x01;
    const uint8_t FA_OPEN_EXISTING = 0x00;
    const uint8_t FA_WRITE = 0x02;
    const uint8_t FA_CREATE_NEW = 0x04;
    const uint8_t FA_CREATE_ALWAYS = 0x08;
    const uint8_t FA_OPEN_ALWAYS = 0x10;

    std::mutex _reqresp_mutex;
    std::condition_variable _responseCheck;
    bool _responseRxed=false;
    bool _responseTimeout=false;
    Se3::UsbProtocol::Se3Command _lastRxCmd;
    std::vector<IDeviceFilereadObserver*> _observerFileRead;

    void NotifyFileRead(bool success, std::vector<uint8_t> filedata);

    bool Open(std::string path, OpenMode openMode);
    std::vector<uint8_t> ReadSingleFrame();
    bool Close();
    bool BlockAwaitResponse();

    bool TransmitCommandBlocking(Se3::UsbProtocol::Se3Command* pCmd);

};

#endif // FILESOCKET_HPP
