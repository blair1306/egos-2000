#include "egos.h"

#define CLINT0_MTIME     0x200bff8
#define CLINT0_MTIMECMP  0x2004000

static long long mtime_get() {
    unsigned int time_lo, time_hi;
    /* Guard against rollover when reading mtime */
    do {
        time_hi = REGW(CLINT0_MTIME, 4);
        time_lo = REGW(CLINT0_MTIME, 0);
    } while (REGW(CLINT0_MTIME, 4) != time_hi);

    return (((unsigned long long)time_hi) << 32) | time_lo;
}

static void mtimecmp_set(long long time) {
    REGW(CLINT0_MTIMECMP, 4) = 0xFFFFFFFF;
    REGW(CLINT0_MTIMECMP, 0) = (unsigned int)time;
    REGW(CLINT0_MTIMECMP, 4) = (unsigned int)(time >> 32);
}

void timer_init()  {
    mtimecmp_set(0);
}

void timer_reset(int quantum) {
    mtimecmp_set(mtime_get() + quantum);
}
