#!/bin/zsh
running_path=`pwd`
script_path=$(cd $(dirname $0); pwd)

# Build dependencies
zsh ${script_path}/install_dependency.sh

cd $running_path