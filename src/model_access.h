#ifndef _MODEL_ACCESS_H_
#define _MODEL_ACCESS_H_

#include <Rcpp.h>

#include "guess.h"

/*
Find a pft in the given patch with the specified name.
@param patch: Owner of the pft.
@param name: Name of the pft.
*/
Pft* get_pft(Patch* patch, std::string name);

/*
Find a Patchpft within the given patch with the specified name.
@param patch: Owner of the pft.
@param name: Name of the pft.
*/
Patchpft* get_patchpft(Patch* patch, std::string name);

/*
Find a Standpft within the given patch with the specified name.
@param patch: Owner of the pft.
@param name: Name of the pft.
*/
Standpft* get_standpft(std::string name);

/*
List all spft names.
*/
std::vector<std::string> list_spft_names();

/*
Get the names of all defined PFTs.
*/
Rcpp::CharacterVector get_pft_names();

/*
Get the individual with the specified name, or throw if not found.
@param name: Name of the individual (case-sensitive).
*/
Individual* get_individual(std::string name);

/*
Get the names of all established individuals.
*/
std::vector<std::string> list_individuals();

#endif // _MODEL_ACCESS_H_
