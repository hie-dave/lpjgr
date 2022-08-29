#include <memory>

#include "disk_bound_save_state.h"
#include "inputmodule.h"
#include "state.h"
#include "guess.h"
#include "outputmodule.h"
#include "state.h"
#include "isave_state.h"
#include "in_memory_save_state.h"

#include "guessserializer.h"

// Global state variables used by other LPJGR modules.

// Global state of the current simulation.
std::shared_ptr<SimulationState> state;

std::vector<ISaveState*> save_states;

/*
Load simulation state from the specified save state.

THIS FUNCTION HAS SIDE EFFECTS.

@param loader: A saved simulation state loader.
*/
void load_state(const ISaveState* loader) {
	// Store results in a new SimulationState pointer.
	std::shared_ptr<SimulationState> new_state = std::shared_ptr<SimulationState>(new SimulationState());
	loader->Load(new_state.get());

	// Global state variable now points to the new simulation state.
	state = std::shared_ptr<SimulationState>(new_state);
}

//'
//' Save the current simulation state
//'
//' @description
//'
//' Save the current simulation state to memory, and return an ID which
//' uniquely identifies this state and which may later be used to
//' resume the simulation from this state.
//'
//' Note that this saved state will not be accessible between restarts
//' of R. To achieve that, use [save_state_to_file].
//'
//' @export
//'
// [[Rcpp::export]]
int save_simulation_state() {
	// Save simulation state.
    InMemorySaveState* saver = new InMemorySaveState();
    saver->save();

    int id = save_states.size();
	save_states.push_back(saver);

    return id;
}

//'
//' Load simulation from a previously saved state
//'
//' @description
//'
//' Load the simulation from a state previously saved via [save_state].
//'
//' Note that the state must have been saved in /this/ R session. To
//' transfer state across R sessions, use [save_state_to_file].
//'
//' @param state_id: A state ID returned from [save_state].
//'
//' @export
//'
// [[Rcpp::export]]
void load_simulation_state(int state_id) {
    if (state_id >= save_states.size()) {
        throw std::runtime_error("Invalid state id: " + state_id);
    }
    ISaveState* saved_state = save_states[state_id];
	load_state(saved_state);
}

//'
//' Save the current simulation state to a directory
//'
//' @description
//'
//' Save the current simulation state to a directory.
//'
//' @param dirname: Path to which the state will be saved.
//'
//' @export
//'
// [[Rcpp::export]]
void save_state_to_dir(std::string dirname) {
	// Don't use the save state queue for this implementation.
	std::unique_ptr<DiskBoundSaveState> state = std::unique_ptr<DiskBoundSaveState>(new DiskBoundSaveState(dirname));
	state->save();
}

//'
//' Load simulation from a state directory
//'
//' @description
//'
//' Load the simulation state from a directory previously generated by
//' [save_state_to_dir].
//'
//' @param dirname: Path to which the state was saved.
//'
//' @export
//'
// [[Rcpp::export]]
void load_state_from_dir(std::string dirname) {
	// Create a new DiskBoundLoader for this task.
	// We don't use the save state queue for this implementation.
    DiskBoundSaveState loader(dirname);
	load_state(&loader);
}
