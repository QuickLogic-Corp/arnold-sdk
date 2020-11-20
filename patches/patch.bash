#!/bin/bash
#######################################################
#
# patch any files in the pulp install
#
#######################################################

# fll-v1.c needs updating
echo "in patch.bash"
cp bridge-makefile ../pulp-builder/pulp-debug-bridge/Makefile
cp fll-v1.c ../pulp-builder/pulp-rt/kernel/
cp bridge.c ../pulp-builder/pulp-rt/kernel/
cp rt_freq.h ../pulp-builder/pulp-rt/include/rt/
cp camera.c ../pulp-builder/pulp-rt/drivers_deprecated/camera/
cp himax.c ../pulp-builder/pulp-rt/drivers_deprecated/camera/
cp implem.h ../pulp-builder/pulp-rt/include/pmsis/implem/ || (echo "copy failed"; exit 1)
