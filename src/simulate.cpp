#include "init.h"
#include "model_access.h"
#include "simulate.h"
#include "state.h"
#include "util.h"

// Defined in framework.o (aka framework.cpp).
void simulate_day(Gridcell& gridcell, InputModule* input_module);


//'
//' Simulate a single day
//'
//' @export
//'
// [[Rcpp::export]]
void simulate_day() {
	simulate_day(*state->grid_cell, state->input_module);
	state->grid_cell->balance.check_period(*state->grid_cell);
	date.next();
    if (!state->input_module->getclimate(*state->grid_cell)) {
        // No met data available - abort!
        char buf[256];
        sprintf(buf, "Day simulated successfully, but no more met data remaining (date=%d-%d-%d)", date.get_calendar_year(), date.month, date.day);
        throw std::runtime_error(buf);
    }
}

/*
A predicate implementation which just wraps a C-style function pointer.
Declaring this inside the source file, as it shouldn't really  be used
externally.
*/
class delegate_predicate : public predicate {
    public:
        // Evaluate the predicate function.
        bool evaluate() {
            return delegate();
        }
        // Constructor
        delegate_predicate(bool (*delegate)()) : delegate(delegate) {
        }
    private:
        bool (*delegate)();
};

void simulate_until(bool (*delegate)()) {
    delegate_predicate proxy(delegate);
    simulate_until(&proxy);
}

void simulate_until(predicate* delegate) {
    ensure_initialised();

    while ( !(delegate->evaluate()) ) {
        simulate_day();
    }
}

/*
A predicate implementation which returns true iff at least a certain number
of trees have been established in the stand.
*/
class NumTreePredicate : public predicate {
    public:
        // Constructor.
        NumTreePredicate(int num_trees) : num_trees(num_trees) {
        }
        bool evaluate() {
            int actual_num_trees = get_num_trees();
            return actual_num_trees >= num_trees;
        }
    private:
        int num_trees;
};

//'
//' Simulate until tree establishment occurs
//'
//' @description
//'
//' Run the simulation until a given number of trees have been established.
//'
//' @details
//'
//' The simulation will be run using data indicated by the .ins file.
//'
//' ```
//' initialise(my_ins_file)
//' simulate_until_tree_establishment(1)
//' list_object_names('individual')
//' ```
//'
//' @param num_trees: Simulation will be run until at least this number of trees
//'                   have been established.
//'
//' @export
//'
// [[Rcpp::export]]
void simulate_until_tree_establishment(int num_trees) {
    NumTreePredicate expr(num_trees);
    simulate_until(&expr);
}
