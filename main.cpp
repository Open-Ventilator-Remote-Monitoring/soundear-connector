#include "se3device.hpp"
#include <unistd.h>
#include "Sockets/metersocket.hpp"
#include "Observers/observers.hpp"
#include "Helpers/helpers.hpp"
#include "Helpers/leq.hpp"
#include "Helpers/leds.hpp"
#include "common.hpp"

#include <string.h>
#include <sstream>
#include <cmath>
#include <csignal>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <memory>
#include <cstdio>
#include <chrono>
#include <unistd.h>
#include <sys/reboot.h>

#include "version.hpp"


#ifndef HOST_DEV
#include <systemd/sd-daemon.h>
#endif

using namespace std;

enum class ProgramState
{
    Init,
    NoSe3Device,
    NewSe3Device,
    Running,
    Error
};



class MainProgram: public IDeviceConnectedObserver, public IDeviceNameObserver, public IDeviceVersionObserver, public IMeasurementObserver
{

private:
ProgramState _state = ProgramState::Init;
ProgramState _oldState = ProgramState::Init;

SE3device _device;
Led _led;

bool _entry=false;
Helpers _helpers;

std::string _name="";
Se3::UsbProtocol::VersionInfo _versionInfo;
std::string _id="";
std::string _siteId="";
std::string _url="";
std::string _user="";
std::string _pass="";
std::string _apn="";

int _port;
bool lastConnection=false;
int stateCount=0;
int brokerConnectionAttempts=0;
int bridgeWatchdogDivider = 0;

public:

MainProgram(): _led("/sys/class/leds/led0")
{}

void ProgramStatemachine()
{

    switch (_state)
    {
    case ProgramState::Init:
        _led.Pattern(300,300);

        _device.Attach(dynamic_cast<IMeasurementObserver*>(this));
        _device.Attach(dynamic_cast<IDeviceNameObserver*>(this));
        _device.Attach(dynamic_cast<IDeviceVersionObserver*>(this));
        _device.Attach(dynamic_cast<IDeviceConnectedObserver*>(this));

        _state = ProgramState::NoSe3Device;
        sleep(2);
        break;
    //No device connected
    case ProgramState::NoSe3Device:        
        if (_device.Open())
        {            
            _id="";
            _name="";
            _siteId="";
            _state=ProgramState::NewSe3Device;
            _entry=true;
            stateCount=0;
        }
        else
        {
            sleep(1);
            stateCount++;
            if (stateCount>900)
            {
                std::cout<<"Device not responding 1. Rebooting";
                sync();
                reboot(RB_AUTOBOOT);
                std::cout<<"- failed"<<std::endl;
                exit(-1);
            }
        }
        break;
    //New Connection
    case ProgramState::NewSe3Device:
        if (_entry)
        {
            _led.Pattern(300,2000);
            sleep(2);

            _entry = false;            
        }
        else
        {
            sleep(1);

            if (_id=="")
            {
                _device.RequestVersion();
                _state=ProgramState::Running;
            }

            if (_name=="")
            {
                _device.RequestName();
                _state=ProgramState::Running;
            }

            stateCount++;
            if (stateCount>900)
            {
                std::cout<<"Device not responding 1. Rebooting";
                sync();
                reboot(RB_AUTOBOOT);
                std::cout<<"- failed"<<std::endl;
                exit(-1);
            }
        }
        break;

    //Connected and transferring measurements
    case ProgramState::Running:
       {
            if (_entry)
            {
                _led.Pattern(4000,1000);
                _entry=false;
            }

            sleep(1);


        }
        break;
    //Some error occurred
    case ProgramState::Error:
        _led.Pattern(200,200);
        _device.Close();
        sleep(2);
        if (_id=="" || _name=="")
        {
            if (_device.Open())
            {
                _device.ResetDevice();
            }
            usleep(500000);
            _device.Close();
        }
        sleep(5);

        std::cout<<"Rebooting from error state...";
        sync();
        reboot(RB_AUTOBOOT);
        std::cout<<"- failed"<<std::endl;
        exit(-1);
        break;
    }

    if (_oldState!=_state)
    {
        std::cout <<"\r\nState change, old: "<<(int)_oldState<< " new: "<<(int)_state <<std::endl;
    }
    _oldState = _state;


    {
        static int divider=0;
        if (divider++>30)
        {
            divider=0;
            //std::cout << (int)_state <<std::endl;
#ifndef HOST_DEV
            sd_notify(0, "WATCHDOG=1\n");
#endif

        }
    }
}

virtual void Update(bool connected)
{
    if (connected &&
       (_state==ProgramState::NoSe3Device || _state==ProgramState::Error))
    {
       _state = ProgramState::NewSe3Device;
    }
    else if (!connected)
    {
        sleep(5);
        _state = ProgramState::NoSe3Device;
    }
}

virtual void Update(std::string name)
{
    _name=name;
    std::cout<<"Name: "<<_name<<std::endl;
}





virtual void Update(const Se3::UsbProtocol::VersionInfo* info)
{
    memcpy(&_versionInfo,info, sizeof(Se3::UsbProtocol::VersionInfo));

    std::cout << std::endl << "Version: " << (int)_versionInfo.major <<"."<<(int)_versionInfo.minor <<"."<<(int)_versionInfo.build<<std::endl;
    std::cout << "Product: " << _versionInfo.product<<std::endl;
    std::cout << "Id: " <<_versionInfo.id[0]<<"-"<<_versionInfo.id[1]<<"-"<<_versionInfo.id[2]<<std::endl;

    std::ostringstream ss;
    int compactId = _versionInfo.id[0] ^ _versionInfo.id[1] ^ _versionInfo.id[2];
    ss << compactId;

    _id = ss.str();
}

void UpdateFast(const Se3::UsbProtocol::DeviceTime *deviceTime, float laf, float laeq1s, float las, float lcPeak, float lcf, float lcs) override
{
    (void) deviceTime;
    static int divider = 0;


    //All measurements arrive in deviBel and they are either A-weighted or C-weighted
    if (divider++>=8)
    {
        printf("LAF: %.1f, LAS: %.1f, LCF: %.1f LCS: %.1f, LAeq1s: %.1f, LCpeak: %.1f [deciBel]\r\n",laf, las, lcf, lcs, laeq1s, lcPeak);
        divider = 0;
    }
}

void UpdateSlow(float leq1min, float leq15min, float leq30min, uint8_t* peaks) override
{
    (void)leq1min;
    (void) leq15min;
    (void) leq30min;
    (void) peaks;
}

};


void signalHandler( int signum )
{
   std::cout << "Interrupt signal (" << signum << ") received.\n";

   // cleanup and close up stuff here
   // terminate program
   exit(signum);

}



int main()
{
    std::cout << "SoundEar Se3Connector version " << (int)defVersionMajor<<"."<<(int)defVersionMinor<<"."<<(int)defVersionBuild<<std::endl;
    #ifndef HOST_DEV
    sd_notify (0, "READY=1");
    #endif

    // register signal SIGINT and signal handler
    signal(SIGINT, signalHandler);

    MainProgram _mainPrg;

    while (true)
    {
        _mainPrg.ProgramStatemachine();
        usleep(100000);
    }

    return 0;
}
