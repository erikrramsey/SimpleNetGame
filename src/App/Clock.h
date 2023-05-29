#ifndef SIMPLENETGAME_CLOCK_H
#define SIMPLENETGAME_CLOCK_H

#include <ctime>
#include <chrono>
#include <thread>

namespace sng {

class Clock {
public:
    Clock();
    static long long getDTMicro();
    static long long GetCurrentTimeMicro();
    static void sleepForMicro(long long ms);
private:
    static long long m_currentTimeMillis;
};

}

#endif //SIMPLENETGAME_CLOCK_H
