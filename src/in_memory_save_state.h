#ifndef _LPJGR_IN_MEMORY_SAVE_STATE_H_
#define _LPJGR_IN_MEMORY_SAVE_STATE_H_

#include <memory>

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
		void Load(SimulationState* state) const;
	private:
		SimulationState* saved_state;
};

#endif // _LPJGR_IN_MEMORY_SAVE_STATE_H_
