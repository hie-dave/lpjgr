#ifndef _LPJGR_INIT_H_
#define _LPJGR_INIT_H_

#include <string>

/*
Check if initialise() has been called. Throw if it has not.
*/
void ensure_initialised();

/*
Initialise the run. This **must** be called before canexch().

@param insFile:     Path to the .ins file.
*/
void initialise(std::string insFile);

/*
Set our shell for the model to communicate with the world. This only
needs to be done once, and I'm not sure of the effects of doing it
multiple times. Failure to do this will result in a segfault.

This will be automatically called by initialise(), so it shouldn't normally be
called manually.
*/
void initialise_shell();

#endif // _LPJGR_INIT_H_
