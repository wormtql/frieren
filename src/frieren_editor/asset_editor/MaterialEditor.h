#ifndef FRIEREN_EDITOR_MATERIAL_EDITOR_H
#define FRIEREN_EDITOR_MATERIAL_EDITOR_H

#include <material/Material.h>
#include <shader/ShaderProperty.h>
#include <vector>
#include <string>
#include <memory>

using namespace frieren_core;
using namespace std;

namespace frieren_editor {
    class MaterialEditor {
    private:
        shared_ptr<Material> related_material;

        vector<string> shader_property_names;
        vector<ShaderProperty> shader_property_values;
        
        vector<string> shader_texture_names;
        vector<string> shader_texture_texture_ids;

        vector<string> shader_sampler_names;
        vector<string> shader_sampler_sampler_ids;
    
    public:
        MaterialEditor(shared_ptr<Material> mat);

        void draw();
    };
}

#endif
