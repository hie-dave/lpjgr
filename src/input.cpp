#include "input.h"
#include "state.h"

// [[Rcpp::export]]
void set_lat(double lat) {
    grid_cell->climate.lat = lat;
}

// [[Rcpp::export]]
void set_lon(double lon) {
    // grid_cell->climate.lon = lon;
    throw std::runtime_error("tbi");
}

// [[Rcpp::export]]
void set_co2(double co2) {
    grid_cell->climate.co2 = co2;
}

void update_temp() {
    grid_cell->climate.temp = (grid_cell->climate.tmax + grid_cell->climate.tmin) / 2.0;
}

// [[Rcpp::export]]
void set_tmin(double tmin) {
    grid_cell->climate.tmin = tmin;
    update_temp();
}

// [[Rcpp::export]]
void set_tmax(double tmax) {
    grid_cell->climate.tmax = tmax;
    update_temp();
}

// [[Rcpp::export]]
void set_prec(double prec) {
    grid_cell->climate.prec = prec;
}

// [[Rcpp::export]]
void set_insol(double insol) {
    grid_cell->climate.insol = insol;
}

// [[Rcpp::export]]
void set_ndep(double ndep) {
    grid_cell->dNO3dep = ndep / 2;
    grid_cell->dNH4dep = ndep / 2;
}

// [[Rcpp::export]]
void set_wind(double wind) {
    grid_cell->climate.u10 = wind;
}

// [[Rcpp::export]]
void set_relhum(double relhum) {
    grid_cell->climate.relhum = relhum;
}
