#!/bin bash

#
# Script to setup pulp tool chain
#
LOGFILE=setup.log

echo "Arnold tool chain installation" |& tee ${LOGFILE}

######################################
#
# Install various generic tools
#
######################################

echo "## Install various generic tools" |& tee --append ${LOGFILE}

echo Y | sudo apt-get install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison\
        flex texinfo gperf libtool patchutils bc zlib1g-dev scons libftdi1-dev &>> ${LOGFILE}

#
# for some reason combining into one call to pip3 does not work for numpy and elftools
#
echo "+++++++++++++++install elftools" |& tee --append ${LOGFILE}
pip3 install elftools &>> ${LOGFILE}
echo "---------------install setuptools" |& tee --append ${LOGFILE}
pip3 install setuptools &>> ${LOGFILE}
echo "---------------install prettytable" |& tee --append ${LOGFILE}
pip3 install prettytable &>> ${LOGFILE}
echo "---------------install numpy" |& tee --append ${LOGFILE}
pip3 install numpy &>> ${LOGFILE}
echo "---------------install configparser" |& tee --append ${LOGFILE}
pip3 install configparser &>> ${LOGFILE}
echo "---------------pip3 done" |& tee --append ${LOGFILE}

#
# Some utilities specify python2.7 so we load their packages
echo "+++++++++++++++pip" |& tee --append ${LOGFILE}
pip install configparser &>> ${LOGFILE}
echo "---------------pip done" |& tee --append ${LOGFILE}

#################################
#
# Install riscv toolchain 
#
#################################

echo "## Install riscv toolchain" |& tee --append ${LOGFILE}
rm -rf pulp-riscv-gnu-toolchain
git clone --recursive https://github.com/pulp-platform/pulp-riscv-gnu-toolchain.git &>> ${LOGFILE}

pushd pulp-riscv-gnu-toolchain &>> ${LOGFILE}
./configure --prefix=/opt/riscv --with-arch=rv32imc --with-cmodel=medlow --enable-multilib &>> ${LOGFILE}
sudo make &>> ${LOGFILE}
popd &>> ${LOGFILE}
export PULP_RISCV_GCC_TOOLCHAIN=/opt/riscv

#
#  Install pulp-builder
#
echo "## Installing pulp-builder" |& tee --append ${LOGFILE}
echo "* remove previous version" |& tee --append ${LOGFILE}
rm -rf pulp-builder &>> ${LOGFILE}
echo "* clone pulp-builder" |& tee --append ${LOGFILE}
git clone https://github.com/pulp-platform/pulp-builder.git &>> ${LOGFILE}
echo "* ls pulp-builder" |& tee --append ${LOGFILE}
pushd pulp-builder &>> ${LOGFILE}
echo "* checkout arnold" |& tee --append ${LOGFILE}
git checkout arnold &>> ${LOGFILE}
source configs/arnold.sh &>> ${LOGFILE}
source configs/board.sh &>> ${LOGFILE}
echo "-------------------------./scripts/update-runtime" |& tee --append ${LOGFILE}
./scripts/update-runtime &>> ${LOGFILE}
echo "-------------------------./scripts/build-runtime" |& tee --append ${LOGFILE}
./scripts/build-runtime &>> ${LOGFILE}
popd &>> ${LOGFILE}


pushd pulp-builder &>> ${LOGFILE}
echo"" |& tee --append ${LOGFILE}
echo"" |& tee --append ${LOGFILE}
echo"" |& tee --append ${LOGFILE}
echo "----------------------sdk-setup.sh" |& tee --append ${LOGFILE}
source sdk-setup.sh &>> ${LOGFILE}
export PULPRT_HOME=$PWD/pulp-rules
popd &>> ${LOGFILE}


#
# Install runtime examples
#
echo "## Install runtime examples" |& tee --append ${LOGFILE}
pushd pulp-builder &>> ${LOGFILE}
rm -rf pulp-runtime-examples &>> ${LOGFILE}
git clone https://github.com/pulp-platform/pulp-runtime-examples.git &>> ${LOGFILE}
popd &>> ${LOGFILE}

#
# Make runtime example hello
#

echo "## Make runtime example hello" |& tee --append ${LOGFILE}
pushd pulp-builder &>> ${LOGFILE}
export PULP_RISCV_GCC_TOOLCHAIN=/opt/riscv
source configs/arnold.sh &>> ${LOGFILE}
source configs/board.sh &>> ${LOGFILE}
source sdk-setup.sh &>> ${LOGFILE}
export PULPRT_HOME=$PWD/pulp-rules
cd pulp-runtime-examples/hello &>> ${LOGFILE}

echo "* make all" |& tee --append ${LOGFILE}
make all &>> ${LOGFILE}
