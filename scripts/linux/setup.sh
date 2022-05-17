#!/bin/bash
running_path=`pwd`
script_path=$(cd $(dirname $0); pwd)

# Build dependencies
bash ${script_path}/build_library.sh

cd $running_path