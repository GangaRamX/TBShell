#include <iostream>
#include <cstdio>
#include <unistd.h>

#define LEN 5
class WaitTimer {
private:
    unsigned m_count;
    unsigned m_delay_msec;
    char m_str[LEN];
    void msleep (unsigned msec) {
        usleep(msec*1000);
    }
public:
    WaitTimer(unsigned delay_msec) : m_delay_msec(delay_msec), m_count(0) {
        strncpy(m_str, "|/-\\", LEN);
    }
    void wait() {
        printf("\r%c",m_str[m_count++ % (LEN-1)]);
        msleep(m_delay_msec);
    }
};
