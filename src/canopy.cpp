// lpjgr
#include "init.h"
#include "state.h"

// lpj-g
#include "canexch.h"

// [[Rcpp::export]]
double lambert_beer(double x) {
    return lambertbeer(x);
}

// [[Rcpp::export]]
void canexch() {
    ensure_initialised();
    canopy_exchange(*patch, grid_cell->climate);
}
