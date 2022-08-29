#ifndef _LPJGR_SIMULATION_STATE_H_
#define _LPJGR_SIMULATION_STATE_H_

#include "guess.h"
#include "inputmodule.h"

#include "simulate.h"

struct SimulationState {
    public:
		~SimulationState() {
			// For now, date is allocated on the runtime stack.
			// (It's just holding the address of the global date.)

			// These should not be NULL. But let's check anyway...
			if (grid_cell) {
				delete grid_cell;
			}
			if (input_module) {
				delete input_module;
			}
		}
		Gridcell* grid_cell;
		TemporalState temporal_state;
		Date* date;
		std::string instruction_file;
		InputModule* input_module;
		Stand* stand;
		Patch* patch;
};

#endif // _LPJGR_SIMULATION_STATE_H_
