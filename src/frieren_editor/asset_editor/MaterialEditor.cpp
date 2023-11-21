#include "MaterialEditor.h"
#include <imgui.h>

namespace frieren_editor {
    MaterialEditor::MaterialEditor(shared_ptr<Material> mat) {
        this->related_material = mat;

        for (const auto& sp: mat->shader_properties) {
            this->shader_property_names.push_back(sp.first);
            this->shader_property_values.push_back(sp.second);
        }

        for (const auto& st: mat->shader_textures) {
            this->shader_texture_names.push_back(st.first);
            this->shader_texture_texture_ids.push_back(st.second->get_id());
        }

        for (const auto& ss: mat->shader_samplers) {
            this->shader_sampler_names.push_back(ss.first);
            // todo
            this->shader_sampler_sampler_ids.push_back("todo");
            // this->shader_sampler_sampler_ids.push_back(ss.second->)
        }
    }

    void MaterialEditor::draw() {
        ImGui::Text("Shader Properties");
        for (int i = 0; i < shader_property_names.size(); i++) {
            auto ty = shader_property_values[i].ty;
            float* value_ptr = reinterpret_cast<float*>(&shader_property_values[i].value);
            ShaderPropertyValue* shader_value_ptr = reinterpret_cast<ShaderPropertyValue*>(value_ptr);
            const string& name = shader_property_names[i];
            if (ty == ShaderPropertyType::Float1) {
                if (ImGui::InputFloat(name.c_str(), value_ptr)) {
                    this->related_material->set_property(name, ShaderProperty{*value_ptr});
                }
            } else if (ty == ShaderPropertyType::Float2) {
                if (ImGui::InputFloat2(name.c_str(), value_ptr)) {
                    this->related_material->set_property(name, ShaderProperty{shader_value_ptr->float2_value});
                }
            } else if (ty == ShaderPropertyType::Float3) {
                if (ImGui::InputFloat3(name.c_str(), value_ptr)) {
                    this->related_material->set_property(name, ShaderProperty{shader_value_ptr->float3_value});
                }
            } else if (ty == ShaderPropertyType::Float4) {
                if (ImGui::InputFloat4(name.c_str(), value_ptr)) {
                    this->related_material->set_property(name, ShaderProperty{shader_value_ptr->float4_value});
                }
            } else if (ty == ShaderPropertyType::Color3) {
                if (ImGui::ColorEdit3(name.c_str(), value_ptr)) {
                    this->related_material->set_property(name, ShaderProperty{
                        ShaderPropertyType::Color3,
                        *shader_value_ptr }
                    );
                }
            } else {
                // todo
                assert(false);
            }
        }

        // todo textures

        // todo samplers
    }
}