#ifndef _LPJGR_ISAVE_STATE_H_
#define _LPJGR_ISAVE_STATE_H_

#include "simulation_state.h"

/*
An interface for any class which can save the current simulation state
and load it again later.
*/
class ISaveState {
	public:
		/*
		Save the current state of the simulation.
		*/
		virtual void save() const = 0;

		/*
		Load the previously-saved simulation state.

		@param state: (out param): the new simulation state.
		*/
		virtual void Load(SimulationState* state) const = 0;
};

#endif // _LPJGR_ISAVE_STATE_H_
