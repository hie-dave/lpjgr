#ifndef _LPJGR_SIMULATE_H_
#define _LPJGR_SIMULATE_H_

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
