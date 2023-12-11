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

bool write_tiff(std::string_view path, uint32_t width, uint32_t height, const std::vector<ldt::Pencil>& pencils) {
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

    return ldt::io::write_tiff(
        path.data(), static_cast<uint16_t>(pencils.size()), width, height, rasters.data(), pencils.data());
}

bool read_tiff(std::string_view path, const std::vector<ldt::Pencil>& pencils) {
    if (const auto derm_tiff = ldt::io::open_tiff(path.data()); derm_tiff.is_valid) {
        Image raster(static_cast<size_t>(derm_tiff.width) * derm_tiff.height);

        // Read original image
        if (!ldt::io::read_original_image(path.data(), reinterpret_cast<uint32_t*>(raster.data()))) {
            return false;
        }

        // Read layers
        for (uint16_t i = 0; i < pencils.size(); i++) {
            ldt::Pencil pencil;
            if (!ldt::io::read_layer(path.data(), i, reinterpret_cast<uint32_t*>(raster.data()), &pencil)
                || pencil != pencils[i]) {
                return false;
            }
        }

        return true;
    }
    return false;
}

std::string get_out_path() {
    static int i = 0;
    return "../test/out/out" + std::to_string(i++) + ".tiff";
}

std::pair<bool, bool> write_read(uint32_t width,
                                 uint32_t height,
                                 const std::vector<ldt::Pencil>& pencils = std::vector<ldt::Pencil>()) {
    const auto file         = get_out_path();
    const auto write_result = write_tiff(file, width, height, pencils);
    const auto read_result  = read_tiff(file, pencils);
    return {write_result, read_result};
}

TEST_CASE("Write", "[io::write_tiff]") {
    SECTION("Without pencils") {
        const auto [write_result, read_result] = write_read(255, 255);

        REQUIRE(write_result);
        REQUIRE(read_result);
    }

    SECTION("With two pencils") {
        std::vector<ldt::Pencil> pencils = {{"pencil"}, {"pencil2"}};

        const auto [write_result, read_result] = write_read(255, 255, pencils);

        REQUIRE(write_result);
        REQUIRE(read_result);
    }

    SECTION("Max size") {
        const auto [write_result, read_result] = write_read(ldt::DermTIFF::max_width, ldt::DermTIFF::max_height);

        REQUIRE(write_result);
        REQUIRE(read_result);
    }
}

TEST_CASE("Not writable", "[io::write_tiff]") {
    SECTION("Invalid pencil") {
        std::vector<ldt::Pencil> pencils = {{""}};

        const auto [write_result, read_result] = write_read(255, 255, pencils);

        REQUIRE_FALSE(write_result);
        REQUIRE_FALSE(read_result);
    }

    SECTION("Larger than max width") {
        const auto [write_result, read_result] = write_read(ldt::DermTIFF::max_width + 1, 255);

        REQUIRE_FALSE(write_result);
        REQUIRE_FALSE(read_result);
    }

    SECTION("Larger than max height") {
        const auto [write_result, read_result] = write_read(255, ldt::DermTIFF::max_height + 1);

        REQUIRE_FALSE(write_result);
        REQUIRE_FALSE(read_result);
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
