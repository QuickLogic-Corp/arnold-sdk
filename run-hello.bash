#!/bin/bash

#
# Make runtime example hello
#
echo "Make runtime example hello"
pushd pulp-builder
export PULP_RISCV_GCC_TOOLCHAIN=/opt/riscv
source configs/arnold.sh
source configs/board.sh
source sdk-setup.sh
export PULPRT_HOME=$PWD/pulp-rules
cd pulp-runtime-examples/hello

groups
echo "* make all run"
make all run