#include "model_access.h"
#include "state.h"

#include "guess.h"

double get_photosynthesis_property(const PhotosynthesisResult* photosynthesis, std::string name) {
    if (name == "vm")
        // RuBisCO capacity (gC/m2/day)
        return photosynthesis->vm;

    else if (name == "agd_g")
        // gross daily photosynthesis (gC/m2/day)
        return photosynthesis->agd_g;

    else if (name == "adtmm")
        // leaf-level net daytime photosynthesis
        // expressed in CO2 diffusion units (mm/m2/day)
        return photosynthesis->adtmm;

    else if (name == "rd_g")
        // leaf respiration (gC/m2/day)
        return photosynthesis->rd_g;

    else if (name == "je")
        // PAR-limited photosynthesis rate (gC/m2/h)
        return photosynthesis->je;

    else if (name == "nactive_opt")
        // optimal leaf nitrogen associated with photosynthesis (kgN/m2)
        return photosynthesis->nactive_opt;

    else if (name == "vmaxnlim")
        // nitrogen limitation on vm
        return photosynthesis->vmaxnlim;
    char buf[256];
    sprintf(buf, "Unknown photosynthesis property name '%s'", name.c_str());
    throw std::runtime_error(buf);
}

// [[Rcpp::export]]
double get_individual_property(std::string individual_name, std::string property_name) {
    Individual* individual = get_individual(individual_name);
    return get_photosynthesis_property(&(individual->photosynthesis), property_name);
}

// [[Rcpp::export]]
double get_pft_property(std::string pft_name, std::string property_name) {
    Standpft* pft = get_standpft(patch, pft_name);
    return get_photosynthesis_property(&(pft->photosynthesis), property_name);
}
