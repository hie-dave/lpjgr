#ifndef _MODEL_ACCESS_H_
#define _MODEL_ACCESS_H_

#include <Rcpp.h>
#include <string>
#include <vector>

#include "guess.h"

/*
Get the first stand in the given grid cell. Throw if none found.
*/
Stand* get_stand(Gridcell* grid_cell);

/*
Get the first patch in the stand. Throw if none found.
*/
Patch* get_patch(Stand* stand);

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

/*
Get the climate object with the specified name.

I'm taking a name argument here because it's required by the ObjectOutputregistry<T>
interface. Possibly need to rethink this - but for now it could provide a
convenient way to access past or future met data. Maybe.
*/
Climate* get_climate(std::string name);

/*
Get the list of valid climate object names. Currently only "today".
See coment on get_climate(): need to rethink this.
*/
std::vector<std::string> list_climate_names();

/*
Get the number of trees established in the stand.
*/
int get_num_trees();

#endif // _MODEL_ACCESS_H_
