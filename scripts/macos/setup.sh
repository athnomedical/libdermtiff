#!/bin/zsh
running_path=`pwd`
script_path=$(cd $(dirname $0); pwd)
repo_path=`readlink -f ${script_path}/../../`

# Workaround for compiling error of libtiff
script_path=$(cd $(dirname $0); pwd)
mv ${script_path}/../deps/libtiff/VERSION ${script_path}/../deps/libtiff/VERSION.txt

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
    cp ${repo_path}/deps/$1/build/$2*.dylib ${repo_path}/bin/
}

# Build and install ZLIB
install_library "zlib" "libz"

cd $running_path