#ifndef _LPJGR_INPUT_H_
#define _LPJGR_INPUT_H_

/*
Change the hardcoded latitude.
@param lat: The new latitude.
*/
void set_lat(double lat);

/*
Change the hardcoded longitude.
@param lon: The new longitude.
*/
void set_lon(double lon);

/*
Change the hardcoded co2 value.
@param co2: The new co2 value (ppm?).
*/
void set_co2(double co2);

/*
Change the hardcoded tmin value.
@param tmin: The new tmin value (°C).
*/
void set_tmin(double tmin);

/*
Change the hardcoded tmax value.
@param tmax: The new tmax value (°C).
*/
void set_tmax(double tmax);

/*
Change the hardcoded precipitation value.
@param prec: The new precipitation value (units?).
*/
void set_prec(double prec);

/*
Change the hardcoded insolation value.
@param insol: The new insolation value (% sunshine).
*/
void set_insol(double insol);

/*
Change the hardcoded N deposition value.
@param ndep: The new N deposition value (units?).
*/
void set_ndep(double ndep);

/*
Change the hardcoded wind value.
@param wind: The new wind value (10m wind in km/h).
*/
void set_wind(double wind);

/*
Change the hardcoded relative humidity value.
@param relhum: The new relative humidity value (fraction).
*/
void set_relhum(double relhum);

#endif // _LPJGR_INPUT_H_
