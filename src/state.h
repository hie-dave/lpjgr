#ifndef _LPJGR_STATE_H_
#define _LPJGR_STATE_H_

#include "guess.h"

// Global state variables used by other LPJGR modules.

// The grid cell which has been configured by initialise().
extern Gridcell* grid_cell;

// The first stand in the grid cell.
extern Stand* stand;

// The first patch in the stand.
extern Patch* patch;

#endif // _LPJGR_STATE_H_
