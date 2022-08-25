#ifndef _LPJGR_UTIL_H_
#define _LPJGR_UTIL_H_

#include <string>
#include <vector>
#include "Rcpp.h"

#include "gutil.h"

/*
Count the number of items in the list for which the given delegate expression
returns true.

@param list: The list of items.
@param delegate: A function pointer which returns true for all elements to be counted.
*/
template<typename tdata, typename tref1, typename tref2>
int count(ListArray_idin2<tdata, tref1, tref2>* list, bool (*delegate)(const tdata*)) {
    list->firstobj();
    int n = 0;
    while (list->isobj) {
        if (delegate(&(list->getobj()))) {
            n++;
        }
        list->nextobj();
    }
    return n;
}

/*
Return true iff the map contains the given key.
*/
template<typename tkey, typename tvalue>
bool contains(std::map<tkey, tvalue>* dict, tkey key) {
    return dict->count(key) > 0;
}

/*
Sum all elements in a map using the given element selector.

@param map: A map.
@param selector: A function to add a value to the sum for a particular element.
*/
template<typename tkey, typename tvalue>
int sum(const std::map<tkey, tvalue*>* map, int (*selector)(const tvalue*)) {
    // todo: should make this useful for any stl container. Also need
    // to think about whether tvalue should really be a pointer type...
    int n = 0;
    for (auto it = map->begin(); it != map->end(); it++) {
        n += selector(it->second);
    }
    return n;
}

/*
Convert a vector to an Rcpp vector (string vectors only).
*/
Rcpp::CharacterVector vec_to_rvec(std::vector<std::string> vec);

#endif // _LPJGR_UTIL_H_
