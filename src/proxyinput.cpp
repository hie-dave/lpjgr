#include "proxyinput.h"
#include "guess.h"
#include "fluxnet.h"

ProxyInput::ProxyInput() {
    lat = 0;
    lon = 0;
    co2 = 0;
    tmin = 0;
    tmax = 0;
    prec = 0;
    insol = 0;
    ndep = 0;
    wind = 0;
    relhum = 0;
}

ProxyInput::ProxyInput(
    double lat,
    double lon,
    double co2,
    double tmin,
    double tmax,
    double prec,
    double insol,
    double ndep,
    double wind,
    double relhum) :
        lat(lat),
        lon(lon),
        co2(co2),
        tmin(tmin),
        tmax(tmax),
        prec(prec),
        insol(insol),
        ndep(ndep),
        wind(wind),
        relhum(relhum) {
}

// Destructor.
ProxyInput::~ProxyInput() {
    // Nothing to do in here for now.
}

void ProxyInput::init() {
    soil_input.init(param["file_soildata"].str);
	landcover_input.init();
	management_input.init();
}

bool ProxyInput::getgridcell(Gridcell& grid_cell) {
    grid_cell.set_coordinates(lon, lat);

    // todo: could make this configurable.
    grid_cell.climate.instype = SUNSHINE;

    // Set available water holding capacity.
    soil_input.get_soil(lon, lat, grid_cell);

    if (date.subdaily > 1) {
        throw std::runtime_error("tbi: subdaily timestep");
    }

	if (run_landcover) {
		bool LUerror = false;
		LUerror = landcover_input.loadlandcover(lon, lat);
		if (!LUerror)
			LUerror = management_input.loadmanagement(lon, lat);
		if (LUerror) {
			dprintf("\nError: could not find stand at (%g,%g) in landcover/management data file(s)\n", lon, lat);
			return false;
		}
	}

    return true;
}

bool ProxyInput::getclimate(Gridcell& grid_cell) {
    // Just use the stored values.
    grid_cell.climate.co2 = co2;
    grid_cell.climate.tmax = tmax;
    grid_cell.climate.tmin = tmin;
    grid_cell.climate.temp = (tmax + tmin) / 2;
    grid_cell.climate.prec = prec;
    grid_cell.climate.insol = insol;
    grid_cell.climate.dtr = tmax - tmin;
    grid_cell.climate.u10 = wind;
    grid_cell.climate.relhum = relhum;

    grid_cell.dNO3dep = ndep / 2;
    grid_cell.dNH4dep = ndep / 2;

    return true;
}

void ProxyInput::getlandcover(Gridcell& grid_cell) {
    // Proxy the request through to the landcover input module.
    landcover_input.getlandcover(grid_cell);
	landcover_input.get_land_transitions(grid_cell);
}

void ProxyInput::getmanagement(Gridcell& grid_cell) {
    // Proxy the request through to the management module.
    management_input.getmanagement(grid_cell);
}

void ProxyInput::set_lat(double lat) {
    this->lat = lat;
}

void ProxyInput::set_lon(double lon) {
    this->lon = lon;
}

void ProxyInput::set_co2(double co2) {
    this->co2 = co2;
}

void ProxyInput::set_tmin(double tmin) {
    this->tmin = tmin;
}

void ProxyInput::set_tmax(double tmax) {
    this->tmax = tmax;
}

void ProxyInput::set_prec(double prec) {
    this->prec = prec;
}

void ProxyInput::set_insol(double insol) {
    this->insol = insol;
}

void ProxyInput::set_ndep(double ndep) {
    this->ndep = ndep;
}

void ProxyInput::set_wind(double wind) {
    this->wind = wind;
}

void ProxyInput::set_relhum(double relhum) {
    this->relhum = relhum;
}
