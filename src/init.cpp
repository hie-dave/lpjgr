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

// LPJGR includes.
#include "init.h"
#include "model_access.h"
#include "proxyinput.h"
#include "state.h"
#include "output.h"

CommandLineShell* shell;

/*
Set our shell for the model to communicate with the world. This only
needs to be done once, and I'm not sure of the effects of doing it
multiple times. Failure to do this will result in a segfault.
*/
void initialise_shell() {
    if (!shell) {
        shell = new CommandLineShell("guess.log");
        set_shell(shell);
    }
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

@param lat: Latitude of the gridcell.
@param lon: Longitude of the gridcell.
@param co2: Daily atmospheric ambient CO2 content (ppm).
@param tmin: Daily minimum temperature (°C).
@param tmax: Daily maximum temperature (°C).
@param prec: Daily precipitation (mm).
@param insol: Daily insolation (units?).
@param ndep: Daily total annual N deposition (units?).
@param wind: Daily 10m wind speed (km/h).
@param relhum: Daily relative humidity (fraction).
*/
// [[Rcpp::export]]
void initialise(std::string insFile,
    double lat,
    double lon,
    double co2,
    double tmin,
    double tmax,
    double prec,
    double insol,
    double ndep,
    double wind,
    double relhum) {
    initialise_shell();

    if (input_module) {
        delete input_module;
    }
    input_module = new ProxyInput(
        lat,
        lon,
        co2,
        tmin,
        tmax,
        prec,
        insol,
        ndep,
        wind,
        relhum);

    GuessOutput::MiscOutput misc;
    GuessOutput::CommonOutput common;
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
        char buf[60];
        sprintf(buf, "Unable to read met data for grid cell (%.2f, %.2f)", lat, lon);
        throw std::runtime_error(buf);
    }

    // Now simulate the day until we are ready to call canopy_exchange().
    dailyaccounting_gridcell(*grid_cell);
    daylengthinsoleet(grid_cell->climate);
    crop_sowing_gridcell(*grid_cell);
    landcover_dynamics(*grid_cell, input_module);
    input_module->getmanagement(*grid_cell);
    manage_forests(*grid_cell);

    // Retrieve 1st stand from gridcell.
    Gridcell::iterator gc_itr = grid_cell->begin();
    if (gc_itr == grid_cell->end()) {
        char buf[60];
        sprintf(buf, "Grid cell (%.2f, %.2f) contains no stands", lat, lon);
        throw std::runtime_error(buf);
    }
    stand = &*gc_itr;

    dailyaccounting_stand(*stand);

    stand->firstobj();
    if (!stand->isobj) {
        char buf[60];
        sprintf(buf, "Grid cell(%.2f, %.2f) contains no stands", lat, lon);
        throw std::runtime_error(buf);
    }

    // Retrieve patch from stand.
    patch = &stand->getobj();
    dailyaccounting_patch(*patch);

    if (run_landcover) {
        nfert(*patch);
    }

    crop_sowing_patch(*patch);
    crop_phenology(*patch);
    leaf_phenology(*patch, grid_cell->climate);
    interception(*patch, grid_cell->climate);

    // We are now ready to call canopy_exchange().
}
