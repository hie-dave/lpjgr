#ifndef _LPJGR_DISKBOUND_SAVE_STATE_H_
#define _LPJGR_DISKBOUND_SAVE_STATE_H_

#include "isave_state.h"
#include "simulate.h"
#include "simulation_state.h"

/*
A class which can save and load the simulation state to/from disk.

This is useful for resuming runs between restarts of R.
*/
class DiskBoundSaveState : public ISaveState {
	public:
		/*
		Constructor.

		@param directory: Directory into which state will be saved/loaded.
		*/
		DiskBoundSaveState(std::string directory);

		/*
		Save the current state of the simulation.
		*/
		void save() const;

		/*
		Load the previously-saved simulation state.

		@param state: (out param): the new simulation state.
		*/
		void Load(SimulationState* state) const;
	private:
		/*
		Get the path to the metadata file.
		*/
		std::string get_meta_path();

		/*
		Path to which state will be read/written.
		*/
		std::string dirname;
};

#endif // _LPJGR_DISKBOUND_SAVE_STATE_H_
