#ifndef FRIEREN_EDITOR_MESH_RENDERER_EDITOR_H
#define FRIEREN_EDITOR_MESH_RENDERER_EDITOR_H

#include "../ComponentEditor.h"
#include <components/all_components.h>
#include <vector>
#include <asset_editor/MaterialEditor.h>

using namespace std;
using frieren_core::component::MeshRenderer;

namespace frieren_editor {
    class MeshRendererEditor: public ComponentEditor {
    private:
        vector<MaterialEditor> material_editors;
    public:
        MeshRendererEditor(shared_ptr<MeshRenderer> component);

        void draw() override;
    };
}

#endif
