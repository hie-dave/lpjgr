#ifndef _LPJGR_DATE_H_
#define _LPJGR_DATE_H_

#include <stdint.h>

// #include "guess.h"

class lpjgr_date {
	public:
		/*
		Create a new date object for the specified year/month/day.
		Will throw for invalid values.
		@param y: Year (1-x).
		@param m: Month (0-11).
		@param d: Day of month (0-30 ish, depending on month).
		*/
		lpjgr_date(int y, int m, int d);

		// // Create a new date object for the specified lpj-guess date.
		// lpjgr_date(const Date* date);

		// Get the (calendar) year represented by this date object.
		uint16_t get_year() const;

		// Get the month of year (0-11).
		uint16_t get_month() const;

		// Get the day of month (0-30 ish, dependent on month).
		uint16_t get_day() const;

		// Check if this date is after another date.
		// @param other: The other date.
		bool is_after(const lpjgr_date* other) const;

		// Check the this date is before another date.
		// @param other: The other date.
		bool is_before(const lpjgr_date* other) const;

		// Check if this date represents the same day as another date.
		// @param other: The other date.
		bool is_equal(const lpjgr_date* other) const;
	private:
		// Gregorian year (BCE is a myth).
		uint16_t year;

		// Month of year (0-11).
		// Although this is an int, the constructors will not allow
		// a negative value to go in here.
		uint16_t month;

		// Day of month (0-30 ish, dependent on month).
		// Although this is an int, the constructors will not allow
		// a negative value to go in here.
		uint16_t day;

		// A date comparison result.
		enum compare_result {
			BEFORE,
			EQUAL,
			AFTER
		};

		// Compare this date to another date.
		compare_result compare(const lpjgr_date* other) const;
};

#endif // _LPJGR_DATE_H_
