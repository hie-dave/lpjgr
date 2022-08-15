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

Standpft* get_standpft(Patch* patch, std::string name) {
    Pft* pft = get_pft(patch, name);
    return &(patch->stand.pft[pft->id]);
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

PhotosynthesisResult* get_photosynthesis(std::string pft_name) {
    return &(get_standpft(patch, pft_name)->photosynthesis);
}
