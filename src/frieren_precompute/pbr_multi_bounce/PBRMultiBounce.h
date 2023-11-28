#ifndef FRIEREN_PRECOMPUTE_MULTI_BOUNCE_H
#define FRIEREN_PRECOMPUTE_MULTI_BOUNCE_H

#include <common_include_glm.h>

namespace frieren_precompute {
    class PBRMultiBounce {
    public:
        double ndf_ggx(glm::vec3 m, glm::vec3 macro_normal, double alpha_g);

        double smith_g2_height_field_with_denom(glm::vec3 light, glm::vec3 view, glm::vec3 n, double alpha_g);

        double smith_g2(glm::vec3 light, glm::vec3 view, glm::vec3 m, glm::vec3 normal, double alpha);

        double lambda(double a);

        // ggx, smith height-correlated G2, schlick fresnel
        double brdf(double alpha_ggx, glm::vec3 light, glm::vec3 view, glm::vec3 normal);

        double integrate_r_sf1(double theta, double alpha_ggx);

        double integrate_r_sf1_cos_theta_eq0(double alpha_ggx);

        // generate R_sf1 map, parameterized by alpha and cos(theta)
        void generate_map(int image_size, uint8_t* data);

        // generate R_sf1_bar map, parameterized by alpha
        void generate_R_sf1_bar_map(int image_width, uint8_t* data);
    };
}

#endif
