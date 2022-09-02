#ifndef _LPJGR_OUTPUT_REGISTRY_H_
#define _LPJGR_OUTPUT_REGISTRY_H_

#include <map>
#include <string>
#include <vector>

#include "output_wrapper.h"
#include "output_registry_base.h"
#include "util.h"

template<class tobj, typename tprop>
class OutputRegistry : public virtual OutputRegistryBase {
    private:
        std::map<std::string, OutputWrapper<tobj, tprop>*> output_map;
    protected:
        std::string name;

        /*
        Throw an exception if this output registry does not contain the
        specified output.
        @param name: Name of the output.
        */
        void ensure_contains(std::string name) {
            // fixme - we're iterating over the map twice. This should return an inter.
            if (!contains(&output_map, name)) {
                throw std::runtime_error("Output registry '" + this->name + "' does not contain output '" + name + "'");
            }
        }
    public:
        /*
        Create a new OutputRegistry instance.
        @param name: Name of the output registry.
        */
        OutputRegistry(std::string name) : name(name) {
            // Empty.
        }

        /*
        Cleanup the vector (note to self: call outputs.clear()).
        */
        ~OutputRegistry()  {
            output_map.clear();
        }

        /*
        Register an output wrapper for use in this output registry.
        */
        void register_output(OutputWrapper<tobj, tprop>* output) {
            output_map.insert(std::make_pair(output->get_name(), output));
        }

        /*
        Get a property value encapsulated by this registry for the given object.
        */
        virtual tprop get_output(tobj* object, std::string name) {
            ensure_contains(name);
            return output_map[name]->get_value(object);
        }

        /*
        Return the names of all available outputs.
        */
        std::vector<std::string> list_output_names() {
            std::vector<std::string> result;
            for (auto iter = output_map.begin(); iter != output_map.end(); iter++) {
                result.push_back(iter->first);
            }
            return result;
        }

        /*
        Get units for the specified output.
        @param output_name: Name of the output.
        */
        std::string get_units(std::string output_name) {
            ensure_contains(output_name);
            return output_map[output_name]->get_units();
        }

        /*
        Get a description of the specified output.
        @param output_name: Name of the output.
        */
        virtual std::string get_description(std::string output_name) {
            ensure_contains(output_name);
            return output_map[output_name]->get_description();
        }

        /*
        Return the size of the output registry.
        */
        int size() const {
            return output_map.size();
        }
};

#endif // _LPJGR_OUTPUT_REGISTRY_H_
