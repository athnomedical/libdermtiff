#define CATCH_CONFIG_MAIN

#include <iostream>
#include <string>
#include <vector>

#include "catch2/catch.hpp"
#include "libdermtiff/dermtiff.hpp"

struct Color {
    uint8_t r = 0, g = 0, b = 0, a = 0;
};

using Image = std::vector<Color>;

bool read_libtiff_image(const std::string& filename) {
    const auto path = "../deps/libtiff/test/images/" + filename + ".tiff";
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "Read libtiff image: " << path << std::endl;

    if (const auto dermTiff = ldt::io::open_tiff(path.c_str()); dermTiff.is_valid) {
        Image raster(static_cast<size_t>(dermTiff.width) * dermTiff.height);
        ldt::Pencil pencil;
        for (uint16_t i = 0; i < dermTiff.layer_count + 1; i++) {
            if (!ldt::io::read_page(path.c_str(), i, reinterpret_cast<uint32_t*>(raster.data()), &pencil)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool read_dermtiff_image(const std::string& filename, const std::vector<ldt::Pencil>& pencils) {
    const auto path = "../test/images/" + filename + ".tiff";
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "Read DermAnnotation TIFF image: " << path << std::endl;

    if (const auto dermTiff = ldt::io::open_tiff(path.c_str()); dermTiff.is_valid) {
        Image raster(static_cast<size_t>(dermTiff.width) * dermTiff.height);

        // Read original image
        if (!ldt::io::read_original_image(path.c_str(), reinterpret_cast<uint32_t*>(raster.data()))) {
            return false;
        }

        // Read layers
        std::vector<ldt::Pencil> tiffPencils(dermTiff.layer_count);
        for (uint16_t layerIndex = 0; layerIndex < dermTiff.layer_count; layerIndex++) {
            if (!ldt::io::read_layer(
                    path.c_str(), layerIndex, reinterpret_cast<uint32_t*>(raster.data()), &tiffPencils[layerIndex])) {
                return false;
            }
        }

        // validate pencils
        for (size_t i = 0; i < tiffPencils.size(); i++) {
            if (tiffPencils[i] != pencils[i]) {
                return false;
            }
        }
        return true;
    }
    return false;
}

TEST_CASE("Read test images in libtiff", "[io::read]") {
    REQUIRE(read_libtiff_image("logluv-3c-16b"));
    REQUIRE(read_libtiff_image("lzw-single-strip"));
    REQUIRE(read_libtiff_image("minisblack-1c-8b"));
    REQUIRE(read_libtiff_image("minisblack-1c-16b"));
    REQUIRE(read_libtiff_image("minisblack-2c-8b-alpha"));
    REQUIRE(read_libtiff_image("miniswhite-1c-1b"));
    REQUIRE(read_libtiff_image("palette-1c-1b"));
    REQUIRE(read_libtiff_image("palette-1c-4b"));
    REQUIRE(read_libtiff_image("palette-1c-8b"));
    REQUIRE(read_libtiff_image("quad-lzw-compat"));
    REQUIRE(read_libtiff_image("rgb-3c-8b"));
    REQUIRE(read_libtiff_image("rgb-3c-16b"));
    REQUIRE(read_libtiff_image("testfax4"));
    REQUIRE(read_libtiff_image("deflate-last-strip-extra-data"));  // Deflate compression support

    // The following tests depend on the build environment.
    // quad-tile.jpg                           : jpeg compression is not supported
    // ojpeg_chewey_subsamp21_multi_strip      : old-jpeg compression is not supported
    // ojpeg_single_strip_no_rowsperstrip      : old-jpeg compression is not supported
    // ojpeg_zackthecat_subsamp22_single_strip : old-jpeg compression is not supported
}

TEST_CASE("Read tiff images created by DermAnnotation", "[io::read]") {
    REQUIRE(read_dermtiff_image("v0",
                                {
                                    {"white", 255, 255, 255, 255},
                                    {"red", 255, 0, 0, 255},
                                    {"green", 0, 255, 0, 255},
                                    {"blue", 0, 0, 255, 255},
                                }));  // Created by DermAnnotation v2.2.2 not libdermtiff.

    REQUIRE(read_dermtiff_image("v1",
                                {
                                    {"white", 255, 255, 255, 255},
                                    {"red", 255, 0, 0, 255},
                                    {"green", 0, 255, 0, 255},
                                    {"blue", 0, 0, 255, 255},
                                }));  // Created with libdermtiff v0.1.0 built into DermAnnotation.
}
