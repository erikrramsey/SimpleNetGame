#include "Clock.h"
#include <iostream>

namespace sng {
long long Clock::m_currentTimeMillis;

Clock::Clock() {
    m_currentTimeMillis = GetCurrentTimeMicro();
}

long long Clock::getDTMicro() {
    long long TimeNowMillis = GetCurrentTimeMicro();
    long long delta = TimeNowMillis - m_currentTimeMillis;
    m_currentTimeMillis = TimeNowMillis;
    return delta;
}

long long Clock::GetCurrentTimeMicro() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
}

void Clock::sleepForMicro(long long ms) {
    std::cout << "sleeping for: " << ms << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds (ms));
}
}
