#! /bin/bash

workspaceRoot="${1}"
toolchian="${2}"

if [ ! -e ${workspaceRoot}/build ]; then 
    mkdir "${workspaceRoot}/build"
    pushd "${workspaceRoot}/build" 
    PATH=${toolchian}:${PATH} cmake .. 
    popd    
fi