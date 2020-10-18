#!/bin/bash
#######################################################
#
# patch any files in the pulp install
#
#######################################################

# fll-v1.c needs updating
echo "IN PATCH.BASH"
cp fll-v1.c ../pulp-builder/pulp-rt/kernel/
echo "%%%%%%%%%%%%%%%%%%%"
cat implem.h
echo "*********************************************"
cp implem.h ../pulp-builder/install/include/pmsis/implem/ || (echo "copy failed"; exit 1)