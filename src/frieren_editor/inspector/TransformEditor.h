#ifndef FRIEREN_EDITOR_TRANSFORM_EDITOR_H
#define FRIEREN_EDITOR_TRANSFORM_EDITOR_H

#include "ComponentEditor.h"
#include <components/all_components.h>
#include <common_include.h>
#include <common_include_glm.h>

using frieren_core::component::Transform;
using namespace std;

namespace frieren_editor {
    class TransformEditor: public ComponentEditor {
    public:
        glm::vec3 position;
        glm::vec3 rotation;
        float scale;

        void draw() override;

        TransformEditor(shared_ptr<Transform> transform);
    };
}

#endif