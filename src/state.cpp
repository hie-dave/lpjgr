#include "inputmodule.h"
#include "state.h"
#include "guess.h"
#include "outputmodule.h"

// Global state variables used by other LPJGR modules.

// The grid cell which has been configured by initialise().
Gridcell* grid_cell;

// The first stand in the grid cell.
Stand* stand;

// The first patch in the stand.
Patch* patch;

// The input module.
InputModule* input_module;

// The output modules.
GuessOutput::OutputModuleContainer* output_modules;
