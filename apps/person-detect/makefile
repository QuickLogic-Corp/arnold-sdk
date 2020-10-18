TARGET := quicklogic-arnold
TARGET_TOOLCHAIN_ROOT := /opt/riscv/bin/
TARGET_TOOLCHAIN_PREFIX := riscv32-unknown-elf-
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


CXXFLAGS += -std=c++11 -DTF_LITE_STATIC_MEMORY -DNDEBUG -O3 -D__riscv__y -mfdiv -fdata-sections -ffunction-sections -MMD -MP -DTF_LITE_MCU_DEBUG_LOG -DTF_LITE_USE_GLOBAL_CMATH_FUNCTIONS -DTF_LITE_USE_GLOBAL_MAX -DTF_LITE_USE_GLOBAL_MIN -fpermissive -fno-rtti -fno-exceptions --std=gnu++11 -I. -I./third_party/gemmlowp -I./third_party/flatbuffers/include -I./third_party/ruy -I./third_party/kissfft -I${PULP_BUILDER}/pulp-rt
CCFLAGS +=  -std=c11   -DTF_LITE_STATIC_MEMORY -DNDEBUG -O3 -D__riscv__y -mfdiv -fdata-sections -ffunction-sections -MMD -MP -I. -I./third_party/gemmlowp -I./third_party/flatbuffers/include -I./third_party/ruy -I./third_party/kissfft 

LDFLAGS += -D__riscv__ -MMD -MP -lbench -nostartfiles -nostdlib -Wl,--gc-sections -L${PULP_BUILDER}/install/rules -Tlink.ld -L${PULP_BUILDER}/install/lib/arnold  -lrt -lrtio -lrt -lgcc -lm 

include tensorflow/tensorflow/lite/micro/tools/make/targets/quicklogic-arnold_makefile.inc

LDFLAGS = -D__riscv__ -MMD -MP -lbench -nostartfiles -nostdlib -Wl,--gc-sections -L${PULP_BUILDER}/install/rules -Tlink.ld -L${PULP_BUILDER}/install/lib/arnold -L${PULP_SDK_HOME}/install/lib/arnold/arnold -lrt -lrtio -lrt -lgcc -lm
LDFLAGS += \
	-MMD\
	-MP\
	-lbench\
	-nostartfiles\
	-nostdlib\
	-Wl,--gc-sections\
	-L${PULP_BUILDER}/install/rules\
	-L${PULP_BUILDER}/install/lib/arnold\
	-lbench\
	-lrt\
	-lrtio\
	-lrt\
	-lgcc
PRJ_ROOT = tensorflow/tensorflow/lite/micro/tools/make/gen/quicklogic-arnold_rv32imfcxpulpv2/prj
BUILD_DIR = ${PULP_SDK_HOME}/pulp-runtime-examples/new_example/build
TFLM_DIR = ../../tensorflow/tensorflow/lite/micro

current: run_person_detection_test

###### GPIO toggle test ############
run_gpio_toggle:	gpio_toggle
		cp $< ${BUILD_DIR}/arnold/test
		#cp $< $<.elf .
		${PULP_SDK_HOME}/install/ws/bin/pulp-run\
			--config-file=arnold@config_file=chips/arnold/arnold.json\
			--config-opt=platform=board\
			--config-opt=rt/start-all=true\
			--config-opt=**/rt/fc-start=true\
			--config-opt=**/rt/type=pulp-rt\
			--config-opt=rt/start-all=true\
			--config-opt=**/rt/fc-start=true\
			--config-opt=**/rt/type=pulp-rt\
			--config=**/rt/iodev=uart
			--dir=${PULP_SDK_HOME}/pulp-runtime-examples/new_example/build/arnold\
			--binary=test/$(<F)

gpio_toggle: gpio_toggle.o gpio.o
	#$(CXX) $(CXXFLAGS) -o $@ gpio_toggle.o gpio.o $(LDFLAGS)
	$(CXX)  -o $@ gpio_toggle.o gpio.o $(LDFLAGS)


###### START person_detection_benchmark ############
EXAMPLE = person_detection_test
run_${EXAMPLE}:	$(PRJ_ROOT)/${EXAMPLE}/make/${EXAMPLE}
		cp $< ${BUILD_DIR}/arnold/test/.
		#cp $< $<.elf .
		${PULP_SDK_HOME}/install/ws/bin/pulp-run\
			--config-file=arnold@config_file=chips/arnold/arnold.json\
			--config-opt=**/rt/iodev=uart\
			--config-opt=platform=board\
			--config-opt=rt/start-all=true\
			--config-opt=**/rt/fc-start=true\
			--config-opt=**/rt/type=pulp-rt\
			--config-opt=rt/start-all=true\
			--config-opt=**/rt/fc-start=true\
			--config-opt=**/rt/type=pulp-rt\
			--dir=${PULP_SDK_HOME}/pulp-runtime-examples/new_example/build/arnold/\
			--binary=test/$(<F)

$(PRJ_ROOT)/${EXAMPLE}/make/${EXAMPLE}:	${PRJ_ROOT}/${EXAMPLE} ${TFLM_DIR}/quicklogic-arnold/person_detection_test.cc\
	${TFLM_DIR}/quicklogic-arnold/fll.cc ${TFLM_DIR}/quicklogic-arnold/gpio.cc\
	${TFLM_DIR}/quicklogic-arnold/micro_interpreter.cc ${TFLM_DIR}/quicklogic-arnold/conv.cc\
	${TFLM_DIR}/quicklogic-arnold/programFPGA.cc\
	${TFLM_DIR}/quicklogic-arnold/rt_conf.c\
	${TFLM_DIR}/quicklogic-arnold/rt_pad_conf.c\
	${TFLM_DIR}/quicklogic-arnold/conv.h
		cp ${TFLM_DIR}/quicklogic-arnold/person_detection_test.cc ${PRJ_ROOT}/${EXAMPLE}/make/tensorflow/lite/micro/examples/person_detection
		cp ${TFLM_DIR}/quicklogic-arnold/* ${PRJ_ROOT}/${EXAMPLE}/make/tensorflow/lite/micro/quicklogic-arnold
		cp ${TFLM_DIR}/quicklogic-arnold/conv.cc ${PRJ_ROOT}/${EXAMPLE}/make/tensorflow/lite/micro/kernels
		cp ${TFLM_DIR}/quicklogic-arnold/conv.h ${PRJ_ROOT}/${EXAMPLE}/make/tensorflow/lite/kernels/internal/reference
		#sed s/-O3// ${PRJ_ROOT}/${EXAMPLE}/make/makefile.orig > ${PRJ_ROOT}/${EXAMPLE}/make/makefile
		#-make --directory=${PRJ_ROOT}/${EXAMPLE}/make all
		#sed s/-O3/-O3/ ${PRJ_ROOT}/${EXAMPLE}/make/makefile.orig > ${PRJ_ROOT}/${EXAMPLE}/make/makefile
		#rm ${PRJ_ROOT}/${EXAMPLE}/make/tensorflow/lite/micro/micro_error_reporter.o
		#rm ${PRJ_ROOT}/${EXAMPLE}/make/tensorflow/lite/core/api/error_reporter.o
		make --directory=${PRJ_ROOT}/${EXAMPLE}/make all
		
$(PRJ_ROOT)/${EXAMPLE}:
		make --directory=tensorflow -f tensorflow/lite/micro/tools/make/Makefile generate_${EXAMPLE}_make_project TARGET=quicklogic-arnold
		cp ${PRJ_ROOT}/${EXAMPLE}/make/makefile ${PRJ_ROOT}/${EXAMPLE}/make/makefile.orig



##### END person_detection_benchmark #########

###### START micro_error_reporter_test ############  works
run_micro_error_reporter_test:	$(PRJ_ROOT)/micro_error_reporter_test/make/micro_error_reporter_test
		cp $< ${BUILD_DIR}/arnold/test
		${PULP_SDK_HOME}/install/ws/bin/pulp-run\
			--config-file=arnold@config_file=chips/arnold/arnold.json\
			--config-opt=platform=board\
			--config-opt=rt/start-all=true\
			--config-opt=**/rt/fc-start=true\
			--config-opt=**/rt/type=pulp-rt\
			--config-opt=rt/start-all=true\
			--config-opt=**/rt/fc-start=true\
			--config-opt=**/rt/type=pulp-rt\
			--dir=${PULP_SDK_HOME}/pulp-runtime-examples/new_example/build/arnold\
			--binary=test/$(<F)

$(PRJ_ROOT)/micro_error_reporter_test/make/micro_error_reporter_test:	${PRJ_ROOT}/micro_error_reporter_test
		make --directory=${PRJ_ROOT}/micro_error_reporter_test/make all

$(PRJ_ROOT)/micro_error_reporter_test:
		make --directory=tensorflow -f tensorflow/lite/micro/tools/make/Makefile generate_micro_error_reporter_test_make_project TARGET=quicklogic-arnold

###### END micro_error_reporter_test #########

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
	rm -rf tensorflow/tensorflow/lite/micro/tools/make/gen/quicklogic-arnold_rv32imfcxpulpv2
  

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.c
	$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

