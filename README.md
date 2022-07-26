# libdermtiff

C++ library to read / write [DermAnnotation](https://kondoa9.github.io/DermAnnotation/) TIFF files.

This requires C++17 or later.

## Setup

```
git submodule update
```

```powershell
# Windows
.\scripts\windows\setup.ps1 # Run as admin
```

```zsh
# macOS
zsh scripts/macos/setup.sh
```

```bash
# Linux
bash scripts/linux/setup.sh
```

## Build

```sh
mkdir build/
cd build/
cmake ..                            # Configure
cmake --build .                     # Build libdermtiff and its dependencies
ctest -C debug --test-dir test/     # Run libdermtiff tests
```

## Usage

libdermtiff is in the namespace `ldt`.

```c++
#include <libdermtiff/dermtiff.hpp>

// Read TIFF file
const std::string path = "image.tiff";
if (const auto dermTiff = ldt::io::OpenTIFF(path); dermTiff.isValid) {
    // Read original image
    std::vector<uint32_t> raster(dermTiff.width * dermTiff.height);
    if (ldt::io::ReadOriginalImage(path, raster.data())) {
        // Success
    } else {
        // Failure
    }

    // Read layers
    for (uint16_t layerIndex = 0; layerIndex < dermTiff.layerCount; layerIndex++) {
        std::vector<uint32_t> raster(dermTiff.width * dermTiff.height);
        ldt::Pencil pencil;
        if (ldt::io::ReadLayer(path, layerIndex, raster.data(), &pencil)) {
            // Success
        } else {
            // Failure
        }
    }
}

// Write TIFF file
std::vector<uint32_t*> rasters(pageCount);      // Array of pointers of image data
std::vector<ldt::Pencil> pencils(pageCount);    // Array of layer pencils
for (uint16_t page = 0; page < pageCount; page++) {
    if (page == 0) {
        // Original image
        rasters[page] = originalImageRaster.data();
    } else {
        // Layers
        rasters[page] = layerRasters[page - 1].data();
        pencils[page] = ldt::Pencil{name, r, g, b, a};
    }
}

bool success = ldt::io::WriteTIFF("image.tiff", pageCount, width, height, rasters.data(), pencils.data());
```

## Dependencies

- [zlib](https://github.com/madler/zlib) v1.2.12  
  Use to support deflate compression.
- [libtiff](https://gitlab.com/libtiff/libtiff) v4.4.0
