#include <RcppCommon.h>

// This must be before Rcpp.h
#include "lpjgr_types.h"

#include <Rcpp.h>
#include <ostream>
#include <stdio.h>

#include "date.h"
#include "guess.h"
#include "init.h"
#include "state.h"

namespace Rcpp {
	template<>
	SEXP wrap(const lpjgr_date& date) {
		// This will wrap the date in an Rcpp Datetime object.
		// std::ostringstream buf;
		// buf << date.get_year() << "-" << date.get_month() << "-" << date.get_day();
		// std::string str(buf.str());
		// Datetime dt(str, "%Y-%m-%d");

		// This will wrap the date in an R List.
		// List list;
		// list["year"] = wrap(date.get_year());
		// list["month"] = wrap(date.get_month());
		// list["day"] = wrap(date.get_day());
		// return list;

		// For now, let's just use a yyyy-MM-dd string.
		// std::ostringstream oss;
		// oss << date.get_year() << "-" << date.get_month() << "-" << date.get_day();

		char buf[10];

		// Remember, month and day are 0-indexed.
		std::sprintf(buf, "%hd-%hd-%hd", date.get_year(), date.get_month() + 1, date.get_day() + 1);
		return wrap(buf);
	}

	template<>
	lpjgr_date as(SEXP ptr) {
		// This will extra the date encapsulated as a list.
		// List list(ptr);
		// int year = list["year"];
		// int month = list["month"];
		// int day = list["day"];
		// lpjgr_date date(year, month, day);
		String str(ptr);

		int year, month, day;
		sscanf(str.get_cstring(), "%d-%d-%d", &year, &month, &day);

		// Remember, month and day are 0-indexed inside the object.
		lpjgr_date date(year, month - 1, day - 1);
		return date;
	}
}
