#!/bin/bash
#######################################################
#
# patch any files in the pulp install
#
#######################################################

# fll-v1.c needs updating
echo "in patch.bash"
cp bridge-makefile ../pulp-builder/pulp-rt/pulp-debug-bridge/Makefile
cp fll-v1.c ../pulp-builder/pulp-rt/kernel/
cp bridge.c ../pulp-builder/pulp-rt/kernel/
cp rt_freq.h ../pulp-builder/pulp-rt/include/rt/
cp implem.h ../pulp-builder/pulp-rt/include/pmsis/implem/ || (echo "copy failed"; exit 1)
