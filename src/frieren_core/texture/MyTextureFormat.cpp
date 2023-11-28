#include "MyTextureFormat.h"

#include <fstream>
#include <iterator>

using namespace std;

namespace frieren_core {
    optional<MyTextureFormatData> MyTextureFormat::read_texture(const filesystem::path &path) {
        MyTextureFormatData result{};

        ifstream file{path, ios::binary};
        if (!file.is_open()) {
            return {};
        }

        file.read(reinterpret_cast<char*>(&result.header), sizeof(MyTextureFormatHeader));
        // todo mipmap
        int size = result.header.width * result.header.width * result.header.depth * result.header.pixel_bytes;
        result.data.resize(size);
        file.read(reinterpret_cast<char*>(result.data.data()), size);

        file.close();

        result.mips.emplace_back(0, size);

        return move(result);
    }

    void MyTextureFormat::write_texture(const filesystem::path &path, const MyTextureFormatData &data) {
        ofstream file{path, ios::binary};
        if (!file.is_open()) {
            return;
        }

        file.write(reinterpret_cast<const char*>(&data.header), sizeof(MyTextureFormatHeader));
        int texture_data_size = data.data.size();
        file.write(reinterpret_cast<const char*>(data.data.data()), texture_data_size);
    }
}