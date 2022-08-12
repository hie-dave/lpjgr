#include "input.h"
#include "proxyinput.h"
#include "state.h"

// [[Rcpp::export]]
void set_lat(double lat) {
    input_module->set_lat(lat);
}

// [[Rcpp::export]]
void set_lon(double lon) {
    input_module->set_lon(lon);
}

// [[Rcpp::export]]
void set_co2(double co2) {
    input_module->set_co2(co2);
}

// [[Rcpp::export]]
void set_tmin(double tmin) {
    input_module->set_tmin(tmin);
}

// [[Rcpp::export]]
void set_tmax(double tmax) {
    input_module->set_tmax(tmax);
}

// [[Rcpp::export]]
void set_prec(double prec) {
    input_module->set_prec(prec);
}

// [[Rcpp::export]]
void set_insol(double insol) {
    input_module->set_insol(insol);
}

// [[Rcpp::export]]
void set_ndep(double ndep) {
    input_module->set_ndep(ndep);
}

// [[Rcpp::export]]
void set_wind(double wind) {
    input_module->set_wind(wind);
}

// [[Rcpp::export]]
void set_relhum(double relhum) {
    input_module->set_relhum(relhum);
}
