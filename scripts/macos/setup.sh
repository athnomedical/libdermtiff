#!/bin/zsh
running_path=`pwd`
script_path=$(cd $(dirname $0); pwd)
repo_path="${script_path}/../../"

# Workaround for compiling error of libtiff
script_path=$(cd $(dirname $0); pwd)
mv ${repo_path}/deps/libtiff/VERSION ${repo_path}/deps/libtiff/VERSION.txt

# Build dependencies
zsh ${script_path}/install_dependency.sh

cd $running_path