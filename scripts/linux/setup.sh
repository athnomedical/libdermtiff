#!/bin/bash
running_path=`pwd`
script_path=$(cd $(dirname $0); pwd)
repo_path=`readlink -f ${script_path}/../../`

# Build and install library.
install_library() {
    echo -e "\e[32mBuild and install $1\e[m"
    cd ${repo_path}/deps/$1
    mkdir -p build/
    cd build/
    cmake ..
    cmake --build . --config Release
    cmake --install .

    # Copy shared object to bin/
    mkdir -p ${repo_path}/bin/
    cp ${repo_path}/deps/$1/build/$2*.so ${repo_path}/bin/

    # Clean library repo
    cd ../
    git restore *
    git clean -f -d
}

# Build and install ZLIB
install_library "zlib" "libz"

cd $running_path