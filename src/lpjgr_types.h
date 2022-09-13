#ifndef _LPGR_TYPES_H_
#define _LPGR_TYPES_H_

#include <RcppCommon.h>
#include "date.h"
// #include "guess.h"

namespace Rcpp {
	template<> SEXP wrap(const lpjgr_date&);
	template<> lpjgr_date as(SEXP);
}

#endif // _LPGR_TYPES_H_
