#ifndef FRIEREN_EDITOR_INSPECTOR_REGISTRY_H
#define FRIEREN_EDITOR_INSPECTOR_REGISTRY_H

#include <map>
#include <memory>
#include <functional>
#include <string>

#include <utilities/utils.h>
#include "ComponentEditor.h"

using namespace std;
using namespace frieren_core;

namespace frieren_editor {
    class InspectorRegistry {
    private:
        using ConstructorType = function<shared_ptr<ComponentEditor>(shared_ptr<Component>)>;
        map<string, ConstructorType> constructors;
    public:
        template<typename ComponentType, typename EditorType>
        void add_registry();

        void init();

        template<typename T>
        shared_ptr<ComponentEditor> create_inspector(shared_ptr<T> component);

        shared_ptr<ComponentEditor> create_inspector(const string& component_type_name, shared_ptr<Component> c);
    };

    template<typename ComponentType, typename EditorType>
    void InspectorRegistry::add_registry() {
        string component_name = utils::get_type_name<ComponentType>();
        auto constructor = [] (shared_ptr<Component> component) -> shared_ptr<ComponentEditor> {
            shared_ptr<ComponentType> c2 = dynamic_pointer_cast<ComponentType>(component);
            shared_ptr<EditorType> editor = make_shared<EditorType>(c2);
            return editor;
            // return nullptr;
        };
        this->constructors[component_name] = constructor;
    }

    template<typename T>
    shared_ptr<ComponentEditor> InspectorRegistry::create_inspector(shared_ptr<T> component) {
        string component_name = utils::get_type_name<T>();
        shared_ptr<Component> c2 = dynamic_pointer_cast<Component>(component);

        shared_ptr<ComponentEditor> editor = this->constructors[component_name](c2);
        editor->related_component = c2;
        return editor;
    }
}

#endif
