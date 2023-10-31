#include "Material.h"
#include <utilities/BindGroupBuilder.h>

namespace frieren_core {
    Material::Material(shared_ptr<Shader> shader) {
        this->shader = shader;
    }

    Material::~Material() {
        if (this->bind_group) {
            wgpuBindGroupRelease(bind_group);
            bind_group = nullptr;
        }
    }

    void Material::set_property(const string& name, ShaderProperty shader_property) {
        shader_properties[name] = shader_property;
    }

    void Material::set_texture(const string& name, shared_ptr<Texture> tex) {
        shader_textures[name] = tex;
    }

    void Material::set_sampler(const string& name, shared_ptr<Sampler> sampler) {
        shader_samplers[name] = sampler;
    }

    void Material::build_bind_group(WGPUDevice device) {
        if (bind_group != nullptr) {
            wgpuBindGroupRelease(bind_group);
        }

        BindGroupBuilder builder{};
        builder.add_whole_buffer(0, this->shader->get_uniform_buffer());
        const auto& layout = this->shader->get_shader_bind_group_layout_entries_ref();
        for (int i = 1; i < layout.size(); i++) {
            const auto& entry = layout[i];
            const string& name = layout[i].name;
            if (entry.sampler.type != WGPUSamplerBindingType_Undefined) {
                // this is a sampler
                shared_ptr<Sampler> sampler = this->shader_samplers.at(name);
                builder.add_sampler(entry.binding, sampler->get_wgpu_sampler());
            } else if (entry.texture.sampleType != WGPUTextureSampleType_Undefined) {
                // this is a texture
                shared_ptr<Texture> texture = this->shader_textures.at(name);
                builder.add_texture(entry.binding, texture->get_wgpu_texture_view());
            }
        }

        this->bind_group = builder.build(device, this->shader->get_wgpu_bind_group_layout());
    }

    void Material::set_uniform_buffer(WGPUQueue queue) const {
        WGPUBuffer buffer = this->shader->get_uniform_buffer();
        const auto& shader_property_layout = this->shader->get_shader_property_layout_ref();
        unsigned char* cpu_buffer = this->shader->cpu_buffer;

        assert(cpu_buffer != nullptr);

        for (const string& name: shader_property_layout.get_names()) {
            int offset = shader_property_layout.get_item_offset(name).value();
            int size = shader_property_layout.get_item_size(name).value();

            ShaderProperty prop = this->shader_properties.at(name);
            memcpy(cpu_buffer + offset, &prop.value, size);
        }
        wgpuQueueWriteBuffer(queue, buffer, 0, cpu_buffer, shader_property_layout.get_total_size());
    }
}