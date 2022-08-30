#include <memory>
#include <sstream>

#include "init.h"
#include "in_memory_save_state.h"
#include "model_access.h"
#include "state.h"

#include "guessserializer.h"

int is_empty(std::stringstream& buf) {
	buf.seekg(0, std::ios::end);
	int size = buf.tellg();
	buf.seekg(0, std::ios::beg);
	return size == 0;
}

void InMemorySaveState::save() {
	// Quick sanity check - this class supports multiple load() calls,
	// but only one save(). If you want to make another save state,
	// create another instance.
	if (!( is_empty(state_buf) && is_empty(meta_buf) )) {
		throw std::runtime_error("Programmer error: cannot reuse InMemorySaveState instance for multiple saves");
	}

	// Serialise the gridcell (to memory - not to disk).
	GuessSerializer serializer(&state_buf, &meta_buf, 0, 1);
	serializer.serialize_gridcell(*state->grid_cell);

	// Store some other metadata which will be needed later.
	year = state->date->year;
	doy = state->date->day;
	ins_file_name = state->instruction_file;
}

void InMemorySaveState::Load(SimulationState* new_state) {
	// Seek to start of streams (remember, load can be called multiple
	// times).
	state_buf.seekg(0, std::ios::beg);
	meta_buf.seekg(0, std::ios::beg);

	// Sadly, we need to do a full re-initialisation, to allow for the
	// fact that the user could have called initialise() between saving
	// and loading the state (initialise() frustratingly changes a ton
	// of global lpj-guess variables).
	if (state->instruction_file != ins_file_name) {
		initialise_state(ins_file_name, new_state);
	} else {
		new_state->grid_cell = state->grid_cell;
		new_state->date = &date;
		new_state->stand = get_stand(new_state->grid_cell);
		new_state->patch = get_patch(new_state->stand);
		new_state->input_module = state->input_module;
		new_state->instruction_file = state->instruction_file;
	}

	// The gridcell object was initialised by the previous call, so we
	// can now deserialize our previous state into the gridcell.
	GuessDeserializer deserializer(&state_buf, &meta_buf);
	deserializer.deserialize_gridcell(*new_state->grid_cell);

	// Set current simulation date.
	new_state->date->init(1);
	initialise_date(new_state->date, year, doy);
}
