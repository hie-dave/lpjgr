#ifndef _PROXY_INPUT_H_
#define _PROXY_INPUT_H_

#include "inputmodule.h"
#include "soilinput.h"
#include "fluxnet.h"

class ProxyInput : public InputModule {
    public:
        /*
        Default constructor.
        */
        ProxyInput();

        /*
        Another constructor to initialise the object.
        @param lat: Latitude of the gridcell.
        @param lon: Longitude of the gridcell.
        @param co2: Daily atmospheric ambient CO2 content (ppm).
        @param tmin: Daily minimum temperature (°C).
        @param tmax: Daily maximum temperature (°C).
        @param prec: Daily precipitation (mm).
        @param insol: Daily insolation (units?).
        @param ndep: Daily total annual N deposition (units?).
        @param wind: Daily 10m wind speed (km/h).
        @param relhum: Daily relative humidity (fraction).
        */
        ProxyInput(
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

        // Destructor.
        ~ProxyInput();

        /// See base class for documentation about this function's responsibilities
        void init();

        /// See base class for documentation about this function's responsibilities
        bool getgridcell(Gridcell&);

        /// See base class for documentation about this function's responsibilities
        bool getclimate(Gridcell&);
        
        /// See base class for documentation about this function's responsibilities
        void getlandcover(Gridcell&);

        /// See base class for documentation about this function's responsibilities
        void getmanagement(Gridcell&);

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

    private:
        // Latitude of the gridcell.
        double lat;

        // Longitude of the gridcell.
        double lon;

        // Daily atmospheric ambient CO2 content (ppm).
        double co2;

        // Daily minimum temperature (°C).
        double tmin;

        // Daily maximum temperature (°C).
        double tmax;

        // Daily precipitation (mm).
        double prec;

        // Daily insolation (units?).
        double insol;

        // Daily total annual N deposition (units?).
        double ndep;

        // Daily 10m wind speed (km/h).
        double wind;

        // Daily relative humidity (fraction).
        double relhum;

        // Soil input module used to parse soil data.
        SoilInput soil_input;

        // Land cover input module
        LandcoverInput landcover_input;

        // Management input module
        ManagementInput management_input;
};

#endif // _PROXY_INPUT_H_
