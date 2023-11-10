#include "RenderingContext.h"

namespace frieren_core {
    RenderingContext::RenderingContext() {
    }

    void RenderingContext::set_target_color_texture(WGPUTextureView texture, int slot) {
        assert(slot >= 0 && slot < 8);

        this->target_color_textures[slot] = texture;
    }

    void RenderingContext::set_target_depth_texture(WGPUTextureView texture) {
        this->target_depth_textures = texture;
    }

    void RenderingContext::remove_target_depth_texture() {
        this->target_depth_textures = {};
    }

    void RenderingContext::set_surface_texture_view(WGPUTextureView texture_view, int width, int height, WGPUTextureFormat format) {
        this->surface_texture_view = texture_view;
        this->surface_texture_width = width;
        this->surface_texture_height = height;
        this->surface_texture_format = format;
    }
    
    void RenderingContext::draw_mesh(
        const shared_ptr<Mesh>& mesh,
        const glm::mat4x4& transform_matrix,
        const shared_ptr<Material>& material
    ) {
        WGPUCommandEncoderDescriptor command_encoder_desc{};
        command_encoder_desc.nextInChain = nullptr;
        command_encoder_desc.label = "Draw Mesh";
        WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, &command_encoder_desc);

        WGPURenderPassDescriptor render_pass_desc{};
        render_pass_desc.timestampWriteCount = 0;
        render_pass_desc.timestampWrites = nullptr;
        render_pass_desc.nextInChain = nullptr;
        // setup color attachments
        vector<WGPURenderPassColorAttachment> color_attachments;
        for (int i = 0; i < color_attachment_size; i++) {
            WGPUTextureView texture_view = target_color_textures[i];
            if (texture_view == nullptr) {
                assert(false);
            }
            WGPURenderPassColorAttachment color_attachment;
            color_attachment.view = texture_view;
            color_attachment.resolveTarget = nullptr;
            color_attachment.loadOp = WGPULoadOp_Load;
            color_attachment.storeOp = WGPUStoreOp_Store;
            
            color_attachments.emplace_back(color_attachment);
        }
        render_pass_desc.colorAttachmentCount = color_attachments.size();
        render_pass_desc.colorAttachments = color_attachments.data();

        // setup depth stencil attachments
        if (this->target_depth_textures.has_value()) {
            render_pass_desc.depthStencilAttachment = &this->depth_stencil_attachment;
        }

        // write per object uniform data
        this->builtin_bind_group.per_object_uniform.data.frieren_object_matrix = transform_matrix;
        this->builtin_bind_group.per_object_uniform.update_uniform_buffer(queue);

        WGPURenderPassEncoder render_pass_encoder = wgpuCommandEncoderBeginRenderPass(encoder, &render_pass_desc);
        material->use_material(render_pass_encoder);
        // set builtin bind group
        wgpuRenderPassEncoderSetBindGroup(render_pass_encoder, 0, this->builtin_bind_group.bind_group, 0, nullptr);

        // setup mesh index and vertex buffer
        mesh->set_buffer_for_render_pass(render_pass_encoder);

        wgpuRenderPassEncoderDrawIndexed(render_pass_encoder, mesh->get_index_count(), 1, 0, 0, 0);
        wgpuRenderPassEncoderEnd(render_pass_encoder);

        WGPUCommandBufferDescriptor cmd_buffer_desc{};
        cmd_buffer_desc.nextInChain = nullptr;
        cmd_buffer_desc.label = "Command Buffer";

        WGPUCommandBuffer command_buffer = wgpuCommandEncoderFinish(encoder, &cmd_buffer_desc);
        wgpuQueueSubmit(queue, 1, &command_buffer);
    }

    void RenderingContext::init(WGPUDevice device, WGPUQueue queue) {
        this->queue = queue;
        this->device = device;
        this->builtin_bind_group.init(device);
    }

    optional<pair<int, int>> RenderingContext::get_surface_size() const {
        if (this->surface_texture_view.has_value()) {
            return pair{surface_texture_width, surface_texture_height};
        }
        return {};
    }
}
