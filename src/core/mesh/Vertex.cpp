#include "Vertex.h"

namespace frieren_core {
    WGPUVertexBufferLayout WGPUVertexBufferLayoutOwned::get_wgpu_vertex_buffer_layout() {
        WGPUVertexBufferLayout ret;
        ret.arrayStride = arrayStride;
        ret.stepMode = stepMode;
        ret.attributeCount = attributes.size();
        ret.attributes = attributes.data();

        return ret;
    }

    WGPUVertexBufferLayoutOwned Vertex::get_vertex_buffer_layout() {
        std::vector<WGPUVertexAttribute> attributes;

        attributes.push_back(WGPUVertexAttribute {
            .format = WGPUVertexFormat_Float32x3,
            .offset = 0,
            .shaderLocation = 0
        });
        attributes.push_back(WGPUVertexAttribute {
            .format = WGPUVertexFormat_Float32x3,
            .offset = 3 * sizeof(float),
            .shaderLocation = 1
        });
        attributes.push_back(WGPUVertexAttribute {
            .format = WGPUVertexFormat_Float32x2,
            .offset = 6 * sizeof(float),
            .shaderLocation = 2
        });
        attributes.push_back(WGPUVertexAttribute {
            .format = WGPUVertexFormat_Float32x3,
            .offset = 8 * sizeof(float),
            .shaderLocation = 3
        });
        attributes.push_back(WGPUVertexAttribute {
            .format = WGPUVertexFormat_Float32x3,
            .offset = 11 * sizeof(float),
            .shaderLocation = 4
        });

        WGPUVertexBufferLayoutOwned ret;
        ret.arrayStride = sizeof(Vertex);
        ret.stepMode = WGPUVertexStepMode_Vertex;
        ret.attributes = std::move(attributes);

        return std::move(ret);
    }
}