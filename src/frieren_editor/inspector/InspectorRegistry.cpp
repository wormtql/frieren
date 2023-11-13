#include "InspectorRegistry.h"
#include "all_inspectors.h"
#include <components/all_components.h>

namespace frieren_editor {
    void InspectorRegistry::init() {
        add_registry<Transform, TransformEditor>();
    }

    shared_ptr<ComponentEditor> InspectorRegistry::create_inspector(const string& component_type_name, shared_ptr<Component> c) {
        if (constructors.find(component_type_name) == constructors.end()) {
            return nullptr;
        }
        const auto& f = this->constructors[component_type_name];
        return f(c);
    }
}
