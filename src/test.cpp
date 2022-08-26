#include "canexch.h"

//'
//' This is mainly a test function to verify that lpj-guess linkage has
//' worked correctly.
//'
//' @export
//'
// [[Rcpp::export]]
double lambert_beer(double x) {
    return lambertbeer(x);
}
