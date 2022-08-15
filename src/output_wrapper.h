#ifndef _LPJGR_OUTPUT_WRAPPER_H_
#define _LPJGR_OUTPUT_WRAPPER_H_

#include <string>

/*
The OutputWrapper class encapsulates a particular output of a particular data
type. These are not really intended to be interacted with directly from R;
instead, they expose functionality such as mapping data types (and names
thereof) to property names and values.
*/
template<class tobj, typename tprop>
class OutputWrapper {
    public:
        /*
        Create a new OutputWrapper instance.
        @param name: Name of the variable/property encapsulated by this class.
        @param description: Description of the variable/property.
        @param unis: Units of the variable/property.
        @param delegate: Delegate function which retrieves the value of the variable.
        */
        OutputWrapper(std::string name, std::string description, std::string units, tprop delegate(const tobj*))
        : variable_name(name),
          description(description),
          units(units),
          delegate(delegate) {
            // Empty
        }

        /*
        Get the property value for an instance of the class type parameter.
        @param instance: The object to be interrogated.
        @return: The value of the property encapsulated by this class.
        */
        tprop get_value(const tobj* instance) {
            return delegate(instance);
        }

        /*
        Get the name of the variable encapsulated by this class.
        */
        std::string get_name() {
            return variable_name;
        }

        /*
        Get info about the specified output.
        */
        std::string get_info() {
            return variable_name + ": " + description + " (" + units + ")";
        }
    private:
        // Name of the variable/property encapsulated by this class.
        std::string variable_name;

        // Description of the variable/property.
        std::string description;

        // Units of the variable/property.
        std::string units;

        // Anonymous function which retrieves the value of the variable for a
        // given instance of the encapsulated type.
        tprop (*delegate)(const tobj*);
};

#endif
