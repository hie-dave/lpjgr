#ifndef _LPJGR_OBJECT_OUTPUT_REGISTRY_BASE_H_
#define _LPJGR_OBJECT_OUTPUT_REGISTRY_BASE_H_

#include <string>
#include <vector>

#include "output_registry_base.h"

/*
This interface encapsulates all classes which may retrieve a property value given
a property name and an object name.
*/
template<typename tprop>
class ObjectOutputRegistryBase : public virtual OutputRegistryBase {
    public:
        /*
        Get a property value encapsulated by this registry for an object
        instance with the given name or identifier.

        @param object_name: Name of the object.
        @param output_name: Name of the output of the object.
        */
        virtual tprop get_output(std::string object_name, std::string output_name) = 0;

        /*
        Return the names of all valid objects encapsulated by this registry.
        */
        virtual std::vector<std::string> list_object_names() = 0;
};

#endif // _LPJGR_OBJECT_OUTPUT_REGISTRY_BASE_H_
