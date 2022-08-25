#ifndef _LPJGR_SIMULATION_HELPERS_H_
#define _LPJGR_SIMULATION_HELPERS_H_

/*
An interface for classes which encapsulate a predicate expression. I've used
this rather than function pointers for extra flexibility.
*/
class predicate {
    public:
        virtual bool evaluate() = 0;
};

/*
Run the simulation until the given delegate returns true. This function can
return partway through the day.

Note that only the first patch and stand are simulated.

@param delegate: Function pointer which is called each day and should return true to stop the simulation.
*/
void simulate_until(bool (*delegate)());

/*
Run the simulation until the given delegate returns true. This function can
return partway through the day.

Note that only the first patch and stand are simulated.

@param delegate: Any predicate expression which should return true to halt simulation execution.
*/
void simulate_until(predicate* delegate);

#endif // _LPJGR_SIMULATION_HELPERS_H_
