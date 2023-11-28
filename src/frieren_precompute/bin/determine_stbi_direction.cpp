#include <stb_image_write.h>
#include <cmath>
#include <random>
#include <iostream>
#include <pbr_multi_bounce/PBRMultiBounce.h>

using namespace std;
using namespace frieren_precompute;

int main() {
    // unsigned char data[256 * 256 * 3];
    // int index = 0;
    // for (int i = 0; i < 256; i++) {
    //     for (int j = 0; j < 256; j++) {
    //         data[index] = i;
    //         data[index + 1] = j;
    //         data[index + 2] = 0;
    //         index += 3;
    //     }
    // }

    // stbi_write_bmp("test.bmp", 256, 256, 3, data);

    // PBRMultiBounce compute;
    // double result = compute.integrate_r_sf1(0.0, )

    std::mt19937 generator;
    std::uniform_real_distribution<double> uniform_real(0.0, 1.0);
    int N = 1000000;
    double sum = 0;
    for (int i = 0; i < N; i++) {
        double r1 = uniform_real(generator); // =cos theta
        sum += r1;
    }
    cout << sum / N * 2 * 3.1415296;
}