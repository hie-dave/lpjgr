#'
#' @title lpjgr: LPJ-Guess Wrapper
#'
#' @description
#'
#' lpjgr is a wrapper around some LPJ-Guess functionality, allowing
#' certain subroutines to be examined/investigated from R.
#'
#' @details
#'
#' ## Initialisation
#'
#' Before running any other function, you'll need to run
#' [initialise].
#' This requires an .ins file as argument, and in the future, it will
#' probably also require an input module, but for now it's hardcoded
#' to use the siteinput module (developed by David Wårlind for eucface).
#'
#' ## Simulation
#'
#' You can run a whole simulation with [run_simulation].
#'
#' In order to access outputs of an individual, that individual will need
#' to be established. A list of established individuals can be retrieved
#' by running [list_object_names]. Note that no individuals will be
#' established at the start of simulation - see
#' [simulate_until_tree_establishment] for a workaround.
#'
#' ## Inputs
#'
#' Input handling needs rationalisation. For now, see:
#'
#' - [set_lat]
#' - [set_lon]
#' - [set_co2]
#' - [set_tmin]
#' - [set_tmax]
#' - [set_prec]
#' - [set_insol]
#' - [set_ndep]
#' - [set_wind]
#' - [set_relhum]
#'
#' ## Outputs
#'
#' Currently only numeric outputs are supported. They are divided into
#' categories (e.g. individual, standpft). To get a list of categories,
#' see [list_all_categories]. To get a list of outputs for a
#' specific category, see [list_outputs_in_category].
#'
#' Outputs are generally a property of an object, and in order to retrieve
#' an output value, you'll need to know the name of the object to which
#' the property belongs (e.g. the name of an individual). To view all valid
#' object names, see [list_object_names].
#'
#' @docType package
#' @name lpjgr
#' @useDynLib lpjgr
#' @import Rcpp
#'
#' @seealso [initialise]
#' @seealso [get_output]
#' @seealso [simulate_until_tree_establishment]
#'
NULL
#> NULL
