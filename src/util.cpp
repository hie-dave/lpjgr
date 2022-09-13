#include <string>
#include <vector>
#include <Rcpp.h>

#include "date.h"
#include "guess.h"
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

lpjgr_date create_date(const Date* date) {
	return lpjgr_date(date->get_calendar_year(), date->month, date->dayofmonth);
}
