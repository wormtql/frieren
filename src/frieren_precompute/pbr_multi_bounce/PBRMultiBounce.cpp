#include "PBRMultiBounce.h"
#include <stb_image.h>
#include <random>
#include <cmath>
#include <iostream>
#include <cassert>

using namespace std;

namespace frieren_precompute {
    double PBRMultiBounce::lambda(double a) {
        double temp = sqrt(1.0 + 1.0 / (a * a));
        return (temp - 1.0) / 2.0;
    }

    double PBRMultiBounce::smith_g2(glm::vec3 light, glm::vec3 view, glm::vec3 m, glm::vec3 normal, double alpha) {
        double ndotl = glm::dot(normal, light);
        double ndotv = glm::dot(normal, view);
        double a1 = ndotl / (alpha * sqrt(1 - ndotl * ndotl));
        double a2 = ndotv / (alpha * sqrt(1 - ndotv * ndotv));

        double lambda_l = lambda(a1);
        double lambda_v = lambda(a2);

        double mdotl = glm::step(0.0f, glm::dot(m, light));
        double mdotv = glm::step(0.0f, glm::dot(m, view));
        double result = mdotl * mdotv / (1.0 + lambda_l + lambda_v);
        assert(result < 1.001);
        return result;
    }

    double PBRMultiBounce::brdf(double alpha_ggx, glm::vec3 light, glm::vec3 view, glm::vec3 normal) {
        glm::vec3 half = glm::normalize(light + view);
        double ndf = ndf_ggx(half, normal, alpha_ggx);
        double g2_with_denom = smith_g2_height_field_with_denom(light, view, half, alpha_ggx);
        double g2 = smith_g2(light, view, half, normal, alpha_ggx);
        double ndotl = glm::dot(normal, light);
        double ndotv = glm::dot(normal, view);

        double numerator = g2 * ndf;
        // if (numerator == 0) {
        //     return 0;
        // }
        double denom = 4.0 * ndotl * ndotv;

        // return g2;
        // return numerator / denom;
        // return ndf * g2_with_denom;
        // return ndf;
        // return g2_with_denom;
        return g2 * ndf / denom;
    }

    double PBRMultiBounce::ndf_ggx(glm::vec3 m, glm::vec3 macro_normal, double alpha_g) {
        double ndotm = glm::dot(m, macro_normal);
        double chi = glm::step(0.0, ndotm);

        // if (alpha_g == 0) {
        //     if (ndotm != 1) {
        //         return 0;
        //     } else {
        //         return 1.0 / 3.14;
        //     }
        // }
        assert(abs(chi - 1) < 1e-6);

        double a2 = alpha_g * alpha_g;
        double temp = 1 + ndotm * ndotm * (a2 - 1);
        double result = a2 * chi / (3.14 * temp * temp);
        return result;
        // return a2 * chi / (3.14 * temp);
    }

    double PBRMultiBounce::smith_g2_height_field_with_denom(glm::vec3 light, glm::vec3 view, glm::vec3 n, double alpha_g) {
        // double ui = glm::saturate(glm::dot(n, light));
        double ui = glm::dot(n, light);
        // double uo = glm::saturate(glm::dot(n, view));
        double uo = glm::dot(n, view);
        double a2 = alpha_g * alpha_g;

        double x1 = uo * sqrt(a2 + ui * (ui - a2 * ui));
        double x2 = ui * sqrt(a2 + uo * (uo - a2 * uo));
        double result = 0.5 / (x1 + x2);
        // assert(result < 1);
        return result;
    }

    double PBRMultiBounce::integrate_r_sf1_cos_theta_eq0(double alpha_ggx) {
        if (alpha_ggx == 0) {
            return 1;
        }
        glm::vec3 view_dir{0, 1, 0};
        glm::vec3 macro_normal{0, 0, 1};
        std::mt19937 generator;
        std::uniform_real_distribution<double> uniform_real(0.0, 1.0);
        double sum = 0;
        int N = 1000;

        for (int i = 0; i < N; i++) {
            double r1 = uniform_real(generator);
            double r2 = uniform_real(generator);
            double cos_theta = sqrt((1 - r1) / (r1 * (alpha_ggx * alpha_ggx - 1) + 1));
            double sin_theta = sqrt(1 - cos_theta * cos_theta);
            double phi = 2 * glm::pi<double>() * r2;

            glm::vec3 half_dir{sin(phi) * sin_theta, cos(phi) * sin_theta, cos_theta};
            glm::vec3 light_dir = -glm::reflect(view_dir, half_dir);

            double vdotl = glm::dot(view_dir, light_dir);
            double ndotl = glm::dot(macro_normal, light_dir);
            if (ndotl > 1e-6) {
                
                double t = (vdotl + 1) / ndotl;
                // if (i == 4651) {
                //     cout << "sum = " << sum << endl;
                //     cout << "t = " << t << endl;
                // }
                sum += t;
            }
        }
        

        double result = 2 * sum / (N * alpha_ggx);
        // cout << "result = " << result << endl;
        result = glm::clamp<double>(result, 0, 1);
        return result;
    }

    double PBRMultiBounce::integrate_r_sf1(double theta, double alpha_ggx) {
        if (alpha_ggx == 0) {
            return 1;
        }

        glm::vec3 view_dir{0, sin(theta), cos(theta)};
        glm::vec3 macro_normal{0, 0, 1};
        std::mt19937 generator;
        std::uniform_real_distribution<double> uniform_real(0.0, 1.0);
        double sum = 0;
        int N = 1000;

        for (int i = 0; i < N; i++) {
            double r1 = uniform_real(generator);
            double r2 = uniform_real(generator);
            double cos_theta = sqrt((1 - r1) / (r1 * (alpha_ggx * alpha_ggx - 1) + 1));
            double sin_theta = sqrt(1 - cos_theta * cos_theta);
            double phi = 2 * glm::pi<double>() * r2;

            glm::vec3 half_dir{sin(phi) * sin_theta, cos(phi) * sin_theta, cos_theta};
            glm::vec3 light_dir = -glm::reflect(view_dir, half_dir);
            float ndotl = glm::dot(macro_normal, light_dir);
            float ndotv = glm::dot(macro_normal, view_dir);
            if (ndotl > 1e-6) {
                double G = smith_g2(light_dir, view_dir, half_dir, macro_normal, alpha_ggx);
                double vdoth = glm::dot(view_dir, half_dir);
                double ndoth = glm::dot(macro_normal, half_dir);
                double t = G * vdoth / (ndoth * ndotv);
                // double t = (G / ndotv) * (vdoth / ndotl);
                sum += t;
            }
        }
        double result = sum / N;
        result = glm::clamp<double>(result, 0, 1);
        return result;

        // if (alpha_ggx == 0) {
        //     return 1;
        // }
        // glm::vec3 view_dir{0, sin(theta), cos(theta)};
        // glm::vec3 macro_normal{0, 0, 1};

        // std::mt19937 generator;
        // std::uniform_real_distribution<double> uniform_real(0.0, 1.0);

        // double sum = 0;
        // int N = 10000;
        // for (int i = 0; i < N; i++) {
        //     double r1 = uniform_real(generator);
        //     double r2 = uniform_real(generator);

        //     float theta2 = glm::acos(1.0 - r1);
        //     float azimuth = 2 * 3.14 * r2;
        //     glm::vec3 light_dir{glm::sin(azimuth) * glm::sin(theta2), glm::cos(azimuth) * glm::sin(theta2), glm::cos(theta2)};

        //     double b = brdf(alpha_ggx, light_dir, view_dir, macro_normal);
        //     b *= 1.0 - r1;
        //     sum += b;
        // }
        // sum /= N;

        // double result = sum * 3.14 * 2;
        // return result;
    }

    void PBRMultiBounce::generate_map(int image_size, uint8_t* data) {
        int index = 0;
        for (int row = 0; row < image_size; row++) {
            for (int col = 0; col < image_size; col++) {
                double cos_theta = (double)col / (image_size - 1);
                double alpha = (double)row / (image_size - 1);
                double theta = glm::acos(cos_theta);
                double value;
                if (col == 0) {
                    // cos theta = 0
                    value = integrate_r_sf1_cos_theta_eq0(alpha);
                } else {
                    value = integrate_r_sf1(theta, alpha);
                }
                value = 1 - value;
                // double value = 1 - integrate_r_sf1(theta, alpha);

                // uint8_t v = static_cast<uint8_t>(value * 255);
                uint8_t v = static_cast<uint8_t>(value * 255);
                // data[index] = v;
                // data[index + 1] = v;
                // data[index + 2] = v;
                // index += 3;
                data[index++] = v;
            }
        }
    }

    void PBRMultiBounce::generate_R_sf1_bar_map(int image_width, uint8_t* data) {
        std::mt19937 generator;
        std::uniform_real_distribution<double> uniform_real(0.0, 3.14 / 2);

        for (int i = 0; i < image_width; i++) {
            int N = 100;
            double alpha = (double)i / (image_width - 1);

            double sum = 0;
            for (int j = 0; j < N; j++) {
                double theta = 3.14 / 2.0 / N * j;
                double value = integrate_r_sf1(theta, alpha);
                value *= glm::sin(theta) * glm::cos(theta);
                // value *= glm::sin(theta) * glm::cos(theta) * 3.14 / 2;
                value *= 3.14 / 2.0 / N;
                sum += value;
            }
            // sum /= N;
            double result = sum * 2;
            uint8_t pixel = static_cast<uint8_t>(result * 255);
            data[i] = pixel;
            // data[3 * i] = pixel;
            // data[3 * i + 1] = pixel;
            // data[3 * i + 2] = pixel;
        }
    }
}