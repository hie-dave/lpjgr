#include "init.h"

#include "guess.h"
#include "commandlinearguments.h"
#include "framework.h"
#include "shell.h"

/*
Run a full LPJ-Guess simulation.
@param input_module_name: Name of the input module to be used.
@param ins_file: Path to the .ins file to be used.
*/
// [[Rcpp::export]]
void run_simulation(std::string input_module_name, std::string ins_file) {
    // This would normally be handled by initialise(), but as we're not calling
    // initialise(), we need to manually initialise the shell.
    initialise_shell();

    CommandLineArguments args(input_module_name, ins_file);
    framework(args);
}
