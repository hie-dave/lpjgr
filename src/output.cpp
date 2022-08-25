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

// --------------------------------------------------------------------
// User-callable (ie from R) functions.
// --------------------------------------------------------------------

/*
Get an output with a given name, of an object with a given name, from an output
category with a given name.

@param category_name: Name of the output category.
@param object_name: Name of the object.
@param property_name: Name of the property.
*/
// [[Rcpp::export]]
double get_output(std::string category_name, std::string object_name, std::string property_name) {
    ensure_initialised();
    ObjectOutputRegistryBase<double>* registry = get_output_registry(category_name);
    return registry->get_output(object_name, property_name);
}

/*
List all valid output categories.
*/
// [[Rcpp::export]]
Rcpp::CharacterMatrix list_output_categories() {
    ensure_initialised();

    Rcpp::CharacterMatrix categories(numeric_outputs->size(), 2);
    Rcpp::CharacterVector column_names = Rcpp::CharacterVector(2);
    column_names[0] = "Name";
    column_names[1] = "Description";
    Rcpp::colnames(categories) = column_names;

    std::map<std::string, ObjectOutputRegistryBase<double>*>::iterator iter;
    int i = 0;
    for (iter = numeric_outputs->begin(); iter != numeric_outputs->end(); iter++) {
        categories[i++] = iter->first;
        categories[i++] = iter->second->get_description();
    }
    return categories;
}

/*
List the names of all valid outputs in the given category.

@param category_name: Name of the category to which the output belongs.
*/
// [[Rcpp::export]]
Rcpp::CharacterVector list_output_names(std::string category_name) {
    ensure_initialised();

    ObjectOutputRegistryBase<double>* registry = get_output_registry(category_name);
    std::vector<std::string> outputs = registry->list_output_names();
    return vec_to_rvec(outputs);
}

/*
List the names of all valid objects in the given output category.

@param category_name: Name of an output category (see `list_category_names()`).
*/
// [[Rcpp::export]]
Rcpp::CharacterVector list_object_names(std::string category_name) {
    ensure_initialised();

    ObjectOutputRegistryBase<double>* registry = get_output_registry(category_name);
    std::vector<std::string> object_names = registry->list_object_names();
    return vec_to_rvec(object_names);
}

/*
List metadata about a single output.
*/
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

/*
Get a table containing metadata for all outputs in a certain category.
*/
// [Rcpp::export]
Rcpp::CharacterMatrix list_outputs_in_category(std::string category_name) {
    ensure_initialised();

    ObjectOutputRegistryBase<double>* registry = get_output_registry(category_name);
    int num_rows = registry->size();

    Rcpp::CharacterMatrix metadata = create_output_metadata_table(num_rows);
    store_output_metadata(&metadata, category_name, 0);
    return metadata;
}

//' 
//' Get a table containing metadata for all outputs.
//' @export
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
