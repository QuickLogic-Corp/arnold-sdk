# arnold-sdk
Repo to support evaluations using arnold evaluation kit
## Installing the toolchains
There are several steps to a complete installation:
1. command to clone the arnold-sdk repository, then install various other elements
2. install.bash script
  - python -- the install process requires both python2 and python3 to be present before the install starts
  - generic tools -- the install requires various tools and packages -- these are always installed
  - riscv-toolchain -- the RI5CY core requires a specific gcc toolchain -- since this takes a long time to generate, it is *not* installed by default.
If you have not previously installed this toolchain, you will want to specify the ++rscv-toolchain option to the insyall script.
  - pulp-builder -- installs and configures the pulp build system -- this is installed by default.  You can disable installation by specifying --pulp-builder to the install script.
  - tensorflow -- installs tensorflow as a submodule -- this is installed by default.  It is quite time-consuming so if you are not using tensorflow you
can disable installation by specifying --tensorflow to the install script.
3. install Olimex [*** need to add instructions for this]
4. script to setup environment
5. make application

If the RI5CY toolchain is not installed use:
~~~
git clone https://github.com/QuickLogic-Corp/arnold-sdk.git
cd arnold-sdk
bash install.bash ++riscv-toolchain  		# check that you see "Arnold install completed successfully" -- details may be found in file 'install.log'
source setup-pulp.bash						# setup environment to run pulp build system
cd tflu-apps/person-detection
make
~~~

If the RIS5CY toolchain is already installed use:
~~~
git clone https://github.com/QuickLogic-Corp/arnold-sdk.git
cd arnold-sdk
bash install.bash   						# check that you see "Arnold install completed successfully" -- details may be found in file 'install.log'
source setup-pulp.bash						# setup environment to run pulp build system
cd tflu-apps/person-detection
make
~~~
