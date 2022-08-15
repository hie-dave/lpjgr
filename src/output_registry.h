#include <map>
#include <vector>

#include "output_wrapper.h"

template<class tobj, typename tprop>
class OutputRegistry {
    private:
        std::map<std::string, OutputWrapper<tobj, tprop>*> output_map;
        std::string name;

        /*
        Throw an exception if this output registry does not contain the
        specified output.
        @param name: Name of the output.
        */
        void ensure_contains(std::string name) {
            if (output_map.count(name) == 0) {
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
        tprop get_output(tobj* object, std::string name) {
            ensure_contains(name);
            return output_map[name]->get_value(object);
        }

        /*
        Check if this registry contains an output with the specified name.
        @param name: Name of the output.
        */
        bool contains_output(std::string name) {
            return output_map.count(name) > 0;
        }

        /*
        Return the names of all available outputs.
        */
        std::vector<std::string> get_output_names() {
            std::vector<std::string> result;
            for (auto iter = output_map.begin(); iter != output_map.end(); iter++) {
                result.push_back(iter->first);
            }
            return result;
        }

        /*
        Get metadata about the specified output.
        @param output_name: Name of the output.
        */
        std::string get_output_info(std::string output_name) {
            ensure_contains(output_name);
            return output_map[output_name]->get_info();
        }
};
