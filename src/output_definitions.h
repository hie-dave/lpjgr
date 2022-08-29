#ifndef _LPJGR_OUTPUT_DEFINITIONS_H_
#define _LPJGR_OUTPUT_DEFINITIONS_H_

#include "guess.h"

#include "outputmodule.h"
#include "object_output_registry.h"

/*
Get available outputs for individuals.
*/
ObjectOutputRegistryBase<double>* get_individual_outputs();

/*
Get available outputs for Stand PFTs.
*/
ObjectOutputRegistryBase<double>* get_spft_outputs();

/*
Get available met outputs.
*/
ObjectOutputRegistryBase<double>* get_met_outputs();

#endif // _LPJGR_OUTPUT_DEFINITIONS_H_
