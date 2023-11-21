#ifndef FRIEREN_EDITOR_MESH_FILTER_EDITOR_H
#define FRIEREN_EDITOR_MESH_FILTER_EDITOR_H

#include "../ComponentEditor.h"
#include <components/all_components.h>
#include <string>
#include <memory>

using namespace std;
using frieren_core::component::MeshFilter;

namespace frieren_editor {
    class MeshFilterEditor: public ComponentEditor {
    private:
        string mesh_name;
        string mesh_id;
    public:
        MeshFilterEditor(shared_ptr<MeshFilter> component);

        void draw() override;

    };
}

#endif
