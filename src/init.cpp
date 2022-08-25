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
    if (!(grid_cell && stand && patch)) {
        throw std::runtime_error("LPJ-Guess environment has not been initialised. Please call initialise().");
    }
}

/*
Initialise the run. This **must** be called before canexch().

@param insFile: Path to the .ins file.
*/
// [[Rcpp::export]]
void initialise(std::string insFile) {
    initialise_shell();

    if (input_module) {
        delete input_module;
    }
    // todo: make input module type an argument
    input_module = new SiteInput;

    GuessOutput::MiscOutput misc;
    GuessOutput::CommonOutput common;
    if (output_modules) {
        delete output_modules;
    }
    output_modules = new GuessOutput::OutputModuleContainer;

	GuessOutput::OutputModuleRegistry::get_instance().create_all_modules(*output_modules);
    register_outputs();

    read_instruction_file(insFile.c_str());

    input_module->init();
    output_modules->init();

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

    // Technically, we could probably reuse the same gridcell object each time
    // this function is called. But for now, I'm just going to play it safe and
    // create a new one every time.
    if (grid_cell) {
        delete(grid_cell);
    }
    grid_cell = new Gridcell;

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

    // Retrieve 1st stand from gridcell.
    Gridcell::iterator gc_itr = grid_cell->begin();
    if (gc_itr == grid_cell->end()) {
        throw std::runtime_error("Grid cell contains no stands");
    }
    stand = &*gc_itr;

    stand->firstobj();
    if (!stand->isobj) {
        throw std::runtime_error("Grid cell contains no stands");
    }

    // Retrieve patch from stand.
    patch = &stand->getobj();

    pre_canexch();

    // We are now ready to call canopy_exchange().
}
