#include <Rcpp.h>

// lpjgr
#include "init.h"
#include "model_access.h"
#include "state.h"
#include "util.h"

// lpj-guess
#include "guess.h"
#include "gutil.h"

const std::string CLIMATE_TODAY = "today";

Pft* get_pft(Patch* patch, std::string name) {

    if (!patch->pft.firstobj()) {
        throw std::runtime_error("Failed to reset vegetation to first individual");
    }

    xtring pft_name(name.c_str());
    for (int i = 0; patch->pft.isobj; i++, patch->pft.nextobj()) {
        Patchpft ppft = patch->pft.getobj();
        if (pft_name == ppft.pft.name) {
            return &(ppft.pft);
        }
    }
    char buf[256];
    sprintf(buf, "No pft exists with name '%s'", name.c_str());
    throw std::runtime_error(buf);
}

Patchpft* get_patchpft(Patch* patch, std::string name) {
    Pft* pft = get_pft(patch, name);
    return &(patch->pft[pft->id]);
}

Standpft* get_standpft(std::string name) {
    Pft* pft = get_pft(state->patch, name);
    return &(state->patch->stand.pft[pft->id]);
}

/*
List all spft names.
*/
std::vector<std::string> list_spft_names() {
    ensure_initialised();

    std::vector<std::string> spft_names;
    if (state->patch->pft.firstobj()) {
        while (state->patch->pft.isobj) {
            spft_names.push_back((const char*)state->patch->pft.getobj().pft.name);
            state->patch->pft.nextobj();
        }
    }
    return spft_names;
}

Rcpp::CharacterVector get_pft_names() {
    // Ensure that initialise() has been called.
    ensure_initialised();

    // Create a vector to hold the return value.
    Rcpp::CharacterVector names(state->patch->pft.nobj);

    if (!state->patch->pft.firstobj()) {
        throw std::runtime_error("Failed to reset vegetation to first individual");
    }

    for (int i = 0; state->patch->pft.isobj; i++, state->patch->pft.nextobj()) {
        names[i] = state->patch->pft.getobj().pft.name;
    }

    return names;
}

Individual* get_individual(std::string name) {
    // Create xtring object for convenient string comparisons.
    xtring xname(name.c_str());

    // Ensure that initialise() has been called.
    ensure_initialised();

    // Attempt to move to front of list.
    if (!state->patch->vegetation.firstobj()) {
        throw std::runtime_error("Unable to get individual: no individuals are defined");
    }

    // Iterate through individuals.
    while (state->patch->vegetation.isobj) {
        Individual& individual = state->patch->vegetation.getobj();
        if (xname == individual.pft.name) {
            return &individual;
        }
        state->patch->vegetation.nextobj();
    }
    state->patch->vegetation.firstobj();

    // Not found.
    char buf[256];
    sprintf(buf, "Cannot find individual with name '%s'", name.c_str());
    throw std::runtime_error(buf);
}

/*
Get the names of all established individuals.
*/
std::vector<std::string> list_individuals() {
    // Ensure that initialise() has been called.
    ensure_initialised();

    std::vector<std::string> individual_names;

    // Attempt to move to front of list.
    if (state->patch->vegetation.firstobj()) {
        // Iterate through individuals.
        while (state->patch->vegetation.isobj) {
            Individual& individual = state->patch->vegetation.getobj();
            std::string name = (char*)individual.pft.name;
            individual_names.push_back(name);
            state->patch->vegetation.nextobj();
        }
    }
    state->patch->vegetation.firstobj();
    return individual_names;
}

/*
Get the climate object with the specified name.

I'm taking a name argument here because it's required by the ObjectOutputregistry<T>
interface. Possibly need to rethink this - but for now it could provide a
convenient way to access past or future met data. Maybe.
*/
Climate* get_climate(std::string name) {
    if (name == CLIMATE_TODAY) {
        return &(state->grid_cell->climate);
    }
    throw std::runtime_error("Unknown climate name: " + name);
}

/*
Get the list of valid climate object names. Currently only "today".
See coment on get_climate(): need to rethink this.
*/
std::vector<std::string> list_climate_names() {
    std::vector<std::string> names;
    names.push_back(CLIMATE_TODAY);
    return names;
}

/*
Get the number of trees established in the stand.
*/
int get_num_trees() {
	return count<Individual, Pft, Vegetation>(&(state->patch->vegetation), [](const Individual* indiv) -> bool {
		return indiv->pft.lifeform == TREE;
	});
}