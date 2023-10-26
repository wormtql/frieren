//
// Created by 58413 on 2023/10/17.
//

#ifndef FRIEREN_SHADERPIPELINEKEY_H
#define FRIEREN_SHADERPIPELINEKEY_H

#include <common_include.h>
#include <webgpu/webgpu.hpp>
#include <utilities/utils.h>

namespace frieren_core {
    const int MAX_BIND_GROUP_LAYOUT_ENTRIES = 10;

    struct ShaderPipelineKey {
        WGPUBlendState blend_state;
        WGPUFragmentState fragment_state;
        WGPUDepthStencilState depth_stencil_state;
        WGPUBindGroupLayoutEntry layout_entries[MAX_BIND_GROUP_LAYOUT_ENTRIES];
        int layout_entries_count;

        size_t hash() {
            return ::frieren_core::hash(*this);
        }
    };
}


#endif //FRIEREN_SHADERPIPELINEKEY_H
