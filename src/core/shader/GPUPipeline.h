//
// Created by 58413 on 2023/10/17.
//

#ifndef FRIEREN_GPUPIPELINE_H
#define FRIEREN_GPUPIPELINE_H

#include <common_include.h>
#include <webgpu/webgpu.hpp>

using namespace std;

namespace frieren_core {
    class GPUPipeline {
    private:
        map<size_t, WGPURenderPipeline> pipelines;
    };
}



#endif //FRIEREN_GPUPIPELINE_H
