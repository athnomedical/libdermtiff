#include <assert.h>

#include <iostream>
#include <string>
#include <vector>

#include "libdermtiff/dermtiff.hpp"

struct Color {
    uint8_t r = 0, g = 0, b = 0, a = 0;
};

using Image = std::vector<Color>;

bool testLibTiffImage(const std::string& filename) {
    const auto path = "../deps/libtiff/test/images/" + filename + ".tiff";
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "Test libtiff images: " << path << std::endl;
    if (const auto dermTiff = ldt::Open(path); dermTiff.isValid) {
        Image raster(dermTiff.width * dermTiff.height);
        ldt::Pencil pencil;
        return ldt::ReadPage(path, 0, reinterpret_cast<uint32_t*>(raster.data()), &pencil);
    }
    return false;
}

bool testDermTiffImage(const std::string& filename, const std::vector<ldt::Pencil>& pencils) {
    const auto path = "../test/images/" + filename + ".tiff";
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "Test DermAnnotation TIFF images: " << path << std::endl;
    if (const auto dermTiff = ldt::Open(path); dermTiff.isValid) {
        Image raster(dermTiff.width * dermTiff.height);
        // Read original image
        {
            if (!ldt::ReadOriginalImage(path, reinterpret_cast<uint32_t*>(raster.data()))) {
                return false;
            }
        }

        // Read layers
        std::vector<ldt::Pencil> tiffPencils(dermTiff.layerCount);
        for (uint16_t layerIndex = 0; layerIndex < dermTiff.layerCount; layerIndex++) {
            if (!ldt::ReadLayer(
                    path, layerIndex, reinterpret_cast<uint32_t*>(raster.data()), &tiffPencils[layerIndex])) {
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

int main() {
    assert(testLibTiffImage("logluv-3c-16b"));
    assert(testLibTiffImage("lzw-single-strip"));
    assert(testLibTiffImage("minisblack-1c-8b"));
    assert(testLibTiffImage("minisblack-1c-16b"));
    assert(testLibTiffImage("minisblack-2c-8b-alpha"));
    assert(testLibTiffImage("miniswhite-1c-1b"));
    assert(testLibTiffImage("palette-1c-1b"));
    assert(testLibTiffImage("palette-1c-4b"));
    assert(testLibTiffImage("palette-1c-8b"));
    assert(testLibTiffImage("quad-lzw-compat"));
    assert(testLibTiffImage("rgb-3c-8b"));
    assert(testLibTiffImage("rgb-3c-16b"));
    assert(testLibTiffImage("testfax4"));
    assert(testLibTiffImage("deflate-last-strip-extra-data"));  // Deflate compression support

    assert(testDermTiffImage("v0",
                             {{"white", 255, 255, 255, 255},
                              {"red", 255, 0, 0, 255},
                              {"green", 0, 255, 0, 255},
                              {"blue", 0, 0, 255, 255}}));  // Created by DermAnnotation v2.2.2 not libdermtiff.

    assert(
        testDermTiffImage("v1",
                          {{"white", 255, 255, 255, 255},
                           {"red", 255, 0, 0, 255},
                           {"green", 0, 255, 0, 255},
                           {"blue", 0, 0, 255, 255}}));  // Created with libdermtiff v0.1.0 built into DermAnnotation.

    // The following tests depend on the build environment.
    // assert(!testLibTiffImage("quad-tile.jpg"));                            // jpeg compression is not supported
    // assert(!testLibTiffImage("ojpeg_chewey_subsamp21_multi_strip"));       // old-jpeg compression is not supported
    // assert(!testLibTiffImage("ojpeg_single_strip_no_rowsperstrip"));       // old-jpeg compression is not supported
    // assert(!testLibTiffImage("ojpeg_zackthecat_subsamp22_single_strip"));  // old-jpeg compression is not supported

    return 0;
}
