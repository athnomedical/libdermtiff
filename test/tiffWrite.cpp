#define CATCH_CONFIG_RUNNER

#include <filesystem>
#include <string>
#include <vector>

#include "catch2/catch.hpp"
#include "libdermtiff/dermtiff.hpp"

struct Color {
    uint8_t r = 0, g = 0, b = 0, a = 0;
};

using Image = std::vector<Color>;

bool WriteTIFF(std::string_view path, uint32_t width, uint32_t height, const std::vector<ldt::Pencil>& pencils) {
    std::vector<Image> images(1 + pencils.size());
    std::vector<uint32_t*> rasters(1 + pencils.size());

    // Original image
    images[0] = Image(width * height);

    // Layers
    for (uint16_t i = 0; i < pencils.size(); i++) {
        images[i + 1] = Image(width * height, {pencils[i].r, pencils[i].g, pencils[i].b, pencils[i].a});
    }

    // To pointer
    for (size_t i = 0; i < images.size(); i++) {
        rasters[i] = reinterpret_cast<uint32_t*>(images[i].data());
    }

    return ldt::io::WriteTIFF(
        path, static_cast<uint16_t>(pencils.size()), width, height, rasters.data(), pencils.data());
}

bool ReadTIFF(std::string_view path, const std::vector<ldt::Pencil>& pencils) {
    if (const auto dermTiff = ldt::io::OpenTIFF(path); dermTiff.isValid) {
        Image raster(static_cast<size_t>(dermTiff.width) * dermTiff.height);

        // Read original image
        if (!ldt::io::ReadOriginalImage(path, reinterpret_cast<uint32_t*>(raster.data()))) {
            return false;
        }

        // Read layers
        for (uint16_t i = 0; i < pencils.size(); i++) {
            ldt::Pencil pencil;
            if (!ldt::io::ReadLayer(path, i, reinterpret_cast<uint32_t*>(raster.data()), &pencil)
                || pencil != pencils[i]) {
                return false;
            }
        }

        return true;
    }
    return false;
}

std::string GetOutPath() {
    static int i = 0;
    return "../test/out/out" + std::to_string(i++) + ".tiff";
}

std::pair<bool, bool> WriteRead(uint32_t width,
                                uint32_t height,
                                const std::vector<ldt::Pencil>& pencils = std::vector<ldt::Pencil>()) {
    const auto file        = GetOutPath();
    const auto writeResult = WriteTIFF(file, width, height, pencils);
    const auto readResult  = ReadTIFF(file, pencils);
    return {writeResult, readResult};
}

TEST_CASE("Write", "[io::WriteTIFF]") {
    SECTION("Without pencils") {
        const auto [writeResult, readResult] = WriteRead(255, 255);

        REQUIRE(writeResult);
        REQUIRE(readResult);
    }

    SECTION("With two pencils") {
        std::vector<ldt::Pencil> pencils = {{"pencil"}, {"pencil2"}};

        const auto [writeResult, readResult] = WriteRead(255, 255, pencils);

        REQUIRE(writeResult);
        REQUIRE(readResult);
    }

    SECTION("Max size") {
        const auto [writeResult, readResult] = WriteRead(ldt::DermTIFF::MaxWidth, ldt::DermTIFF::MaxHeight);

        REQUIRE(writeResult);
        REQUIRE(readResult);
    }
}

TEST_CASE("Not writable", "[io::WriteTIFF]") {
    SECTION("Invalid pencil") {
        std::vector<ldt::Pencil> pencils = {{""}};

        const auto [writeResult, readResult] = WriteRead(255, 255, pencils);

        REQUIRE_FALSE(writeResult);
        REQUIRE_FALSE(readResult);
    }

    SECTION("Larger than max width") {
        const auto [writeResult, readResult] = WriteRead(ldt::DermTIFF::MaxWidth + 1, 255);

        REQUIRE_FALSE(writeResult);
        REQUIRE_FALSE(readResult);
    }

    SECTION("Larger than max height") {
        const auto [writeResult, readResult] = WriteRead(255, ldt::DermTIFF::MaxHeight + 1);

        REQUIRE_FALSE(writeResult);
        REQUIRE_FALSE(readResult);
    }
}

int main(int argc, char* argv[]) {
    // Delete all output files
    for (const auto& file : std::filesystem::directory_iterator("../test/out/")) {
        if (file.path().filename() != ".gitkeep") {
            std::filesystem::remove(file);
        }
    }

    // Run tests
    return Catch::Session().run(argc, argv);
}
