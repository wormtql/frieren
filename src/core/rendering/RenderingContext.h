#ifndef FRIEREN_CORE_RENDERING_CONTEXT_H
#define FRIEREN_CORE_RENDERING_CONTEXT_H

#include <webgpu/webgpu.hpp>
#include <common_include.h>
#include <common_include_glm.h>
#include <texture/Texture.h>
#include <mesh/Mesh.h>
#include <material/Material.h>
#include "BuiltinBindGroupLayout.h"

using namespace std;

namespace frieren_core {
    class RenderingContext {
    private:
        // wgpu
        WGPUDevice device;
        WGPUQueue queue;

        // rendering
        shared_ptr<Texture> target_color_textures[8];
        int color_attachment_size;
        optional<shared_ptr<Texture>> target_depth_textures;

        BuiltinBindGroup builtin_bind_group;
    public:
        RenderingContext();

        /**
         * @brief set target color texture
         * 
         */
        void set_target_color_texture(shared_ptr<Texture> texture, int slot);

        void set_color_attachments_size(int size) {
            this->color_attachment_size = size;
        }

        void set_target_depth_texture(shared_ptr<Texture> texture);

        void remove_target_depth_texture();

        /**
         * @brief init rendering context
         * 1. set member variable device and queue
         * 2. init builtin bind group
         * 
         * @param device 
         * @param queue 
         */
        void init(WGPUDevice device, WGPUQueue queue);

        void draw_mesh(
            shared_ptr<Mesh> mesh, const glm::mat4x4& transform_matrix, shared_ptr<Material> material);
    };
}

#endif
