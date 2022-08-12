#ifndef _LPJGR_INIT_H_
#define _LPJGR_INIT_H_

#include <string>

/*
Check if initialise() has been called. Throw if it has not.
*/
void ensure_initialised();

/*
Initialise the run. This **must** be called before canexch().

@param insFile:     Path to the .ins file.
@param lat:         Latitude of the gridcell.
@param lon:         Longitude of the gridcell.
@param co2:         Daily atmospheric ambient CO2 content (ppm).
@param tmin:        Daily minimum temperature (°C).
@param tmax:        Daily maximum temperature (°C).
@param prec:        Daily precipitation (mm).
@param insol:       Daily insolation (units?).
@param ndep:        Daily total annual N deposition (units?).
@param wind:        Daily 10m wind speed (km/h).
@param relhum:      Daily relative humidity (fraction).
*/
void initialise(std::string insFile,
    double lat,
    double lon,
    double co2,
    double tmin,
    double tmax,
    double prec,
    double insol,
    double ndep,
    double wind,
    double relhum);

/*
Set our shell for the model to communicate with the world. This only
needs to be done once, and I'm not sure of the effects of doing it
multiple times. Failure to do this will result in a segfault.

This will be automatically called by initialise(), so it shouldn't normally be
called manually.
*/
void initialise_shell();

#endif // _LPJGR_INIT_H_
