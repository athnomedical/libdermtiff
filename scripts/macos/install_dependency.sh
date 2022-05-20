#!/bin/zsh
running_path=`pwd`
script_path=$(cd $(dirname $0); pwd)
repo_path="${script_path}/../../"

# Build and install dependency.
install_dependency() {
    echo -e "\e[32mBuild and install $1\e[m"
    cd ${repo_path}/deps/$1
    mkdir -p build/
    cd build/
    cmake -DBUILD_SHARED_LIBS=ON ..
    cmake --build . --config Release
    sudo cmake --install .

    # Clean library repo
    cd ../
    git restore .
    git clean -f -d
}

# Build and install ZLIB
install_dependency "zlib" "libz"

cd $running_path