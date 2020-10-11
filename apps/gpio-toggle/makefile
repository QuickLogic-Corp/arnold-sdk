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


CXXFLAGS += -std=c++11 -DTF_LITE_STATIC_MEMORY -DNDEBUG -O3 -D__riscv__y -mfdiv -fdata-sections -ffunction-sections -MMD -MP -DTF_LITE_MCU_DEBUG_LOG -DTF_LITE_USE_GLOBAL_CMATH_FUNCTIONS -DTF_LITE_USE_GLOBAL_MAX -DTF_LITE_USE_GLOBAL_MIN -fpermissive -fno-rtti -fno-exceptions --std=gnu++11 -I. -I./third_party/gemmlowp -I./third_party/flatbuffers/include -I./third_party/ruy -I./third_party/kissfft
CCFLAGS +=  -std=c11   -DTF_LITE_STATIC_MEMORY -DNDEBUG -O3 -D__riscv__y -mfdiv -fdata-sections -ffunction-sections -MMD -MP -I. -I./third_party/gemmlowp -I./third_party/flatbuffers/include -I./third_party/ruy -I./third_party/kissfft

LDFLAGS += -D__riscv__ -MMD -MP -lbench -nostartfiles -nostdlib -Wl,--gc-sections -L/home/gmartin/ETH/TRY5/arnold/pulp-builder/install/rules -T/mnt/hgfs/Documents/ActiveProjects/arnold/arnold/link.ld -L/home/gmartin/ETH/TRY5/arnold/pulp-builder/install/lib/arnold -L/home/gmartin/ETH/TRY5/arnold/pulp-builder/install/lib/arnold/arnold -lrt -lrtio -lrt -lgcc -lm

include ${TENSORFLOWHOME}/tensorflow/lite/micro/tools/make/targets/quicklogic-arnold_makefile.inc

LDFLAGS = -D__riscv__ -MMD -MP -lbench -nostartfiles -nostdlib -Wl,--gc-sections -L/home/gmartin/ETH/TRY5/arnold/pulp-builder/install/rules -T/mnt/hgfs/Documents/ActiveProjects/arnold/arnold/link.ld -L/home/gmartin/ETH/TRY5/arnold/pulp-builder/install/lib/arnold -L/home/gmartin/ETH/TRY5/arnold/pulp-builder/install/lib/arnold/arnold -lrt -lrtio -lrt -lgcc -lm
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
BUILD_DIR = ${PULP_BUILDER}/pulp-rt-examples/arnold_tests/new_example/build
TFLM_DIR = ${TENSORFLOWHOME}/tensorflow/lite/micro

current: run_gpio_toggle

###### GPIO toggle test ############
run_gpio_toggle:	gpio_toggle
		${PULP_BUILDER}/install/ws/bin/pulp-run\
			--config-file=arnold@config_file=chips/arnold/arnold.json\
			--config-opt=platform=board\
			--config-opt=rt/start-all=true\
			--config-opt=**/rt/fc-start=true\
			--config-opt=**/rt/type=pulp-rt\
			--config-opt=rt/start-all=true\
			--config-opt=**/rt/fc-start=true\
			--config-opt=**/rt/type=pulp-rt\
			--dir=${PWD}
			--binary=$(<F)

gpio_toggle: gpio_toggle.o gpio.o
	#$(CXX) $(CXXFLAGS) -o $@ gpio_toggle.o gpio.o $(LDFLAGS)
	$(CXX)  -o $@ gpio_toggle.o gpio.o $(LDFLAGS)


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
	rm gpio_toggle gpio_toggle.o
  

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.c
	$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

