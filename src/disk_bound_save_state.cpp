#include <stdio.h>

#include "guess.h"
#include "guessserializer.h"

#include "disk_bound_save_state.h"
#include "init.h"
#include "simulation_state.h"
#include "state.h"

struct MetadataFile {
	// Read metadata from current simulation state.
	MetadataFile();

	// Read metadata from a directory written to by save().
	// @param dir: Directory containing the metadata file.
	MetadataFile(std::string dir);

	int year;
	int dayofyear;
	TemporalState temporal_state;
	std::string instruction_file;

	// Save metadata information to the given filename.
	// @param filename: Directory to which the metadata file will be saved.
	void save(std::string dir);

	private:
		// Metadata filename.
		const std::string META_FILE_NAME = "lpjgr.meta.bin";

		// Get the name of the metadata file in the given directory.
		// @param dir: Directory containing the metadata file.
		std::string get_meta_path(std::string dir);
};

MetadataFile::MetadataFile() {
	year = state->date->year;
	dayofyear = state->date->day;
	temporal_state = state->temporal_state;
	instruction_file = state->instruction_file;
}

MetadataFile::MetadataFile(std::string dir) {
	// 0. Open the file.
	std::string meta_file_name = get_meta_path(dir);
	FILE* meta_file = fopen(meta_file_name.c_str(), "r");
	if (!meta_file) {
		throw std::runtime_error("Unable to open metadata file '"
		+ meta_file_name + "' for reading: " + strerror(errno));
	}

	// 1. Parse the file.
	int err = fscanf(
		meta_file,
		"%d-%d\n%d\n%s",
		year,
		dayofyear,
		temporal_state,
		instruction_file);

	// 2. Close the file.
	err = fclose(meta_file);
	if (err != 0) {
		throw std::runtime_error("Failed to close metadata file '"
		+ meta_file_name + "': " + strerror(errno));
	}

}

void MetadataFile::save(std::string dir) {
	// 0. Open the file.
	std::string meta_file_name = get_meta_path(dir);
	FILE* meta_file = fopen(meta_file_name.c_str(), "w");
	if (!meta_file) {
		throw std::runtime_error("Unable to open metadata file '"
		+ meta_file_name + "' for writing: " + strerror(errno));
	}

	// 1. Write to the file.
	int err = fprintf(meta_file,
		"%d-%d\n%d\n%s",
		year,
		dayofyear,
		temporal_state,
		instruction_file);
	if (err < 0) {
		throw std::runtime_error("Unable to write to metadata file '"
		+ meta_file_name + "': " + strerror(errno));
	}

	// 2. Close the file.
	err = fclose(meta_file);
	if (err != 0) {
		throw std::runtime_error("Unable to close metadata file '"
		+ meta_file_name + "': " + strerror(errno));
	}
}

std::string MetadataFile::get_meta_path(std::string dir) {
	return dir + "/" + META_FILE_NAME;
}

DiskBoundSaveState::DiskBoundSaveState(std::string directory)
: dirname(directory) {
	// Do nothing
}

/*
Save the current state of the simulation.
*/
void DiskBoundSaveState::save() const {
	// Serialize grid cell.
	GuessSerializer serialiser(dirname.c_str(), 0, 1);
    serialiser.serialize_gridcell(*state->grid_cell);

	// Write some extra metadata which isn't included in the standard
	// lpj-guess state file. Note that the default constructor here
	// reads the metadata from the current environment.
	MetadataFile metadata;
	metadata.save(dirname);
}

/*
Load the previously-saved simulation state.

@param state: (out param): the new simulation state.
*/
void DiskBoundSaveState::Load(SimulationState* state) const {
	if (!state) {
		throw std::runtime_error("Programmer error: must initialize state before calling load()");
	}

	// 0. Read metadata file (this constructor will parse the file).
	MetadataFile metadata(dirname);

	// 1. Re-initialise from instruction file.
	initialise_state(metadata.instruction_file, state);

	// 2. Deserialise the grid cell from the lpj-guess statefile.
	GuessDeserializer deserialiser(dirname.c_str());
    deserialiser.deserialize_gridcell(*state->grid_cell);

	// 3. Ideally, we would now create and initialise a new date object.
	//    Unfortunately, the date variable used by all of the lpjg
	//    infrastructure is a plain old global variable on the runtime
	//    stack. This makes it very difficult to abstract out. For now,
	//    we're going to play with fire by just storing the address of
	//    date, and praying that everything is behaving properly.
	date.init(1);
	initialise_date(&date, metadata.year, metadata.dayofyear);

	// 4. Store results in the state object.
	state->date = &date;
	state->instruction_file = metadata.instruction_file;
	state->temporal_state = static_cast<TemporalState>(metadata.temporal_state);
}
