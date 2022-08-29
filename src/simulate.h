#ifndef _LPJGR_SIMULATE_H_
#define _LPJGR_SIMULATE_H_

typedef enum {
    // At start of day
    START_OF_DAY,
    // Ready to call canopy_exchange()
    BEFORE_CANEXCH,
    // After canopy_exchange() has been called
    AFTER_CANEXCH,
    // At end of day
    END_OF_DAY
} TemporalState;

/*
Simulate the day until before canopy_exchange(). This must be called in the
appropriate sequence (ie directly after post_canexch, but not multiple times
in a row, and not after canexch()).
*/
void pre_canexch();

/*
Run canopy exchange. May be called multiple times per day.
*/
void canexch();

/*
Simulate the rest of the day after canopy_exchange(). This must be called in
the appropriate sequence (ie after call(s) to canexch()).
*/
void post_canexch();

#endif // _LPJGR_SIMULATE_H_
