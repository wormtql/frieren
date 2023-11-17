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
    public:
        // wgpu
        WGPUDevice device;
        WGPUQueue queue;

        // rendering
        WGPUTextureView target_color_textures[8];
        int color_attachment_size;
        optional<WGPUTextureView> target_depth_textures;
        optional<WGPUTextureView> surface_texture_view;
        int surface_texture_width;
        int surface_texture_height;
        int surface_texture_format;
        WGPURenderPassDepthStencilAttachment depth_stencil_attachment{};

        BuiltinBindGroup builtin_bind_group;
    public:

        RenderingContext();

        /**
         * @brief set target color texture
         * 
         */
        void set_target_color_texture(WGPUTextureView texture, int slot);

        void set_color_attachments_size(int size) {
            this->color_attachment_size = size;
        }

        void set_target_depth_texture(WGPUTextureView texture);

        void remove_target_depth_texture();

        void set_surface_texture_view(WGPUTextureView texture_view, int width, int height, WGPUTextureFormat format);

        [[nodiscard]] optional<WGPUTextureView> get_surface_texture_view() const {
            return surface_texture_view;
        }

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
            const shared_ptr<Mesh>& mesh, const glm::mat4x4& transform_matrix, const shared_ptr<Material>& material);

        void draw_imgui();

        void clear_targets(glm::vec4 clear_color);

        [[nodiscard]] WGPUDevice get_device() const {
            return device;
        }

        [[nodiscard]] optional<pair<int, int>> get_surface_size() const;

        [[nodiscard]] bool has_surface() const {
            return surface_texture_view.has_value();
        }
    };
}

#endif
