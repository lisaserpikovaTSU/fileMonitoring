#ifndef ITIMER_H
#define ITIMER_H

class ITimer {
public:
    virtual ~ITimer() = default;
    virtual void sleepFor(int seconds) = 0;
};

#endif // ITIMER_H
