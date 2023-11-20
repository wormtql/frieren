//
// Created by 58413 on 2023/10/16.
//

#ifndef FRIEREN_BINDGROUPLAYOUTBUILDER_H
#define FRIEREN_BINDGROUPLAYOUTBUILDER_H

#include <vector>
#include <webgpu/webgpu.hpp>

using namespace std;

namespace frieren_core {
    class BindGroupLayoutBuilder {
    private:
        vector<WGPUBindGroupLayoutEntry> entries;
        string name;

    public:
        BindGroupLayoutBuilder(const string& name);

        BindGroupLayoutBuilder& add_uniform_buffer(int binding);
        BindGroupLayoutBuilder& add_sampler(int binding, WGPUSamplerBindingType bindingType);
        BindGroupLayoutBuilder& add_texture(int binding, WGPUTextureViewDimension dim);
        WGPUBindGroupLayout build(WGPUDevice device) const;
    };
}


#endif //FRIEREN_BINDGROUPLAYOUTBUILDER_H
