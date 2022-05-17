#!/bin/bash
running_path=`pwd`
script_path=$(cd $(dirname $0); pwd)
repo_path=`readlink -f ${script_path}/../../`

build() {
    echo -e "\e[32mBuild and install $1\e[m"
    cd ${repo_path}/deps/$1
    mkdir -p build/
    cd build/
    cmake ..
    cmake --build . --config Release
    cmake --install .
}

# Build and install ZLIB
build "zlib"

cd $running_path