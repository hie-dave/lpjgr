// System includes.
#include <Rcpp.h>
#include <memory>
#include <stdlib.h>

// LPJ-Guess includes.
#include "canexch.h"
#include "inputmodule.h"
#include "outputchannel.h"
#include "outputmodule.h"
#include "commonoutput.h"
#include "miscoutput.h"
#include "bvoc.h"
#include "guessserializer.h"
#include "parallel.h"
#include "landcover.h"
#include "driver.h"
#include "gutil.h"
#include "shell.h"
#include "outputmodule.h"
#include "siteinput.h"

// LPJGR includes.
#include "init.h"
#include "model_access.h"
#include "state.h"
#include "output.h"
#include "simulate.h"
#include "util.h"

/*
Set our shell for the model to communicate with the world. Failure to
do this will result in a segfault.
*/
void initialise_shell() {
    // set_shell uses smart pointers internally, so it will free up the
    // shell instance for us.
    set_shell(new CommandLineShell("guess.log"));
}

/*
Check if initialise() has been called. Throw if it has not.
*/
void ensure_initialised() {
    if (!(state && state->grid_cell && state->stand && state->patch)) {
        throw std::runtime_error("LPJ-Guess environment has not been initialised. Please call initialise().");
    }
}

/*
Set the date object to the specified date.
*/
void initialise_date(Date* date, int year, int dayofyear) {
	if (year < 0 || dayofyear >= Date::MAX_YEAR_LENGTH) {
		char buf[256];
		sprintf(buf, "Invalid date: year=%d, doy=%d", year, dayofyear);
		throw std::runtime_error(buf);
	}

	if (date->year > year && date->day > dayofyear) {
		throw std::runtime_error("Need to call date.init() before initialise_date()");
	}

	// The safe but (relatively) slow approach.
	while (date->year < year || date->day < dayofyear) {
		date->next();
	}
}

//'
//' Initialise the LPJ-Guess environment
//'
//' @description
//'
//' This will cause lpj-guess to parse the .ins file, initialise the
//' output modules, and otherwise prepare a simulation run.
//'
//' This **must** be called before any other lpjgr functions.
//'
//' @details
//'
//' This is currently hardwired to use a siteinput input module. This will change
//' at some point in the future, to make the input module user-configurable.
//'
//' In theory, it *should* be possible to call this multiple times, in order
//' to re-initialise the lpj-guess environment, but I haven't tested that, so
//' use at your own risk (for now).
//'
//' @param insFile: Path to the .ins file.
//'
//' @export
//'
// [[Rcpp::export]]
void initialise(std::string insFile) {
    // Technically, we could probably reuse the same set of objects
	// each time around, but for now I'm going to play it safe and
	// rebuild the environment from scratch each time.
	state = std::shared_ptr<SimulationState>(new SimulationState());
	initialise_state(insFile, state.get());
}

void reference_output_modules() {
	// This function is never called, and doesn't do anything.
	// However, it does reference a few output module types, to ensure
	// that the linker includes them in the output library.
	GuessOutput::CommonOutput common;
	GuessOutput::MiscOutput misc;
}

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
void initialise_state(std::string insFile, SimulationState* state) {
    initialise_shell();

    // todo: make input module type an argument
    InputModule* input_module = new SiteInput;

	GuessOutput::OutputModuleContainer output_modules;
	GuessOutput::OutputModuleRegistry::get_instance().create_all_modules(output_modules);

	// lpjgr output initialisation.
    register_outputs();

    read_instruction_file(insFile.c_str());

    input_module->init();
    output_modules.init();

	// Nitrogen limitation
	if (ifnlim && !ifcentury) {
		fail("\n\nIf nitrogen limitation is switched on then century soil module also needs to be switched on!");
	}

	// bvoc
	if (ifbvoc) {
		initbvoc();
	}

	// Create objects for (de)serializing grid cells
	std::unique_ptr<GuessSerializer> serializer;
	std::unique_ptr<GuessDeserializer> deserializer;

	if (save_state) {
		serializer = std::unique_ptr<GuessSerializer>(new GuessSerializer(state_path, GuessParallel::get_rank(), GuessParallel::get_num_processes()));
	}

	if (restart) {
		deserializer = std::unique_ptr<GuessDeserializer>(new GuessDeserializer(state_path));
	}

    date.init(1);

	Gridcell* grid_cell = new Gridcell;
    if (!input_module->getgridcell(*grid_cell)) {
        throw std::runtime_error("Gridlist contains no grid cells");
    }

    grid_cell->climate.initdrivers(grid_cell->get_lat());

    landcover_init(*grid_cell, input_module);

    if (restart) {
        deserializer->deserialize_gridcell(*grid_cell);
        date.year = state_year;
    }

    if (!input_module->getclimate(*grid_cell)) {
        throw std::runtime_error("Unable to read met data for grid cell");
    }

    Stand* stand = get_stand(grid_cell);
    Patch* patch = get_patch(stand);

	// Store stuff in the state object.
	state->date = &date; // Date is a global variable in lpj-guess...
	state->grid_cell = grid_cell;
	state->input_module = input_module;
	state->instruction_file = insFile;
	state->patch = patch;
	state->stand = stand;

    // We are now ready to call simulation functions.
}

#include "date.h"

//'
//' Get the current simulation date.
//'
//' @description
//'
//' Get the current simulation date (ie the date which will be
//' simulated on the next call to simulate_day()).
//'
//' @export
//'
// [[Rcpp::export]]
lpjgr_date get_date() {
	ensure_initialised();
	lpjgr_date dt = create_date(state->date);
	return dt;
}
