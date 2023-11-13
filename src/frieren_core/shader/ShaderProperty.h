#ifndef FRIEREN_CORE_SHADER_PROPERTY
#define FRIEREN_CORE_SHADER_PROPERTY

#include <common_include_glm.h>
#include <common_include.h>
#include <webgpu/webgpu.hpp>

using namespace std;

namespace frieren_core {
    enum class ShaderPropertyType {
        Float4,
        Float3,
        Float2,
        Float1,
        Uint32,
        Mat4x4,
    };

    void from_json(const json& j, ShaderPropertyType& ty);

    int get_align(ShaderPropertyType ty);
    int get_size(ShaderPropertyType ty);

    union ShaderPropertyValue {
        glm::vec4 float4_value;
        glm::vec3 float3_value;
        glm::vec2 float2_value;
        float float_value;
        uint32_t uint32_value;
        glm::mat4x4 mat4x4_value;
    };

    struct ShaderProperty {
        ShaderPropertyType ty;
        ShaderPropertyValue value;
    };

    class ShaderPropertyLayout {
    private:
        vector<ShaderPropertyType> types;
        vector<string> names;
        map<string, int> name_to_index;

        vector<int> offsets;
        vector<int> sizes;
        vector<int> aligns;
        int align_of_struct = 0;
        int size_of_struct = 0;
        int struct_size_padding = 0;

        void calc_offsets();
    public:
        ShaderPropertyLayout() = default;
        ShaderPropertyLayout(vector<ShaderPropertyType> types, vector<string> names);

        int get_total_size() const;

        std::optional<int> get_item_offset(const string& name) const;

        std::optional<int> get_item_size(const string& name) const;

        const vector<string>& get_names() const;
    };

    void from_json(const json& j, ShaderProperty& prop);

    class ShaderPropertyLayoutBuilder {
    private:
        vector<ShaderPropertyType> types;
        vector<string> names;
    public:
        ShaderPropertyLayoutBuilder() = default;

        void add_entry(const string& name, ShaderPropertyType ty);

        ShaderPropertyLayout build() const;
    };

    void from_json(const json& j, ShaderPropertyLayoutBuilder& builder);
}

#endif
