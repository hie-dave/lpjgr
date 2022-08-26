#include "input.h"
#include "state.h"

//'
//' Set the lat value.
//'
//' @description
//'
//' Note that this value will be overwritten when the next day's met
//' data is read in (ie when running simulation functions such as
//' [run_simulation] or [simulate_until_tree_establishment]).
//' ([canexch] will never overwrite this value.)
//'
//' This is a temporary input function which will be removed in a
//' future version when I rework the input infrastructure.
//'
//' @export
// [[Rcpp::export]]
void set_lat(double lat) {
    grid_cell->climate.lat = lat;
}

//'
//' Set the lon value.
//'
//' @description
//'
//' Note that this value will be overwritten when the next day's met
//' data is read in (ie when running simulation functions such as
//' [run_simulation] or [simulate_until_tree_establishment]).
//' ([canexch] will never overwrite this value.)
//'
//' This is a temporary input function which will be removed in a
//' future version when I rework the input infrastructure.
//'
//' @export
// [[Rcpp::export]]
void set_lon(double lon) {
    // grid_cell->climate.lon = lon;
    throw std::runtime_error("tbi");
}

//'
//' Set the co2 value.
//'
//' @description
//'
//' Note that this value will be overwritten when the next day's met
//' data is read in (ie when running simulation functions such as
//' [run_simulation] or [simulate_until_tree_establishment]).
//' ([canexch] will never overwrite this value.)
//'
//' This is a temporary input function which will be removed in a
//' future version when I rework the input infrastructure.
//'
//' @export
// [[Rcpp::export]]
void set_co2(double co2) {
    grid_cell->climate.co2 = co2;
}

void update_temp() {
    grid_cell->climate.temp = (grid_cell->climate.tmax + grid_cell->climate.tmin) / 2.0;
}

//'
//' Set the tmin value.
//'
//' @description
//'
//' Note that this value will be overwritten when the next day's met
//' data is read in (ie when running simulation functions such as
//' [run_simulation] or [simulate_until_tree_establishment]).
//' ([canexch] will never overwrite this value.)
//'
//' This is a temporary input function which will be removed in a
//' future version when I rework the input infrastructure.
//'
//' @export
// [[Rcpp::export]]
void set_tmin(double tmin) {
    grid_cell->climate.tmin = tmin;
    update_temp();
}

//'
//' Set the tmax value.
//'
//' @description
//'
//' Note that this value will be overwritten when the next day's met
//' data is read in (ie when running simulation functions such as
//' [run_simulation] or [simulate_until_tree_establishment]).
//' ([canexch] will never overwrite this value.)
//'
//' This is a temporary input function which will be removed in a
//' future version when I rework the input infrastructure.
//'
//' @export
// [[Rcpp::export]]
void set_tmax(double tmax) {
    grid_cell->climate.tmax = tmax;
    update_temp();
}

//'
//' Set the prec value.
//'
//' @description
//'
//' Note that this value will be overwritten when the next day's met
//' data is read in (ie when running simulation functions such as
//' [run_simulation] or [simulate_until_tree_establishment]).
//' ([canexch] will never overwrite this value.)
//'
//' This is a temporary input function which will be removed in a
//' future version when I rework the input infrastructure.
//'
//' @export
// [[Rcpp::export]]
void set_prec(double prec) {
    grid_cell->climate.prec = prec;
}

//'
//' Set the insol value.
//'
//' @description
//'
//' Note that this value will be overwritten when the next day's met
//' data is read in (ie when running simulation functions such as
//' [run_simulation] or [simulate_until_tree_establishment]).
//' ([canexch] will never overwrite this value.)
//'
//' This is a temporary input function which will be removed in a
//' future version when I rework the input infrastructure.
//'
//' @export
// [[Rcpp::export]]
void set_insol(double insol) {
    grid_cell->climate.insol = insol;
}

//'
//' Set the ndep value.
//'
//' @description
//'
//' Note that this value will be overwritten when the next day's met
//' data is read in (ie when running simulation functions such as
//' [run_simulation] or [simulate_until_tree_establishment]).
//' ([canexch] will never overwrite this value.)
//'
//' This is a temporary input function which will be removed in a
//' future version when I rework the input infrastructure.
//'
//' @export
// [[Rcpp::export]]
void set_ndep(double ndep) {
    grid_cell->dNO3dep = ndep / 2;
    grid_cell->dNH4dep = ndep / 2;
}

//'
//' Set the wind value.
//'
//' @description
//'
//' Note that this value will be overwritten when the next day's met
//' data is read in (ie when running simulation functions such as
//' [run_simulation] or [simulate_until_tree_establishment]).
//' ([canexch] will never overwrite this value.)
//'
//' This is a temporary input function which will be removed in a
//' future version when I rework the input infrastructure.
//'
//' @export
// [[Rcpp::export]]
void set_wind(double wind) {
    grid_cell->climate.u10 = wind;
}

//'
//' Set the relhum value.
//'
//' @description
//'
//' Note that this value will be overwritten when the next day's met
//' data is read in (ie when running simulation functions such as
//' [run_simulation] or [simulate_until_tree_establishment]).
//' ([canexch] will never overwrite this value.)
//'
//' This is a temporary input function which will be removed in a
//' future version when I rework the input infrastructure.
//'
//' @export
// [[Rcpp::export]]
void set_relhum(double relhum) {
    grid_cell->climate.relhum = relhum;
}
