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
    return ldt::io::WriteTIFF(
        path, static_cast<uint16_t>(pencils.size()), width, height, rasters.data(), pencils.data());
}

bool writeWithPencils(
    const std::string& path, uint32_t width, uint32_t height, Image& image, std::vector<ldt::Pencil>& pencils) {
    const auto size = width * height;
    Image layer(size);
    for (uint32_t i = 0; i < size; i++) {
        layer[i].r = 50;
        layer[i].g = 50;
        layer[i].b = 200;
        layer[i].a = 255;
    }
    std::vector<uint32_t*> rasters(pencils.size() + 1);
    rasters[0] = reinterpret_cast<uint32_t*>(image.data());
    for (size_t i = 1; i < rasters.size(); i++) {
        rasters[i] = reinterpret_cast<uint32_t*>(layer.data());
    }
    return ldt::io::WriteTIFF(
        path, static_cast<uint16_t>(pencils.size()), width, height, rasters.data(), pencils.data());
}

bool read(const std::string& path, const std::vector<ldt::Pencil>& pencils = std::vector<ldt::Pencil>()) {
    if (const auto dermTiff = ldt::io::OpenTIFF(path); dermTiff.isValid) {
        Image raster(static_cast<size_t>(dermTiff.width) * dermTiff.height);
        // read layers
        if (!pencils.empty()) {
            const uint16_t layerCount = static_cast<uint16_t>(pencils.size());
            for (uint16_t layerIndex = 0; layerIndex < layerCount; layerIndex++) {
                if (ldt::Pencil pencil;
                    ldt::io::ReadLayer(path, layerIndex, reinterpret_cast<uint32_t*>(raster.data()), &pencil)) {
                    // compare pencil
                    return pencil == pencils[layerIndex];
                } else {
                    return false;
                }
            }
        }
        // read original image
        else {
            return ldt::io::ReadOriginalImage(path, reinterpret_cast<uint32_t*>(raster.data()));
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

#define TestWriteReadPencil(writable, readable, width, height, image, pencils)                                         \
    {                                                                                                                  \
        const auto file = getOutPath();                                                                                \
        assert(writeWithPencils(file, width, height, image, pencils) == writable);                                     \
        assert(read(file, pencils) == readable);                                                                       \
    }

int main() {
    // delete all tiff files exist
    for (auto& file : std::filesystem::directory_iterator("../test/out/")) {
        if (file.path().filename() != ".gitkeep") {
            std::filesystem::remove(file);
        }
    }

    {
        // Basic write / read without pencil
        const uint32_t width = 255, height = 255;
        Image image(width * height);
        TestWriteRead(true, true, width, height, image);
    }

    {
        // With invalid pencil name
        const uint32_t width = 255, height = 255;
        Image image(width * height);
        std::vector<ldt::Pencil> pencils = {{""}};
        TestWriteReadPencil(false, false, width, height, image, pencils);
    }

    {
        // With two pencils
        const uint32_t width = 255, height = 255;
        Image image(width * height);
        std::vector<ldt::Pencil> pencils = {{"pencil"}, {"pencil2"}};
        TestWriteReadPencil(true, true, width, height, image, pencils);
    }

    {
        // Larger than max width
        const uint32_t width = ldt::DermTIFF::MaxWidth + 1, height = 1;
        Image image(width * height);
        TestWriteRead(false, false, width, height, image);
    }

    {
        // Larger than max height
        const uint32_t width = 1, height = ldt::DermTIFF::MaxHeight + 1;
        Image image(width * height);
        TestWriteRead(false, false, width, height, image);
    }

    {
        // Max size
        const uint32_t width = ldt::DermTIFF::MaxWidth, height = ldt::DermTIFF::MaxHeight;
        Image image(width * height);
        TestWriteRead(true, true, width, height, image);
    }

    return 0;
}
