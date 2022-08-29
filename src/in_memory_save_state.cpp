#include <memory>

#include "in_memory_save_state.h"
#include "state.h"

void InMemorySaveState::save() {
	saved_state = state.get();
}

void InMemorySaveState::Load(SimulationState* state) const {
	state->grid_cell = saved_state->grid_cell;
	state->date = saved_state->date;
	state->instruction_file = saved_state->instruction_file;
	state->input_module = saved_state->input_module;
	state->stand = saved_state->stand;
	state->patch = saved_state->patch;
}
