#ifndef _LPJGR_OUTPUT_REGISTRY_BASE_H_
#define _LPJGR_OUTPUT_REGISTRY_BASE_H_

#include <string>
#include <vector>

/*
Methods common to all output registries.
*/
class OutputRegistryBase {
    public:
        /*
        Return the names of all available outputs.
        */
        virtual std::vector<std::string> list_output_names() = 0;

        /*
        Get a human-readable description of the output registry.
        */
        virtual std::string get_description() = 0;

        /*
        Get units for the specified output.
        @param output_name: Name of the output.
        */
        virtual std::string get_units(std::string output_name) = 0;

        /*
        Get a description of the specified output.
        @param output_name: Name of the output.
        */
        virtual std::string get_description(std::string output_name) = 0;

        /*
        Return the size of the output registry.
        */
        virtual int size() const = 0;
};

#endif // _LPJGR_OUTPUT_REGISTRY_BASE_H_
