#ifndef FRIEREN_CORE_RENDER_PIPELINE_H
#define FRIEREN_CORE_RENDER_PIPELINE_H

#include <scene/Scene.h>
#include <rendering/RenderingContext.h>

namespace frieren_core {
    class RenderPipeline {
    public:
        virtual void render_scene(const Scene& scene, RenderingContext& rendering_context) {}
    };
}

#endif
