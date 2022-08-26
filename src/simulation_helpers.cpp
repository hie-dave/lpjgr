#include "init.h"
#include "simulate.h"
#include "simulation_helpers.h"
#include "state.h"
#include "util.h"

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
    unsigned int output_freq = 1;
    unsigned int num_days_simulated = 0;

    while ( !(delegate->evaluate()) ) {
        // Need to finish with pre_canexch(). That way, user can change
        // daily met data (which is normally overriden in getclimate()).
        // This is a bit of a hack, really. Need to rethink this.
        canexch();
        post_canexch();
        pre_canexch();

        num_days_simulated++;
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
        // Get the number of trees established in the stand.
        // todo: move this somewhere better.
        int get_num_trees() {
            return count<Individual, Pft, Vegetation>(&(patch->vegetation), [](const Individual* indiv) -> bool {
                return indiv->pft.lifeform == TREE;
            });
        }
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
