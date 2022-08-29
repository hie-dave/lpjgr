#include "init.h"
#include "simulate.h"
#include "state.h"
#include "simulation_state.h"

#include "guess.h"
#include "commandlinearguments.h"
#include "framework.h"
#include "shell.h"
#include "soilwater.h"
#include "cropallocation.h"
#include "somdynam.h"
#include "soilmethane.h"
#include "blaze.h"
#include "growth.h"
#include "management.h"
#include "vegdynam.h"
#include "canexch.h"
#include "driver.h"
#include "cropsowing.h"
#include "cropphenology.h"
#include "landcover.h"

//'
//' Run a full LPJ-Guess simulation.
//'
//' @param input_module_name: Name of the input module to be used.
//' @param ins_file: Path to the .ins file to be used.
//'
//' @export
//'
// [[Rcpp::export]]
void run_simulation(std::string input_module_name, std::string ins_file) {
    // This would normally be handled by initialise(), but as we're not calling
    // initialise(), we need to manually initialise the shell.
    initialise_shell();

    CommandLineArguments args(input_module_name, ins_file);
    framework(args);
}

/*
Read today's met data.

THIS HAS SIDE EFFECTS!! Do **not** call this multiple times per day, or you
will regret it.
*/
void read_todays_met_data() {
    if (!state->input_module->getclimate(*state->grid_cell)) {
        // No met data available - abort!
        char buf[256];
        sprintf(buf, "No more met data (date=%d-%d-%d)", date.get_calendar_year(), date.month, date.day);
        throw std::runtime_error(buf);
    }
}

void pre_canexch() {
    ensure_initialised();

    // Check state to make sure that this is a valid time
    // to be calling this function.
    if (state->temporal_state == START_OF_DAY) {
        // Read today's met data.
        read_todays_met_data();
    } else if (state->temporal_state == END_OF_DAY) {
        // Advance to next day of simulation.
        date.next();

        // Read today's met data.
        read_todays_met_data();
    } else if (state->temporal_state == BEFORE_CANEXCH) {
        canexch();
        post_canexch();
    } else if (state->temporal_state == AFTER_CANEXCH) {
        post_canexch();
    } else {
        throw std::runtime_error("Simulation in unknown state: " + state->temporal_state);
    }

	// Update daily climate drivers etc
	dailyaccounting_gridcell(*state->grid_cell);

	// Calculate daylength, insolation and potential evapotranspiration
	daylengthinsoleet(state->grid_cell->climate);

	// Update crop sowing date calculation framework
	crop_sowing_gridcell(*state->grid_cell);

	// Dynamic landcover and crop fraction data during historical
	// period and create/kill stands.
	landcover_dynamics(*state->grid_cell, state->input_module);

	// Update dynamic management options
	state->input_module->getmanagement(*state->grid_cell);

	// Set forest management for all stands this year
	manage_forests(*state->grid_cell);

    dailyaccounting_stand(*state->stand);
    dailyaccounting_patch(*state->patch);

    if (run_landcover) {
        nfert(*state->patch);
    }

    crop_sowing_patch(*state->patch);
    crop_phenology(*state->patch);
    leaf_phenology(*state->patch, state->grid_cell->climate);
    interception(*state->patch, state->grid_cell->climate);

    // Update simulation state.
    state->temporal_state = BEFORE_CANEXCH;
}

//'
//' Run daily canopy exchange. This can be called multiple times on a single day.
//'
//' @export
//'
// [[Rcpp::export]]
void canexch() {
    ensure_initialised();

    if (state->temporal_state == START_OF_DAY || state->temporal_state == END_OF_DAY) {
        pre_canexch();
    } else if (state->temporal_state != BEFORE_CANEXCH && state->temporal_state != AFTER_CANEXCH) {
        // It's fine to run canexch() multiple times consecutively (ie
        // state == AFTER_CANEXCH is fine).
        throw std::runtime_error("Simulation in unknown state: " + state->temporal_state);
    }
    canopy_exchange(*state->patch, state->grid_cell->climate);

    // Update simulation state.
    state->temporal_state = AFTER_CANEXCH;
}

void post_canexch() {
    ensure_initialised();

    if (state->temporal_state == START_OF_DAY || state->temporal_state == END_OF_DAY) {
        pre_canexch();
    }
    if (state->temporal_state == BEFORE_CANEXCH) {
        canexch();
    }
    if (state->temporal_state != AFTER_CANEXCH) {
        throw std::runtime_error("Simulation in unknown state: " + state->temporal_state);
    }

    // Sum total required irrigation
    irrigation(*state->patch);
    // Soil water accounting, snow pack accounting
    soilwater(*state->patch, state->grid_cell->climate);

    // Daily C allocation (cropland)
    growth_daily(*state->patch);

    // Soil organic matter and litter dynamics
    som_dynamics(*state->patch, state->grid_cell->climate);

    // Methane production/consumption on wetlands and peatlands (no methane dynamics for other stand types at present) 
    methane_dynamics(*state->patch);

    // BLAZE fire model
    blaze_driver(*state->patch, state->grid_cell->climate);

    if (state->date->islastday && state->date->islastmonth) {
        // LAST DAY OF YEAR
        // Tissue turnover, allocation to new biomass and reproduction,
        // updated allometry
        growth(*state->stand, *state->patch);
    }
    crop_rotation(*state->stand);
    if (state->date->islastday && state->date->islastmonth) {
        // Note that we should be doing this for all patches.
        vegetation_dynamics(*state->stand, *state->patch);
    }

    // Check mass balance.
    state->grid_cell->balance.check_period(*state->grid_cell);

    // Update simulation state.
    state->temporal_state = END_OF_DAY;
}
