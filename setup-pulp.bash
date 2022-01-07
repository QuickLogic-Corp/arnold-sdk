#!/bin/bash

#
# setup environment to run pulp tools
#
# you should 'source' this file before attempting to run any pulp tools
#
echo "setup pulp environment"
pushd pulp-builder
export PULP_RISCV_GCC_TOOLCHAIN=/opt/pulp-gcc-ubuntu1804-20200913
source configs/arnold.sh
source configs/board.sh
source sdk-setup.sh
export PULPRT_HOME=$PWD/pulp-rules
popd
