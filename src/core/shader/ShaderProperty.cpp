#include "ShaderProperty.h"

namespace frieren_core {
    int get_size(ShaderPropertyType ty) {
        if (ty == ShaderPropertyType::Float1) {
            return 4;
        } else if (ty == ShaderPropertyType::Float2) {
            return 8;
        } else if (ty == ShaderPropertyType::Float3) {
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
        } else if (ty == ShaderPropertyType::Float3) {
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
        return ShaderPropertyLayout(types, names);
    }

    ShaderPropertyLayout::ShaderPropertyLayout(vector<ShaderPropertyType> types, vector<string> names) {
        this->types = types;
        this->names = names;
        int len = names.size();
        for (int i = 0; i < len; i++) {
            this->name_to_index[names[i]] = i;
        }

        align_of_struct = 0;
        for (int i = 0; i < len; i++) {
            sizes[i] = get_size(types[i]);
            aligns[i] = get_align(types[i]);
            align_of_struct = max(align_of_struct, aligns[i]);
        }

        calc_offsets();

        int just_past_last = offsets[len - 1] + sizes[len - 1];
        if (just_past_last % align_of_struct == 0) {
            size_of_struct = just_past_last;
            struct_size_padding = 0;
        } else {
            struct_size_padding = align_of_struct - (just_past_last % align_of_struct);
            size_of_struct = just_past_last + struct_size_padding;
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
}

// serde
namespace frieren_core {
    void from_json(const json& j, ShaderPropertyType& ty) {
        string s = j.template get<string>();
        if (s == "Float") {
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
        }
    }

    void from_json(const json& j, ShaderProperty& prop) {
        // todo
        // ShaderPropertyType ty = j["type"].template get<ShaderPropertyType>();
        // prop.ty = ty;

        // if (ty == ShaderPropertyType::Float1) {
        //     float f = j["value"].template get<float>();
        //     prop.value = ShaderPropertyValue {.float_value = f};
        // } else if (ty == ShaderPropertyType)
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