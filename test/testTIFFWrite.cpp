#include <assert.h>

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "libdermtiff/dermtiff.hpp"

struct Color {
    uint8_t r = 0, g = 0, b = 0, a = 0;
};

using Image = std::vector<Color>;

bool write(const std::string& path, uint32_t width, uint32_t height, Image& image) {
    std::vector<uint32_t*> rasters(1);
    rasters[0] = reinterpret_cast<uint32_t*>(image.data());
    std::vector<ldt::Pencil> pencils;
    return ldt::WriteTIFF(path, 1, width, height, rasters.data(), pencils.data());
}

bool writeWithPencil(
    const std::string& path, uint32_t width, uint32_t height, Image& image, const ldt::Pencil& pencil) {
    const auto size = width * height;
    Image layer(size);
    for (uint32_t i = 0; i < size; i++) {
        layer[i].r = 0;
        layer[i].g = 0;
        layer[i].b = 0;
        layer[i].a = 255;
    }
    std::vector<uint32_t*> rasters(2);
    rasters[0] = reinterpret_cast<uint32_t*>(image.data());
    rasters[1] = reinterpret_cast<uint32_t*>(layer.data());
    std::vector<ldt::Pencil> pencils(2);
    pencils[1] = pencil;
    return ldt::WriteTIFF(path, 2, width, height, rasters.data(), pencils.data());
}

bool read(const std::string& path, const std::optional<ldt::Pencil>& pencil = std::nullopt) {
    if (const auto dermTiff = ldt::Open(path); dermTiff.isValid) {
        Image raster(dermTiff.width * dermTiff.height);
        ldt::Pencil p;
        // read layer
        if (pencil.has_value()) {
            if (ldt::ReadPage(path, 1, reinterpret_cast<uint32_t*>(raster.data()), &p)) {
                // compare pencil
                if (pencil.has_value()) {
                    return p.name == pencil.value().name && p.r == pencil.value().r && p.g == pencil.value().g
                           && p.b == pencil.value().b && p.a == pencil.value().a;
                } else {
                    return true;
                }
            } else {
                return false;
            }
        }
        // read original image
        else {
            return ldt::ReadPage(path, 0, reinterpret_cast<uint32_t*>(raster.data()), &p);
        }
    }
    return false;
}

std::string getOutPath() {
    static int i = 0;
    return "../test/out/out" + std::to_string(i++) + ".tiff";
}

#define TestWriteRead(writable, readable, width, height, image)                                                        \
    {                                                                                                                  \
        const auto file = getOutPath();                                                                                \
        assert(write(file, width, height, image) == writable);                                                         \
        assert(read(file) == readable);                                                                                \
    }

#define TestWriteReadPencil(writable, readable, width, height, image, pencil)                                          \
    {                                                                                                                  \
        const auto file = getOutPath();                                                                                \
        assert(writeWithPencil(file, width, height, image, pencil) == writable);                                       \
        assert(read(file, pencil) == readable);                                                                        \
    }

int main() {
    // delete all tiff files exist
    for (auto& file : std::filesystem::directory_iterator("../test/out/")) {
        if (file.path().filename() != ".gitkeep") {
            std::filesystem::remove(file);
        }
    }

    {
        const uint32_t width = 255, height = 255;
        Image image(width * height);
        TestWriteRead(true, true, width, height, image);
    }

    {
        const uint32_t width = 255, height = 255;
        Image image(width * height);
        ldt::Pencil pencil;
        pencil.name = "";
        TestWriteReadPencil(false, false, width, height, image, pencil);
    }

    {
        const uint32_t width = 255, height = 255;
        Image image(width * height);
        ldt::Pencil pencil;
        pencil.name = "pencil";
        TestWriteReadPencil(true, true, width, height, image, pencil);
    }

    {
        const uint32_t width = ldt::DermTIFF::MaxWidth + 1, height = 1;
        Image image(width * height);
        TestWriteRead(false, false, width, height, image);
    }

    {
        const uint32_t width = 1, height = ldt::DermTIFF::MaxHeight + 1;
        Image image(width * height);
        TestWriteRead(false, false, width, height, image);
    }

    {
        const uint32_t width = ldt::DermTIFF::MaxWidth, height = ldt::DermTIFF::MaxHeight;
        Image image(width * height);
        TestWriteRead(true, true, width, height, image);
    }

    return 0;
}
