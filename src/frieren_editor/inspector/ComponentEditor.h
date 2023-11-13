#ifndef FRIEREN_CORE_EDITOR_COMPONENT_EDITOR_H
#define FRIEREN_CORE_EDITOR_COMPONENT_EDITOR_H

#include <components/Component.h>
#include <common_include.h>

using namespace frieren_core;
using namespace frieren_core::component;

namespace frieren_editor {
    class ComponentEditor {
    public:
        shared_ptr<Component> related_component{};

        template<typename T>
        ComponentEditor(shared_ptr<T> component);

        virtual void draw() {};
    };

    template<typename T>
    ComponentEditor::ComponentEditor(shared_ptr<T> component) {
        static_assert(std::is_base_of<Component, T>::value, "must be a base of Component");
        this->related_component = dynamic_pointer_cast<Component>(component);
    }
}

#endif
