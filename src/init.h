#ifndef _LPJGR_INIT_H_
#define _LPJGR_INIT_H_

#include <string>
#include "simulation_state.h"

/*
Check if initialise() has been called. Throw if it has not.
*/
void ensure_initialised();

/*
Set the date object to the specified date.
*/
void initialise_date(Date* date, int year, int dayofyear);

/*
Initialise the run. This **must** be called before canexch().

@param insFile:     Path to the .ins file.
*/
void initialise(std::string insFile);

/*
This function exists for internal use only. It will initialise the
specified state object but will otherwise have no effect on the global
environment.

Note: upon further consideration, the lpj-guess globals (see parameters.cpp)
will be modified by this function. So it's sadly not as friendly as one
might like.

See initialise() for more details.

@param insFile: Path to the instructionfile.
@param state: The simulation state to be configured/initialised.
*/
void initialise_state(std::string insFile, SimulationState* state);

/*
Set our shell for the model to communicate with the world. This only
needs to be done once, and I'm not sure of the effects of doing it
multiple times. Failure to do this will result in a segfault.

This will be automatically called by initialise(), so it shouldn't normally be
called manually.
*/
void initialise_shell();

#endif // _LPJGR_INIT_H_
