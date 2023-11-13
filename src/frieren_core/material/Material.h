#ifndef FRIEREN_CORE_MATERIAL_H
#define FRIEREN_CORE_MATERIAL_H

#include <common_include.h>
#include <shader/Shader.h>
#include <shader/ShaderProperty.h>
#include <shader/ShaderManager.h>
#include <texture/Texture.h>
#include <texture/Sampler.h>
#include <texture/SamplerManager.h>
#include <texture/TextureManager.h>

using namespace std;

namespace frieren_core {
    struct MaterialDescriptor {
        string name;
        optional<string> id;
        string shader_id;
        map<string, ShaderProperty> shader_properties;
        map<string, string> shader_textures;
        map<string, string> shader_samplers;
    };

    void from_json(const json& j, MaterialDescriptor& desc);

    class Material {
    private:
        shared_ptr<Shader> shader;
        map<string, ShaderProperty> shader_properties;
        map<string, shared_ptr<Texture>> shader_textures;
        map<string, shared_ptr<Sampler>> shader_samplers;
        string id;
        string name;

        WGPUBindGroup bind_group = nullptr;
    public:
        explicit Material(shared_ptr<Shader> shader, const string& name);
        Material(
            WGPUDevice device,
            WGPUQueue queue,
            const MaterialDescriptor& desc,
            TextureManager& texture_manager,
            SamplerManager& sampler_manager,
            ShaderManager& shader_manager
        );
        Material(const Material& other) = delete;
        ~Material();

        void set_property(const string& name, ShaderProperty property);

        void set_texture(const string& name, shared_ptr<Texture> tex);

        void set_sampler(const string& name, shared_ptr<Sampler> sampler);

        /**
         * @brief build bind group for render pass
         * this should be called if textures or samplers change
         * @param device 
         */
        void build_bind_group(WGPUDevice device);

        /**
         * @brief write shader property into uniform buffer
         * 
         * @param queue 
         */
        void set_uniform_buffer(WGPUQueue queue) const;

        void use_material(WGPURenderPassEncoder render_pass) const;
    };
}

#endif
