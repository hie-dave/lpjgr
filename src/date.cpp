#include <stdint.h>

// lpj-guess
#include "guess.h"

// lpjgr
#include "date.h"

// Return the number of days in a given month.
int days_in_month(int m) {
	if (m < 0 || m > 11) {
		// Should never happen, this will be caught earlier.
		throw std::runtime_error("Invalid month");
	}

	return date.ndaymonth[m];
}

lpjgr_date::lpjgr_date(int y, int m, int d) {
	if (y < 0) {
		char buf[32];
		sprintf(buf, "Invalid year: %d", y);
		throw std::runtime_error(buf);
	}

	if (m < 0 || m > 11) {
		char buf[64];
		sprintf(buf, "Invalid month: %d. Should be in range [0, 11]", m);
		throw std::runtime_error(buf);
	}

	int max_days = days_in_month(m);
	if (d < 0 || d > max_days) {
		char buf[64];
		sprintf(buf, "Invalid day: %d. Should be in range [0, %d]", d, max_days);
		throw std::runtime_error(buf);
	}

	year = y;
	month = m;
	day = d;
}

uint16_t lpjgr_date::get_year() const {
	return year;
}

uint16_t lpjgr_date::get_month() const {
	return month;
}

uint16_t lpjgr_date::get_day() const {
	return day;
}

lpjgr_date::compare_result lpjgr_date::compare(const lpjgr_date* other) const {
	uint16_t other_year = other->get_year();
	if (year < other_year) {
		return BEFORE;
	}

	if (year > other_year) {
		return AFTER;
	}

	// Dates are in same year.

	uint16_t other_month = other->get_month();

	if (month < other_month) {
		return BEFORE;
	}

	if (month > other_month) {
		return AFTER;
	}

	// Dates are in same month and year.

	uint16_t other_day = other->get_day();

	if (day < other_day) {
		return BEFORE;
	}

	if (day > other_day) {
		return AFTER;
	}

	// Same day.
	return EQUAL;
}

bool lpjgr_date::is_after(const lpjgr_date* other) const {
	return compare(other) == AFTER;
}

bool lpjgr_date::is_before(const lpjgr_date* other) const {
	return compare(other) == BEFORE;
}

bool lpjgr_date::is_equal(const lpjgr_date* other) const {
	return compare(other) == EQUAL;
}
