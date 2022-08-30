#ifndef _LPJGR_IN_MEMORY_SAVE_STATE_H_
#define _LPJGR_IN_MEMORY_SAVE_STATE_H_

#include <memory>
#include <sstream>

#include "isave_state.h"
#include "state.h"

class InMemorySaveState : public ISaveState {
	public:
		/*
		Save the current state of the simulation.
		*/
		void save();

		/*
		Load the previously-saved simulation state.

		@param state: (out param): the new simulation state.
		*/
		void Load(SimulationState* state);
	private:
		std::stringstream state_buf;
		std::stringstream meta_buf;

		// .ins file in use when state was saved.
		std::string ins_file_name;

		// Year in which state was saved.
		int year;

		// Day of year in which state was saved.
		int doy;

		// Start year in which state was saved (to allow for
		// save_state() -> initialise() -> load_state()).
		int start_year;
};

#endif // _LPJGR_IN_MEMORY_SAVE_STATE_H_
