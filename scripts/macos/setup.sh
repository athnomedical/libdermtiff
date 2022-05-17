#!/bin/zsh
running_path=`pwd`
script_path=$(cd $(dirname $0); pwd)
repo_path=`readlink -f ${script_path}/../../`

# Update submodules
git submodule update --init

# Workaround for compiling error of libtiff
script_path=$(cd $(dirname $0); pwd)
mv ${repo_path}/deps/libtiff/VERSION ${repo_path}/deps/libtiff/VERSION.txt

# Build dependencies
zsh ${script_path}/build_library.sh

cd $running_path