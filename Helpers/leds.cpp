#include "leds.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>

Led::Led(std::string path): _path(path)
{   
    path.append("/trigger");
    std::ofstream ledfile;
    ledfile.open (path);
    std::cout << "Setting up led...";
    if (ledfile.is_open())
    {
        ledfile << "none";
        ledfile.close();
         std::cout << " done" << std::endl;
    }
    else
    {
        std::cout << " fail" << std::endl;
    }

    _onTimeMs=1000;
    _offTimeMs=1000;

    //Create a thread in this context
    _threadLed = new std::thread([=]()
    {
        LedStatemachine();
    });
    _threadLed->detach();
}

Led::~Led()
{
    _terminate=true;
    _threadLed->join();
}

void Led::Toggle()
{
    if (_state==0)
        _state=1;
    else
        _state=0;
    SetState(_state);
}

void Led::On()
{
    _state=1;
    SetState(_state);
}

void Led::Off()
{
    _state=0;
    SetState(_state);
}

void Led::Pattern(int onMs, int offMs)
{
    _onTimeMs = onMs;
    _offTimeMs = offMs;
}



void Led::SetState(int state)
{
    std::string s(_path);
    std::ofstream ledfile;
    ledfile.open (s.append("/brightness"));
    if (ledfile.is_open())
    {
        ledfile << state;
        ledfile.close();
    }
}

void Led::LedStatemachine()
{
    while (!_terminate)
    {
        switch(_state)
        {
            //Off
            case 0:
            Off();
            usleep(1000*_offTimeMs);
            _state=1;
            break;
            case 1:
            On();
            usleep(1000*_onTimeMs);
            _state=0;
            break;
        }
    }
}
