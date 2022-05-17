#!/bin/bash
running_path=`pwd`
script_path=$(cd $(dirname $0); pwd)

# Update submodules
git submodule update --init

# Build dependencies
bash ${script_path}/build_library.sh

cd $running_path