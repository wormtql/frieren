#ifndef FRIEREN_DISPLAYCOLORRENDERPIPELINE_H
#define FRIEREN_DISPLAYCOLORRENDERPIPELINE_H

#include "RenderPipeline.h"
#include <texture/Texture.h>

using namespace std;

namespace frieren_core {
    class DisplayColorRenderPipeline: public RenderPipeline {
    private:
        optional<Texture> depth_stencil_texture;
    public:
        DisplayColorRenderPipeline() = default;

        void render_scene(const Scene &scene, RenderingContext &rendering_context) override;
    };
}



#endif //FRIEREN_DISPLAYCOLORRENDERPIPELINE_H
