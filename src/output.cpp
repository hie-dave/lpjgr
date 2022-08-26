#include "init.h"
#include "model_access.h"
#include "output_registry.h"
#include "output_wrapper.h"
#include "output_definitions.h"
#include "state.h"
#include "object_output_registry_base.h"
#include "util.h"

#include "guess.h"

std::map<std::string, ObjectOutputRegistryBase<double>*>* numeric_outputs;

// category, name, description, units
const int OUTPUT_METADATA_NUM_COLS = 4;

// --------------------------------------------------------------------
// Internal functions.
// --------------------------------------------------------------------
void register_outputs() {
    if (!numeric_outputs) {
        // No point doing this more than once.
        numeric_outputs = new std::map<std::string, ObjectOutputRegistryBase<double>*>;
        numeric_outputs->emplace("individual", get_individual_outputs());
        numeric_outputs->insert(std::make_pair("spft", get_spft_outputs()));
    }
}

/*
Return the output registry for the specified output category. Throw if not found.

@param category_name: Name of the output category.
*/
ObjectOutputRegistryBase<double>* get_output_registry(std::string category_name) {
    ensure_initialised();

    // tbi: support for non-numeric outputs.
    std::map<std::string, ObjectOutputRegistryBase<double>*>::const_iterator pos = numeric_outputs->find(category_name);
    if (pos == numeric_outputs->end()) {
        throw std::runtime_error("No output category exists with name '" + category_name + "' (this is case-sensitive). You can list the available output categories with list_output_categories()");
    }

    return pos->second;
}

/*
Return the total number of outputs in a certain registry.

@param registry: The registry to be counted.
*/
int count_outputs(OutputRegistryBase* registry) {
    return registry->size();
}

/*
Return the total number of outputs.
*/
int count_all_outputs() {
    return sum<std::string, ObjectOutputRegistryBase<double>>(
        numeric_outputs, 
        [] (const ObjectOutputRegistryBase<double>* registry) -> int {
            return registry->size();
        }
    );
}

/*
Store metadata for all outputs from a certain object category inside the
matrix, starting from the specified row number. The matrix must already
contain enough rows/columns to store all of the data.

@param matrix: Matrix in which metadata should be stored.
@param category_name: Name of an output category. Will throw if invalid.
@param start: First row in matrix in which data should be stored.
*/
void store_output_metadata(Rcpp::CharacterMatrix* matrix, std::string category_name, int start) {
    ObjectOutputRegistryBase<double>* registry = get_output_registry(category_name);
    int row = start;
    std::vector<std::string> outputs = registry->list_output_names();
    for (int i = 0; i < outputs.size(); i++) {
        int col = 0;
        (*matrix)(row, col++) = category_name;
        (*matrix)(row, col++) = outputs[i];
        (*matrix)(row, col++) = registry->get_description(outputs[i]);
        (*matrix)(row, col++) = registry->get_units(outputs[i]);
        row++;
    }
}

/*
Create a "standard" output metadata matrix with the specified row count.

@param num_rows: Number of rows to be added to the matrix.
*/
Rcpp::CharacterMatrix create_output_metadata_table(int num_rows) {
    Rcpp::CharacterMatrix metadata(num_rows, OUTPUT_METADATA_NUM_COLS);

    Rcpp::CharacterVector column_names(OUTPUT_METADATA_NUM_COLS);
    column_names[0] = "Category";
    column_names[1] = "Name";
    column_names[2] = "Description";
    column_names[3] = "Units";
    Rcpp::colnames(metadata) = column_names;

    return metadata;
}

// --------------------------------------------------------------------
// User-callable (ie from R) functions.
// --------------------------------------------------------------------

//'
//' Read an output value from LPJ-Guess
//' 
//' @description 
//'
//' Ouputs in lpjgr are grouped by category and by object. E.g. there
//' is an output category for individuals, and we can retrieve an
//' output for a particular individual (identified by its name).
//'
//' @details
//'
//' Here is some sample code which prints the value of each output in
//' each object in each category.
//'
//' ```R
//' initialise(my_ins_file)
//' categories <- list_output_categories()[, 'Name']
//' for (category in categories) { 
//'     cat(strrep('-', 72))
//'     cat(paste('\nCategory:', category, '\n'))
//'     outputs <- list_output_names(category)
//'     objects <- list_object_names(category)
//'     if (length(objects) == 0) {
//'         cat(paste('No objects in category', category, '\n'))
//'         next
//'     }
//'     data <- matrix(nrow = length(objects), ncol = length(outputs))
//'     rownames(data) <- objects
//'     colnames(data) <- outputs
//'     for (obj in objects) { 
//'         for (output in outputs) {
//'             data[obj, output] <- get_output(category, obj, output)
//'         }
//'     }
//'     print(data)
//' }
//' ```
//'
//' @param category_name: Name of the output category (see [list_output_categories]).
//' @param object_name: Name of the object to which the output belongs (see [list_object_names]).
//' @param property_name: Name of the output property (see [list_output_names]).
//'
//' @seealso [lpjgr]
//' @seealso [list_output_categories]
//' @seealso [list_object_names]
//' @seealso [list_output_names]
//'
//' @export
//'
// [[Rcpp::export]]
double get_output(std::string category_name, std::string object_name, std::string property_name) {
    ensure_initialised();
    ObjectOutputRegistryBase<double>* registry = get_output_registry(category_name);
    return registry->get_output(object_name, property_name);
}

//'
//' List all valid output categories
//'
//' @description
//'
//' This will return a matrix containing all output category names, and
//' a description of each category.
//'
//' @seealso [get_output]
//'
//' @export
//'
// [[Rcpp::export]]
Rcpp::CharacterMatrix list_output_categories() {
    ensure_initialised();

    Rcpp::CharacterMatrix categories(numeric_outputs->size(), 2);
    Rcpp::CharacterVector column_names = Rcpp::CharacterVector(2);
    column_names[0] = "Name";
    column_names[1] = "Description";
    Rcpp::colnames(categories) = column_names;

    std::map<std::string, ObjectOutputRegistryBase<double>*>::iterator iter;
    int row = 0;
    for (iter = numeric_outputs->begin(); iter != numeric_outputs->end(); iter++) {
        categories(row, 0) = iter->first;
        categories(row, 1) = iter->second->get_description();
        row++;
    }
    return categories;
}

//'
//' List the names of all valid outputs in the given category
//'
//' @param category_name: Name of the category to which the output
//'                       belongs (see [list_output_categories]).
//'
//' @seealso [list_output_categories]
//' @seealso [get_output]
//'
//' @export
//'
// [[Rcpp::export]]
Rcpp::CharacterVector list_output_names(std::string category_name) {
    ensure_initialised();

    ObjectOutputRegistryBase<double>* registry = get_output_registry(category_name);
    std::vector<std::string> outputs = registry->list_output_names();
    return vec_to_rvec(outputs);
}

//'
//' List the names of all valid objects in the given output category
//'
//' @description
//'
//' Note that the valid objects in a category can change over the
//' course of a simulation; e.g. in the case of individuals it grows
//' due to establishment and mortality.
//'
//' @param category_name: Name of an output category (see
//'                       [list_output_categories]).
//'
//' @seealso [list_output_categories]
//'
//' @export
//'
// [[Rcpp::export]]
Rcpp::CharacterVector list_object_names(std::string category_name) {
    ensure_initialised();

    ObjectOutputRegistryBase<double>* registry = get_output_registry(category_name);
    std::vector<std::string> object_names = registry->list_object_names();
    return vec_to_rvec(object_names);
}

//'
//' List metadata about a single output
//'
//' @description
//'
//' This will return the name, description, and units of the output.
//'
//' @details
//'
//' Data is returned in a character matrix containing one row per
//' output.
//'
//' @param category_name: Name of the category to which the output
//'                       belongs (see [list_output_categories]).
//' @param output_name: Name of the output.
//'
//' @seealso [list_output_categories]
//'
//' @export
//'
// [[Rcpp::export]]
Rcpp::CharacterMatrix list_output_metadata(std::string category_name, std::string output_name) {
    ensure_initialised();

    ObjectOutputRegistryBase<double>* registry = get_output_registry(category_name);

    Rcpp::CharacterMatrix metadata(1, 3);
    Rcpp::CharacterVector column_names = Rcpp::CharacterVector(3);
    column_names[0] = "Name";
    column_names[1] = "Description";
    column_names[2] = "Units";
    Rcpp::colnames(metadata) = column_names;

    metadata[0] = output_name;
    metadata[1] = registry->get_description(output_name);
    metadata[2] = registry->get_units(output_name);
    return metadata;
}

//'
//' Get metadata for all outputs in a certain category
//'
//' @details
//'
//' Data is returned in a character matrix containing one row per
//' output.
//'
//' @param category_name: Name of the category (see
//' [list_output_categories]).
//'
//' @seealso [list_output_categories]
//'
//' @export
//'
// [[Rcpp::export]]
Rcpp::CharacterMatrix list_outputs_in_category(std::string category_name) {
    ensure_initialised();

    ObjectOutputRegistryBase<double>* registry = get_output_registry(category_name);
    int num_rows = registry->size();

    Rcpp::CharacterMatrix metadata = create_output_metadata_table(num_rows);
    store_output_metadata(&metadata, category_name, 0);
    return metadata;
}

//' 
//' Get a table containing metadata for all outputs
//'
//' @description
//'
//' This function will display metadata (category, name, description,
//' and units) for all available outputs in all categories.
//'
//' @seealso [get_output]
//'
//' @export
//'
// [[Rcpp::export]]
Rcpp::CharacterMatrix list_all_outputs() {
    ensure_initialised();

    // We really need to know the total number of rows in advance. The
    // only real alternative is to append each output as a column then
    // return the transpose of this matrix, which obviously isn't ideal.
    int num_rows = count_all_outputs();

    Rcpp::CharacterMatrix metadata = create_output_metadata_table(num_rows);

    int row = 0;
    for (auto it = numeric_outputs->begin(); it != numeric_outputs->end(); it++) {
        std::string category = it->first;
        ObjectOutputRegistryBase<double>* registry = it->second;
        store_output_metadata(&metadata, category, row);
        row += registry->size();
    }

    return metadata;
}
