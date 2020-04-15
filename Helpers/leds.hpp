#ifndef LEDS_HPP
#define LEDS_HPP

#include <string>
#include <thread>

class Led
{
public:
    Led(std::string path);
    ~Led();

    void Pattern(int onMs, int offMs);
private:
    void Toggle();
    void On();
    void Off();

    void SetState(int state);
    void LedStatemachine();
    int _state=0;
    int _onTimeMs=0;
    int _offTimeMs=0;
    std::string _path;
    std::thread* _threadLed;
    bool _terminate=false;
};

#endif // LEDS_HPP
