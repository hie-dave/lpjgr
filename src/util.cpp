#include <string>
#include <vector>
#include <Rcpp.h>

#include "util.h"

/*
Convert a vector to an Rcpp vector (string vectors only).
*/
Rcpp::CharacterVector vec_to_rvec(std::vector<std::string> vec) {
    Rcpp::CharacterVector rvec(vec.size());
    for (std::size_t i = 0; i < vec.size(); i++) {
        rvec[i] = vec[i];
    }
    return rvec;
}
