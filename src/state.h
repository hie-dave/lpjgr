#ifndef _LPJGR_STATE_H_
#define _LPJGR_STATE_H_

#include <memory>

#include "inputmodule.h"
#include "guess.h"
#include "outputmodule.h"

#include "state.h"

struct SimulationState {
    public:
		~SimulationState();
		Gridcell* grid_cell;
		Date* date;
		std::string instruction_file;
		InputModule* input_module;
		Stand* stand;
		Patch* patch;
};

// Global state variables used by other LPJGR modules.
extern std::shared_ptr<SimulationState> state;

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
//' of R. To achieve that, use [save_state_to_dir].
//'
//' @export
//'
int save_simulation_state();

//'
//' Save the current simulation state to a directory
//'
//' @description
//'
//' Save the current simulation state to a directory.
//'
//' @param dirname: Directory in which the state will be saved.
//'
//' @export
//'
void save_state_to_dir(std::string dirname);

//'
//' Load simulation from a previously saved state
//'
//' @description
//'
//' Load the simulation from a state previously saved via [save_state].
//'
//' Note that the state must have been saved in /this/ R session. To
//' transfer state across R sessions, use [save_state_to_dir].
//'
//' @param state_id: A state ID returned from [save_state].
//'
//' @export
//'
void load_simulation_state(int state_id);

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
void load_state_from_dir(std::string dirname);

#endif // _LPJGR_STATE_H_
