# libdermtiff

C++ library to read / write [DermAnnotation](https://kondoa9.github.io/DermAnnotation/) TIFF files.

This requires C++17 or later.

## Setup

```powershell
# Windows
.\scripts\windows\setup.ps1 # Run as admin
```

```zsh
# macOS
sudo zsh scripts/macos/setup.sh
```

```bash
# Linux
sudo bash scripts/linux/setup.sh
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
if (const auto dermTiff = ldt::Open(path); dermTiff.isValid) {
    for (uint16_t page = 0; page < dermTiff.pages; page++) {
        std::vector<uint32_t> raster(dermTiff.width * dermTiff.height);
        ldt::Pencil pencil;
        if (ldt::ReadPage(path, page, raster.data(), &pencil)) {
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

bool success = ldt::WriteTIFF("image.tiff", pageCount, width, height, rasters.data(), pencils.data());
```

## Dependencies

- [libtiff](https://gitlab.com/libtiff/libtiff) v4.3.0
