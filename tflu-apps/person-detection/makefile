TARGET := quicklogic-arnold
TARGET_TOOLCHAIN_ROOT := /opt/riscv/bin/
TARGET_TOOLCHAIN_PREFIX := riscv32-unknown-elf-
TENSORFLOWHOME := ../../tensorflow

# These are microcontroller-specific rules for converting the ELF output
# of the linker into a binary image that can be loaded directly.
CXX             := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)g++'
CC              := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)gcc'
AS              := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)as'
AR              := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)ar' 
LD              := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)ld'
NM              := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)nm'
OBJDUMP         := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)objdump'
OBJCOPY         := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)objcopy'
SIZE            := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)size'

RM = rm -f
ARFLAGS := -csr


CXXFLAGS += -std=c++11\
	-DTF_LITE_STATIC_MEMORY\
	-DNDEBUG\
	-march=rv32imfcxpulpv2\
	-mfdiv\
	-D__riscv__\
	-O3\
	-g\
	-fdata-sections\
	-ffunction-sections\
	-MMD\
	-MP -DTF_LITE_MCU_DEBUG_LOG -DTF_LITE_USE_GLOBAL_CMATH_FUNCTIONS -DTF_LITE_USE_GLOBAL_MAX -DTF_LITE_USE_GLOBAL_MIN\
	-fpermissive\
	-fno-rtti\
	-fno-exceptions\
	--std=gnu++11\
	-I${PULP_SDK_HOME}/install/include/io\
	-I${PULP_SDK_HOME}/install/include\
	-I${PULP_SDK_HOME}/pulp-rt/include\
	-include fconfig.h\
	-I. -I./third_party/gemmlowp -I./third_party/flatbuffers/include -I./third_party/ruy -I./third_party/kissfft
CCFLAGS +=  -std=c11\
   -DTF_LITE_STATIC_MEMORY\
   -DNDEBUG -O3 -D__riscv__y -mfdiv -fdata-sections -ffunction-sections -MMD -MP\
   -I.\
    -I./third_party/kissfft\
   -I./third_party/gemmlowp\
   -I./third_party/flatbuffers/include\
   -I./third_party/ruy\
   -I${PULP_SDK_HOME}/install/include/io\
	-I${PULP_SDK_HOME}/install/include\
	-I${PULP_SDK_HOME}/pulp-rt/include\

LDFLAGS +=\
	-D__riscv__ \
	-MMD\
	-MP\
	-lbench\
	-nostartfiles\
	-nostdlib\
	-Wl,--gc-sections\
	-L${PULP_SDK_HOME}/install/rules\
	-T/mnt/hgfs/Documents/ActiveProjects/arnold/arnold/link.ld\
	-L${PULP_SDK_HOME}/install/lib/arnold\
	-L${PULP_SDK_HOME}/install/lib/arnold/arnold\
	-lrt\
	-lrtio\
	-lrt\
	-lgcc\
	-lm

include  ${TENSORFLOWHOME}/tensorflow/lite/micro/tools/make/targets/quicklogic-arnold_makefile.inc

LDFLAGS = -D__riscv__ -MMD -MP -lbench -nostartfiles -nostdlib -Wl,--gc-sections -L/home/gmartin/ETH/TRY5/arnold/pulp-builder/install/rules\
	-T/mnt/hgfs/Documents/ActiveProjects/arnold/arnold/link.ld\
	-L/home/gmartin/ETH/TRY5/arnold/pulp-builder/install/lib/arnold -L/home/gmartin/ETH/TRY5/arnold/pulp-builder/install/lib/arnold/arnold -lrt -lrtio -lrt -lgcc -lm
LDFLAGS += \
	-MMD\
	-MP\
	-lbench\
	-nostartfiles\
	-nostdlib\
	-Wl,--gc-sections\
	-L${PULP_BUILDER}/install/rules\
	-L${PULP_BUILDER}/install/lib/arnold\
	-L${PULP_BUILDER}/install/lib/arnold/arnold\
	-lrt\
	-lrtio\
	-lrt\
	-lgcc
PRJ_ROOT = ${TENSORFLOWHOME}/tensorflow/lite/micro/tools/make/gen/quicklogic-arnold_rv32imfcxpulpv2/prj
BUILD_DIR = ~/ETH/TRY5/arnold/pulp-builder/pulp-rt-examples/arnold_tests/new_example/build
TFLM_DIR = ${TENSORFLOWHOME}/tensorflow/lite/micro

current: run_person_detection_test


###### START person_detection_benchmark ############
EXAMPLE = person_detection_test
run_${EXAMPLE}:	$(PRJ_ROOT)/${EXAMPLE}/make/${EXAMPLE}
		echo "## running ${EXAMPLE}"
		echo "* copy the app from the generated project to here"
		cp $< .
		echo "* run it"
		${PULP_BUILDER}/install/ws/bin/pulp-run\
			--config-file=arnold@config_file=chips/arnold/arnold.json\
			--config-opt=platform=board\
			--config-opt=rt/start-all=true\
			--config-opt=**/rt/fc-start=true\
			--config-opt=**/rt/type=pulp-rt\
			--config-opt=rt/start-all=true\
			--config-opt=**/rt/fc-start=true\
			--config-opt=**/rt/type=pulp-rt\
			--dir=${pwd}\
			--binary=$(<F)

##########################################################################
#
#   This section builds the application using the makefile in the generated project
#   Since the generated makefile assume that the generated project is complete so it does *not* have dependency links to quicklogc-arnold
#   however, during development we want to change the real soruces in quicklogic-arnold, not the copied (and volatile) sorurce sin teh egenrated project
#   so this section copies files from quicklogic-arnols into the appropriate place in the generated project.  Unfortunately, setting this up is manual,
#   so be sure that all files you are editing are included here.
#
##########################################################################
$(PRJ_ROOT)/${EXAMPLE}/make/${EXAMPLE}:	${PRJ_ROOT}/${EXAMPLE} ${TFLM_DIR}/quicklogic-arnold/person_detection_test.cc ${TFLM_DIR}/quicklogic-arnold/fll.cc ${TFLM_DIR}/quicklogic-arnold/gpio.cc\
																				${TFLM_DIR}/quicklogic-arnold/micro_interpreter.cc ${TFLM_DIR}/quicklogic-arnold/conv.cc\
																				${TFLM_DIR}/quicklogic-arnold/conv.h
		echo "## building generated project"
		echo "* copy arnold specific source files into generated project"
		cp ${TFLM_DIR}/quicklogic-arnold/person_detection_test.cc ${PRJ_ROOT}/${EXAMPLE}/make/tensorflow/lite/micro/examples/person_detection
		cp ${TFLM_DIR}/quicklogic-arnold/* ${PRJ_ROOT}/${EXAMPLE}/make/tensorflow/lite/micro/quicklogic-arnold
		cp ${TFLM_DIR}/quicklogic-arnold/conv.cc ${PRJ_ROOT}/${EXAMPLE}/make/tensorflow/lite/micro/kernels
		cp ${TFLM_DIR}/quicklogic-arnold/conv.h ${PRJ_ROOT}/${EXAMPLE}/make/tensorflow/lite/kernels/internal/reference
		echo "* make the generated app"
		make --directory=${PRJ_ROOT}/${EXAMPLE}/make all
		echo "-------------app built"
        
##################################
# 
#  This code uses the existing tensorflow capability to generate a project -- the project is customized for quicklogic-arnold and located in 
#  ${TENSORFLOWHOME}/tensorflow/tensorflow/lite/micro/tools/make/gen/quicklogic-arnold_rv32imfcxpulpv2/prj
#
#   Customization means that files found in quicklogic-arnold replace files from the mainline
#
###################################
$(PRJ_ROOT)/${EXAMPLE}:
		echo "## Generating tensorflow project"
		make --directory=${TENSORFLOWHOME} -f tensorflow/lite/micro/tools/make/Makefile generate_${EXAMPLE}_make_project TARGET=quicklogic-arnold
		cp ${PRJ_ROOT}/${EXAMPLE}/make/Makefile ${PRJ_ROOT}/${EXAMPLE}/make/Makefile.orig
		echo "---------------project generated"



##### END person_detection_benchmark #########


all1:	${PRJ_ROOT}/generate_hello_world_make_project
		make --directory=${PRJ_ROOT}/hello_world/make all


all2:	../tf-arnold/tensorflow/lite/micro/tools/make/gen/quicklogic-arnold_rv32imfcxpulpv2
		make --directory=../tf-arnold/tensorflow/lite/micro/tools/make/gen/quicklogic-arnold_rv32imfcxpulpv2/prj/person_detection_test_int8/make all

orig:	../tf-arnold/tensorflow/lite/micro/tools/make/gen/quicklogic-arnold_rv32imfcxpulpv2
	make -f "Makefile - orig" --directory=../tf-arnold/tensorflow/lite/micro/tools/make/gen/quicklogic-arnold_rv32imfcxpulpv2/prj/person_detection_test_int8/make all

${PRJ_ROOT}/generate_hello_world_make_project:
	make --directory=../tf-arnold -f tensorflow/lite/micro/tools/make/Makefile generate_hello_world_make_project TARGET=quicklogic-arnold

${PRJ_ROOT}/generate_person_detection_test_int8_make_project:
	make --directory=../tf-arnold -f tensorflow/lite/micro/tools/make/Makefile generate_person_detection_test_int8_make_project TARGET=quicklogic-arnold

clean:
	rm -rf ${PRJ_ROOT}/${EXAMPLE}
  

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.c
	$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

