#include "DisplayColorRenderPipeline.h"
#include <components/all_components.h>
#include <scene/Scene.h>
#include <common_include_glm.h>

using namespace frieren_core::component;

namespace frieren_core {
    void DisplayColorRenderPipeline::render_scene(const Scene &scene, RenderingContext &rendering_context) {
        if (!rendering_context.has_surface()) {
            return;
        }

        WGPUDevice device = rendering_context.get_device();

        auto size = rendering_context.get_surface_size().value();
        int width = size.first;
        int height = size.second;

        if (!this->depth_stencil_texture.has_value()) {
            depth_stencil_texture.emplace(std::move(Texture::create_depth_stencil_texture(
                    device,
                    WGPUTextureFormat_Depth24PlusStencil8,
                    "depth texture",
                    width,
                    height
            )));
        }
        if (this->depth_stencil_texture->get_width() != width || this->depth_stencil_texture->get_height() != height) {
            depth_stencil_texture.reset();
            depth_stencil_texture.emplace(Texture::create_depth_stencil_texture(
                    device,
                    WGPUTextureFormat_Depth24PlusStencil8,
                    "depth texture",
                    width,
                    height
            ));
        }

        vector<shared_ptr<MeshRenderer>> mesh_renderers = scene.get_game_objects_of_type<MeshRenderer>();
//        cout << "render scene" << endl;

        rendering_context.set_color_attachments_size(1);
        rendering_context.set_target_color_texture(rendering_context.get_surface_texture_view().value(), 0);
        rendering_context.set_target_depth_texture(this->depth_stencil_texture->get_wgpu_texture_view());
        rendering_context.depth_stencil_attachment.depthLoadOp = WGPULoadOp_Clear;
        rendering_context.depth_stencil_attachment.depthStoreOp = WGPUStoreOp_Store;
        rendering_context.depth_stencil_attachment.depthClearValue = 1.0f;
        rendering_context.depth_stencil_attachment.depthReadOnly = false;
        rendering_context.depth_stencil_attachment.stencilLoadOp = WGPULoadOp_Load;
        rendering_context.depth_stencil_attachment.stencilStoreOp = WGPUStoreOp_Store;
        rendering_context.depth_stencil_attachment.view = depth_stencil_texture->get_wgpu_texture_view();

        rendering_context.clear_targets({ 0.1, 0.2, 0.3, 1 });

        for (int i = 0; i < mesh_renderers.size(); i++) {
            shared_ptr<MeshFilter> mesh_filter = mesh_renderers[i]->get_component<MeshFilter>().value();
            shared_ptr<MeshRenderer> mesh_renderer = mesh_renderers[i];
            rendering_context.draw_mesh(mesh_filter->get_mesh(), glm::mat4x4{1.0f}, mesh_renderer->get_materials()[0].object);
        }
    }
}
