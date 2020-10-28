#!/bin bash

#############################################
#
# Script to install pulp tool chain
#
#############################################
# make absolute, otherwise parts end up in the subdirectories
LOGFILE=${PWD}/install.log

echo "Arnold tool chain installation" |& tee ${LOGFILE}

#############################################
#
# Code to manage command line arguments that enable/disable various steps
#
#############################################
flags=(	"help"				"xxxx"\
			"riscv-toolchain" 	"false"\
			"pulp-builder" 		"true"\
			"tensorflow" 		"true")
			
declare -A steps
for i in ${!flags[@]}; do
	let tmp=i%2
	if [ $tmp -eq 0 ]
	then
		steps[${flags[i]}]=${flags[i+1]}
	fi
done

while true; do
	if [ $1 ]; then
		stringz=$1
		code=${stringz:0:2}
		flag=${stringz:2}
		if [ ${code} != "--" ] && [ ${code} != "++" ]; then
			echo "ERROR: badly formed argument " $1
			exit 1
		fi
		stringz=${steps[${flag}]}
		if [ Z${stringz}Z == "ZZ" ]; then
			echo "ERROR: unknown argument " $1
		fi
		if [ ${code} == "--" ]; then
			steps[${flag}]="false"
		fi
		if [ ${code} == "++" ]; then
			steps[${flag}]="true"
		fi
		shift
	else
		break
	fi
done

if [ ${steps["help"]} != "xxxx" ]; then
	echo "Default settings listed below"
	echo "To override use '--step' to disable installation and '++step' to enable installation"
	for i in ${!flags[@]}; do
		let tmp=i%2
		if [ $tmp -eq 0 ] && [ $i -gt 0 ]
		then
			flag=${flags[i]}
			if [ ${steps[${flag}]} == "false" ]; then
				echo "   Step '"${flags[i]}"' disabled by default"
			fi
			if [ ${steps[${flag}]} == "true" ]; then
				echo "   Step '"${flags[i]}"' enabled by default"
			fi
		fi
	done
	exit 1
fi

for i in ${!flags[@]}; do
	let tmp=i%2
	if [ $tmp -eq 0 ] && [ $i -gt 0 ]
	then
		flag=${flags[i]}
		if [ ${steps[${flag}]} == "false" ]; then
			echo "   Step '"${flags[i]}"' is disabled"
		fi
		if [ ${steps[${flag}]} == "true" ]; then
			echo "   Step '"${flags[i]}"' is enabled"
		fi
	fi
done


######################################
#
# Install various generic tools
#
######################################
set -e

echo "## Install various generic tools" |& tee --append ${LOGFILE}

echo Y | sudo apt-get install autoconf automake autotools-dev curl\
	      libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison\
          flex texinfo gperf libtool patchutils bc zlib1g-dev scons\
	      cmake libftdi1-dev &>> ${LOGFILE}

#
# for some reason combining into one call to pip3 does not work for numpy and elftools
#
echo "+++++++++++++++install pyelftools" |& tee --append ${LOGFILE}
pip3 install pyelftools &>> ${LOGFILE}
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
if [  ${steps["riscv-toolchain"]} == "true"  ]; then

	echo "## Install riscv toolchain" |& tee --append ${LOGFILE}
	rm -rf pulp-riscv-gnu-toolchain
	git clone --recursive https://github.com/pulp-platform/pulp-riscv-gnu-toolchain.git &>> ${LOGFILE}

	pushd pulp-riscv-gnu-toolchain &>> ${LOGFILE}
	echo "+++++++++++++++configure" |& tee --append ${LOGFILE}
	./configure --prefix=/opt/riscv --with-arch=rv32imc --with-cmodel=medlow --enable-multilib &>> ${LOGFILE}
	echo "---------------make" |& tee --append ${LOGFILE}
	sudo make &>> ${LOGFILE}
	popd &>> ${LOGFILE}
fi
export PULP_RISCV_GCC_TOOLCHAIN=/opt/riscv

################################
#
#  Install pulp-builder
#
################################
if [ ${steps["pulp-builder"]}  == "true" ]; then
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
	echo "----------------------./scripts/update-runtime" |& tee --append ${LOGFILE}
	./scripts/update-runtime &>> ${LOGFILE}
	echo "----------------------./scripts/build-runtime" |& tee --append ${LOGFILE}
	./scripts/build-runtime &>> ${LOGFILE}
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
	# Install patches (overrides that we have not upstreamed)
	#
	pushd pulp-builder &>> ${LOGFILE}
	echo "scripts/clean" |& tee --append ${LOGFILE}
	source ./scripts/clean &>> ${LOGFILE}
	echo "----------------------./scripts/build-runtime" |& tee --append ${LOGFILE}
	./scripts/build-runtime &>> ${LOGFILE}
	popd &>> ${LOGFILE}
	
	echo "----------------------install patches" |& tee --append ${LOGFILE}
	pushd patches &>> ${LOGFILE}
	bash patch.bash |& tee --append ${LOGFILE}
	popd &>> ${LOGFILE}
	

	pushd pulp-builder &>> ${LOGFILE}
	echo "----------------------sdk-setup.sh" |& tee --append ${LOGFILE}
	source sdk-setup.sh &>> ${LOGFILE}
	export PULPRT_HOME=$PWD/pulp-rules
	popd &>> ${LOGFILE}
fi

################################
#
#  Install tensorflow
#
################################
if [ ${steps["tensorflow"]}  == "true" ]; then
	echo "----------------------clone tensorflow" |& tee --append ${LOGFILE}
	pushd tensorflow &>> ${LOGFILE}
	git submodule update --init --recursive &>> ${LOGFILE}
	git checkout master &>> ${LOGFILE}
	popd &>> ${LOGFILE}
fi

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

echo "Arnold install completed successfully" |& tee --append ${LOGFILE}
