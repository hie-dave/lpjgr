#include "init.h"
#include "simulate.h"
#include "state.h"

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

typedef enum {
    // At start of day
    START_OF_DAY,
    // Ready to call canopy_exchange()
    BEFORE_CANEXCH,
    // After canopy_exchange() has been called
    AFTER_CANEXCH,
    // At end of day
    END_OF_DAY
} SimulationState;

// The current (temporal) state of the simulation.
SimulationState simulation_state;

/*
Run a full LPJ-Guess simulation.
@param input_module_name: Name of the input module to be used.
@param ins_file: Path to the .ins file to be used.
*/
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
    if (!input_module->getclimate(*grid_cell)) {
        // No met data available - abort!
        char buf[256];
        sprintf(buf, "No more met data (date=%d-%d-%d)", date.get_calendar_year(), date.month, date.day);
        throw std::runtime_error(buf);
    }
}

// [[Rcpp::export]]
void pre_canexch() {
    ensure_initialised();

    // Check state to make sure that this is a valid time
    // to be calling this function.
    if (simulation_state == START_OF_DAY) {
        // Read today's met data.
        read_todays_met_data();
    } else if (simulation_state == END_OF_DAY) {
        // Advance to next day of simulation.
        date.next();

        // Read today's met data.
        read_todays_met_data();
    } else if (simulation_state == BEFORE_CANEXCH) {
        canexch();
        post_canexch();
    } else if (simulation_state == AFTER_CANEXCH) {
        post_canexch();
    } else {
        throw std::runtime_error("Simulation in unknown state: " + simulation_state);
    }

	// Update daily climate drivers etc
	dailyaccounting_gridcell(*grid_cell);

	// Calculate daylength, insolation and potential evapotranspiration
	daylengthinsoleet(grid_cell->climate);

	// Update crop sowing date calculation framework
	crop_sowing_gridcell(*grid_cell);

	// Dynamic landcover and crop fraction data during historical
	// period and create/kill stands.
	landcover_dynamics(*grid_cell, input_module);

	// Update dynamic management options
	input_module->getmanagement(*grid_cell);

	// Set forest management for all stands this year
	manage_forests(*grid_cell);

    dailyaccounting_stand(*stand);
    dailyaccounting_patch(*patch);

    if (run_landcover) {
        nfert(*patch);
    }

    crop_sowing_patch(*patch);
    crop_phenology(*patch);
    leaf_phenology(*patch, grid_cell->climate);
    interception(*patch, grid_cell->climate);

    // Update simulation state.
    simulation_state = BEFORE_CANEXCH;
}

// [[Rcpp::export]]
void canexch() {
    ensure_initialised();

    if (simulation_state == START_OF_DAY || simulation_state == END_OF_DAY) {
        pre_canexch();
    } else if (simulation_state != BEFORE_CANEXCH && simulation_state != AFTER_CANEXCH) {
        // It's fine to run canexch() multiple times consecutively (ie
        // state == AFTER_CANEXCH is fine).
        throw std::runtime_error("Simulation in unknown state: " + simulation_state);
    }
    canopy_exchange(*patch, grid_cell->climate);

    // Update simulation state.
    simulation_state = AFTER_CANEXCH;
}

// [[Rcpp::export]]
void post_canexch() {
    ensure_initialised();

    if (simulation_state == START_OF_DAY || simulation_state == END_OF_DAY) {
        pre_canexch();
    }
    if (simulation_state == BEFORE_CANEXCH) {
        canexch();
    }
    if (simulation_state != AFTER_CANEXCH) {
        throw std::runtime_error("Simulation in unknown state: " + simulation_state);
    }

    // Sum total required irrigation
    irrigation(*patch);
    // Soil water accounting, snow pack accounting
    soilwater(*patch, grid_cell->climate);

    // Daily C allocation (cropland)
    growth_daily(*patch);

    // Soil organic matter and litter dynamics
    som_dynamics(*patch, grid_cell->climate);

    // Methane production/consumption on wetlands and peatlands (no methane dynamics for other stand types at present) 
    methane_dynamics(*patch);

    // BLAZE fire model
    blaze_driver(*patch, grid_cell->climate);

    if (date.islastday && date.islastmonth) {
        // LAST DAY OF YEAR
        // Tissue turnover, allocation to new biomass and reproduction,
        // updated allometry
        growth(*stand, *patch);
    }
    crop_rotation(*stand);
    if (date.islastday && date.islastmonth) {
        // Note that we should be doing this for all patches.
        vegetation_dynamics(*stand, *patch);
    }

    // Check mass balance.
    grid_cell->balance.check_period(*grid_cell);

    // Update simulation state.
    simulation_state = END_OF_DAY;
}
