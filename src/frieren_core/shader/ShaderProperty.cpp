#include "ShaderProperty.h"
#include <glm_serde.h>

namespace frieren_core {
    int get_size(ShaderPropertyType ty) {
        if (ty == ShaderPropertyType::Float1) {
            return 4;
        } else if (ty == ShaderPropertyType::Float2) {
            return 8;
        } else if (ty == ShaderPropertyType::Float3 || ty == ShaderPropertyType::Color3) {
            return 12;
        } else if (ty == ShaderPropertyType::Float4) {
            return 16;
        } else if (ty == ShaderPropertyType::Uint32) {
            return 4;
        } else if (ty == ShaderPropertyType::Mat4x4) {
            return 64;
        }
        return 0;
    }

    int get_align(ShaderPropertyType ty) {
        if (ty == ShaderPropertyType::Float1) {
            return 4;
        } else if (ty == ShaderPropertyType::Float2) {
            return 8;
        } else if (ty == ShaderPropertyType::Float3 || ty == ShaderPropertyType::Color3) {
            return 16;
        } else if (ty == ShaderPropertyType::Float4) {
            return 16;
        } else if (ty == ShaderPropertyType::Uint32) {
            return 4;
        } else if (ty == ShaderPropertyType::Mat4x4) {
            return 16;
        }
        return 0;
    }

    void ShaderPropertyLayoutBuilder::add_entry(const string& name, ShaderPropertyType ty) {
        names.push_back(name);
        types.push_back(ty);
    }

    ShaderPropertyLayout ShaderPropertyLayoutBuilder::build() const {
        return std::move(ShaderPropertyLayout(types, names));
    }

    ShaderPropertyLayout::ShaderPropertyLayout(vector<ShaderPropertyType> types, vector<string> names) {
        this->types = types;
        this->names = names;
        int len = names.size();
        for (int i = 0; i < len; i++) {
            this->name_to_index[names[i]] = i;
        }

        this->sizes.resize(len);
        this->aligns.resize(len);

        align_of_struct = 0;
        for (int i = 0; i < len; i++) {
            sizes[i] = get_size(types[i]);
            aligns[i] = get_align(types[i]);
            align_of_struct = max(align_of_struct, aligns[i]);
        }

        calc_offsets();

        if (len != 0) {
            int just_past_last = offsets[len - 1] + sizes[len - 1];
            if (just_past_last % align_of_struct == 0) {
                size_of_struct = just_past_last;
                struct_size_padding = 0;
            } else {
                struct_size_padding = align_of_struct - (just_past_last % align_of_struct);
                size_of_struct = just_past_last + struct_size_padding;
            }
        }
    }

    void ShaderPropertyLayout::calc_offsets() {
        int p = 0;
        int len = names.size();
        for (int i = 0; i < len; i++) {
            int size = sizes[i];
            int align = aligns[i];

            if (p % align == 0) {
                offsets.push_back(p);
                p += size;
            } else {
                int padding = align - (p % align);
                offsets.push_back(p + padding);
                p += padding + size;
            }
        }
    }

    int ShaderPropertyLayout::get_total_size() const {
        return size_of_struct;
    }

    std::optional<int> ShaderPropertyLayout::get_item_offset(const string& name) const {
        if (!this->name_to_index.contains(name)) {
            return {};
        }
        int index = this->name_to_index.at(name);
        return offsets[index];
    }

    std::optional<int> ShaderPropertyLayout::get_item_size(const string& name) const {
        if (!this->name_to_index.contains(name)) {
            return {};
        }
        int index = this->name_to_index.at(name);
        return sizes[index];
    }

    const std::vector<string>& ShaderPropertyLayout::get_names() const {
        return names;
    }

    ShaderProperty::ShaderProperty(float value) {
        this->ty = ShaderPropertyType::Float1;
        this->value.float_value = value;
    }

    ShaderProperty::ShaderProperty(glm::vec2 value) {
        ty = ShaderPropertyType::Float2;
        this->value.float2_value = value;
    }

    ShaderProperty::ShaderProperty(glm::vec3 value) {
        ty = ShaderPropertyType::Float3;
        this->value.float3_value = value;
    }

    ShaderProperty::ShaderProperty(glm::vec4 value) {
        ty = ShaderPropertyType::Float4;
        this->value.float4_value = value;
    }
}

// serde
namespace frieren_core {
    void from_json(const json& j, ShaderPropertyType& ty) {
        string s = j.template get<string>();
        if (s == "Float" || s == "Float1") {
            ty = ShaderPropertyType::Float1;
        } else if (s == "Float2") {
            ty = ShaderPropertyType::Float2;
        } else if (s == "Float3") {
            ty = ShaderPropertyType::Float3;
        } else if (s == "Float4") {
            ty = ShaderPropertyType::Float4;
        } else if (s == "Uint32") {
            ty = ShaderPropertyType::Uint32;
        } else if (s == "Mat4x4") {
            ty = ShaderPropertyType::Mat4x4;
        } else if (s == "Color3") {
            ty = ShaderPropertyType::Color3;
        }
    }

    void from_json(const json& j, ShaderProperty& prop) {
        ShaderPropertyType ty = j["type"].template get<ShaderPropertyType>();
        prop.ty = ty;

        if (ty == ShaderPropertyType::Float1) {
            float f = j["value"].template get<float>();
            prop.value.float_value = f;
        } else if (ty == ShaderPropertyType::Float2) {
            glm::vec2 f = j["value"].template get<glm::vec2>();
            prop.value.float2_value = f;
        } else if (ty == ShaderPropertyType::Float3 || ty == ShaderPropertyType::Color3) {
            prop.value.float3_value = j["value"].template get<glm::vec3>();
        } else if (ty == ShaderPropertyType::Float4) {
            prop.value.float4_value = j["value"].template get<glm::vec4>();
        } else if (ty == ShaderPropertyType::Uint32) {
            prop.value.uint32_value = j["value"].template get<uint32_t>();
        } else if (ty == ShaderPropertyType::Mat4x4) {
            prop.value.mat4x4_value = j["value"].template get<glm::mat4x4>();
        }
    }

    void from_json(const json& j, ShaderPropertyLayoutBuilder& builder) {
        assert(j.is_array());
        for (const auto& item: j) {
            string name = item["name"].template get<string>();
            ShaderPropertyType ty = item["type"].template get<ShaderPropertyType>();
            builder.add_entry(name, ty);
        }
    }
}