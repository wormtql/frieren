#ifndef FRIEREN_CORE_MATERIAL_H
#define FRIEREN_CORE_MATERIAL_H

#include <common_include.h>
#include <shader/Shader.h>
#include <shader/ShaderProperty.h>
#include <texture/Texture.h>
#include <texture/Sampler.h>

using namespace std;

namespace frieren_core {
    class Material {
    private:
        shared_ptr<Shader> shader;
        map<string, ShaderProperty> shader_properties;
        map<string, shared_ptr<Texture>> shader_textures;
        map<string, shared_ptr<Sampler>> shader_samplers;

        WGPUBindGroup bind_group = nullptr;
    public:
        explicit Material(shared_ptr<Shader> shader);
        ~Material();

        void set_property(const string& name, ShaderProperty property);

        void set_texture(const string& name, shared_ptr<Texture> tex);

        void set_sampler(const string& name, shared_ptr<Sampler> sampler);

        // it should be called if textures or samplers change
        void build_bind_group(WGPUDevice device);

        void set_uniform_buffer(WGPUQueue queue) const;
    };
}

#endif
