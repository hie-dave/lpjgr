#include <Rcpp.h>

// lpjgr
#include "init.h"
#include "model_access.h"
#include "state.h"

// lpj-guess
#include "guess.h"
#include "gutil.h"

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
    Pft* pft = get_pft(patch, name);
    return &(patch->stand.pft[pft->id]);
}

/*
List all spft names.
*/
std::vector<std::string> list_spft_names() {
    ensure_initialised();

    std::vector<std::string> spft_names;
    if (patch->pft.firstobj()) {
        while (patch->pft.isobj) {
            spft_names.push_back((const char*)patch->pft.getobj().pft.name);
            patch->pft.nextobj();
        }
    }
    return spft_names;
}

// [[Rcpp::export]]
Rcpp::CharacterVector get_pft_names() {
    // Ensure that initialise() has been called.
    ensure_initialised();

    // Create a vector to hold the return value.
    Rcpp::CharacterVector names(patch->pft.nobj);

    if (!patch->pft.firstobj()) {
        throw std::runtime_error("Failed to reset vegetation to first individual");
    }

    for (int i = 0; patch->pft.isobj; i++, patch->pft.nextobj()) {
        names[i] = patch->pft.getobj().pft.name;
    }

    return names;
}

Individual* get_individual(std::string name) {
    // Create xtring object for convenient string comparisons.
    xtring xname(name.c_str());

    // Ensure that initialise() has been called.
    ensure_initialised();

    // Attempt to move to front of list.
    Vegetation veg = patch->vegetation;
    if (!veg.firstobj()) {
        throw std::runtime_error("Unable to get individual: no individuals are defined");
    }

    // Iterate through individuals.
    while (veg.isobj) {
        Individual& individual = veg.getobj();
        if (xname == individual.pft.name) {
            return &individual;
        }
        veg.nextobj();
    }

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
    Vegetation veggies = patch->vegetation;

    if (veggies.firstobj()) {
        // Iterate through individuals.
        while (veggies.isobj) {
            Individual& individual = veggies.getobj();
            individual_names.push_back((const char*)individual.pft.name);
            veggies.nextobj();
        }
    }

    return individual_names;
}
