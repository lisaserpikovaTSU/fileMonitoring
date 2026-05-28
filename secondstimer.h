#ifndef SECONDSTIMER_H
#define SECONDSTIMER_H

#include "itimer.h"
#include <thread>
#include <chrono>

class SecondsTimer : public ITimer {
public:
    void sleepFor(int seconds) override {
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
    }
};

#endif // SECONDSTIMER_H
