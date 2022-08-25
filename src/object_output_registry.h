#ifndef _LPJGR_OBJECT_OUTPUT_REGISTRY_H_
#define _LPJGR_OBJECT_OUTPUT_REGISTRY_H_

#include <string>
#include <vector>

#include "output_registry.h"
#include "object_output_registry_base.h"

/*
This class extends the OutputRegistry class and provides a generic means by
which we may fetch an output for an arbitrary object which may be identified
by a string (typically a name).
*/
template<class tobj, typename tprop>
class ObjectOutputRegistry : public virtual ObjectOutputRegistryBase<tprop>, public OutputRegistry<tobj, tprop> {
    public:
        /*
        Create a new ObjectOutputRegistry instance.
        @param name: Name of the output registry.
        @param instance_accessor: Anonymous function which will fetch an object
                                  (tobj*) instance given a string (name).
        @param instance_enumerator: Anonymous function  which will fetch the names
                                    of all objects encapsulated by this registry.
        */
        ObjectOutputRegistry(std::string name, tobj *(*instance_accessor)(std::string object_name), std::vector<std::string> (*instance_enumerator)())
            : instance_accessor(instance_accessor)
            , instance_enumerator(instance_enumerator)
            , OutputRegistry<tobj, tprop>(name) {
        }

        /*
        Get a property value encapsulated by this registry for an object
        instance with the given name or identifier.

        @param object_name: Name of the object.
        @param output_name: Name of the output of the object.
        */
        tprop get_output(std::string object_name, std::string output_name) {
            OutputRegistry<tobj, tprop>::ensure_contains(output_name);
            tobj* instance = instance_accessor(object_name);
            return OutputRegistry<tobj, tprop>::get_output(instance, output_name);
        }

        /*
        Get a human-readable description of the output registry.
        */
        std::string get_description() {
            return OutputRegistry<tobj, tprop>::name;
        }

        /*
        Return the names of all valid objects encapsulated by this registry.
        */
        std::vector<std::string> list_object_names() {
            return instance_enumerator();
        }
    private:
        // Anonymous function which retrieves the a tobj instance given
        // a particular string (usually an object name).
        tobj *(*instance_accessor)(std::string object_name);

        /*
        Anonymous function which returns the names of all valid objects
        encapsulated by this output registry.
        */
        std::vector<std::string> (*instance_enumerator)();
};

#endif // _LPJGR_OBJECT_OUTPUT_REGISTRY_H_
