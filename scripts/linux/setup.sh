#!/bin/bash
running_path=`pwd`
script_path=$(cd $(dirname $0); pwd)

# Build dependencies
bash ${script_path}/install_dependency.sh

cd $running_path