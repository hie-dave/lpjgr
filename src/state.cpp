#include "proxyinput.h"
#include "state.h"
#include "guess.h"

// Global state variables used by other LPJGR modules.

// The grid cell which has been configured by initialise().
Gridcell* grid_cell;

// The first stand in the grid cell.
Stand* stand;

// The first patch in the stand.
Patch* patch;

// The input module.
ProxyInput* input_module;
