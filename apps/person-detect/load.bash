PRJ_ROOT=../tf-arnold/tensorflow/lite/micro/tools/make/gen/quicklogic-arnold_rv32imfcxpulpv2/prj

echo "PRJ_ROOT=" ${PRJ_ROOT}
EXAMPLE=person_detection_test
DIRECTORY=${PRJ_ROOT}/${EXAMPLE}/make

cd ${DIRECTORY} && \
'/opt/riscv/bin/riscv32-unknown-elf-g++' \
	-std=c++11 \
	-DTF_LITE_STATIC_MEMORY \
	-DNDEBUG -O3 -D__riscv__ \
	-mfdiv \
	-fdata-sections \
	-ffunction-sections \
	-MMD \
	-MP \
	-DTF_LITE_MCU_DEBUG_LOG \
	-DTF_LITE_USE_GLOBAL_CMATH_FUNCTIONS \
	-DTF_LITE_USE_GLOBAL_MAX \
	-DTF_LITE_USE_GLOBAL_MIN \
	-fpermissive \
	-fno-rtti \
	-fno-exceptions \
	--std=gnu++11 \
	-I. -I./third_party/gemmlowp -I./third_party/flatbuffers/include -I./third_party/ruy -I./third_party/kissfft \
	-o person_detection_test \
	tensorflow/lite/micro/simple_memory_allocator.o tensorflow/lite/micro/memory_helpers.o tensorflow/lite/micro/test_helpers.o tensorflow/lite/micro/recording_micro_allocator.o tensorflow/lite/micro/micro_error_reporter.o tensorflow/lite/micro/micro_time.o tensorflow/lite/micro/quicklogic-arnold/debug_log.o tensorflow/lite/micro/micro_allocator.o tensorflow/lite/micro/micro_string.o tensorflow/lite/micro/micro_profiler.o tensorflow/lite/micro/micro_utils.o tensorflow/lite/micro/recording_simple_memory_allocator.o tensorflow/lite/micro/micro_optional_debug_tools.o tensorflow/lite/micro/all_ops_resolver.o tensorflow/lite/micro/micro_interpreter.o tensorflow/lite/micro/benchmarks/keyword_scrambled_model_data.o tensorflow/lite/micro/kernels/conv.o tensorflow/lite/micro/kernels/mul.o tensorflow/lite/micro/kernels/prelu.o tensorflow/lite/micro/kernels/dequantize.o tensorflow/lite/micro/kernels/ethosu.o tensorflow/lite/micro/kernels/l2norm.o tensorflow/lite/micro/kernels/resize_nearest_neighbor.o tensorflow/lite/micro/kernels/tanh.o tensorflow/lite/micro/kernels/pooling.o tensorflow/lite/micro/kernels/activations.o tensorflow/lite/micro/kernels/logistic.o tensorflow/lite/micro/kernels/ceil.o tensorflow/lite/micro/kernels/arg_min_max.o tensorflow/lite/micro/kernels/reduce.o tensorflow/lite/micro/kernels/sub.o tensorflow/lite/micro/kernels/add.o tensorflow/lite/micro/kernels/softmax.o tensorflow/lite/micro/kernels/pad.o tensorflow/lite/micro/kernels/floor.o tensorflow/lite/micro/kernels/circular_buffer.o tensorflow/lite/micro/kernels/pack.o tensorflow/lite/micro/kernels/svdf.o tensorflow/lite/micro/kernels/hard_swish.o tensorflow/lite/micro/kernels/concatenation.o tensorflow/lite/micro/kernels/unpack.o tensorflow/lite/micro/kernels/neg.o tensorflow/lite/micro/kernels/quantize.o tensorflow/lite/micro/kernels/fully_connected.o tensorflow/lite/micro/kernels/maximum_minimum.o tensorflow/lite/micro/kernels/elementwise.o tensorflow/lite/micro/kernels/strided_slice.o tensorflow/lite/micro/kernels/round.o tensorflow/lite/micro/kernels/split.o tensorflow/lite/micro/kernels/logical.o tensorflow/lite/micro/kernels/comparisons.o tensorflow/lite/micro/kernels/reshape.o tensorflow/lite/micro/kernels/depthwise_conv.o tensorflow/lite/micro/memory_planner/linear_memory_planner.o tensorflow/lite/micro/memory_planner/greedy_memory_planner.o tensorflow/lite/micro/testing/test_conv_model.o tensorflow/lite/c/common.o tensorflow/lite/core/api/error_reporter.o tensorflow/lite/core/api/flatbuffer_conversions.o tensorflow/lite/core/api/op_resolver.o tensorflow/lite/core/api/tensor_utils.o tensorflow/lite/kernels/internal/quantization_util.o tensorflow/lite/kernels/kernel_util.o tensorflow/lite/micro/testing/test_utils.o tensorflow/lite/micro/examples/person_detection/person_detection_test.o tensorflow/lite/micro/tools/make/downloads/person_model_grayscale/no_person_image_data.o tensorflow/lite/micro/tools/make/downloads/person_model_grayscale/person_image_data.o tensorflow/lite/micro/examples/person_detection/model_settings.o tensorflow/lite/micro/tools/make/downloads/person_model_grayscale/person_detect_model_data.o \
	-D__riscv__ \
	-MMD \
	-MP \
	-lbench \
	-nostartfiles \
	-nostdlib \
	-Wl,--gc-sections,--print-map \
	-L/home/gmartin/ETH/TRY5/arnold/pulp-builder/install/rules \
	-T/mnt/hgfs/Documents/ActiveProjects/arnold/chrome/link.ld \
	-L/home/gmartin/ETH/TRY5/arnold/pulp-builder/install/lib/arnold \
	-L/home/gmartin/ETH/TRY5/arnold/pulp-builder/install/lib/arnold/arnold \
	-lrt \
	-lrtio \
	-lrt \
	-lgcc \
	-lm


exit
	
	cd ${DIRECTORY} && \
'/opt/riscv/bin/riscv32-unknown-elf-g++' \
	-std=c++11 \
	-DTF_LITE_STATIC_MEMORY \
	-DNDEBUG \
	-O3 \
	-mfdiv \
	-D__riscv__ \
	-O3 \
	-fdata-sections \
	-ffunction-sections \
	-MMD \
	-MP \
	-DTF_LITE_USE_GLOBAL_CMATH_FUNCTIONS \
	-DTF_LITE_USE_GLOBAL_MAX \
	-DTF_LITE_USE_GLOBAL_MIN \
	-fpermissive \
	-fno-rtti \
	--std=gnu++11 \
	-I. -I./third_party/gemmlowp -I./third_party/flatbuffers/include -I./third_party/ruy -I./third_party/kissfft \
	-o person_detection_test \
	tensorflow/lite/micro/simple_memory_allocator.o tensorflow/lite/micro/memory_helpers.o tensorflow/lite/micro/test_helpers.o tensorflow/lite/micro/recording_micro_allocator.o tensorflow/lite/micro/micro_error_reporter.o tensorflow/lite/micro/micro_time.o tensorflow/lite/micro/quicklogic-arnold/debug_log.o tensorflow/lite/micro/micro_allocator.o tensorflow/lite/micro/micro_string.o tensorflow/lite/micro/micro_profiler.o tensorflow/lite/micro/micro_utils.o tensorflow/lite/micro/recording_simple_memory_allocator.o tensorflow/lite/micro/micro_optional_debug_tools.o tensorflow/lite/micro/all_ops_resolver.o tensorflow/lite/micro/micro_interpreter.o tensorflow/lite/micro/benchmarks/keyword_scrambled_model_data.o tensorflow/lite/micro/kernels/conv.o tensorflow/lite/micro/kernels/mul.o tensorflow/lite/micro/kernels/prelu.o tensorflow/lite/micro/kernels/dequantize.o tensorflow/lite/micro/kernels/ethosu.o tensorflow/lite/micro/kernels/l2norm.o tensorflow/lite/micro/kernels/resize_nearest_neighbor.o tensorflow/lite/micro/kernels/tanh.o tensorflow/lite/micro/kernels/pooling.o tensorflow/lite/micro/kernels/activations.o tensorflow/lite/micro/kernels/logistic.o tensorflow/lite/micro/kernels/ceil.o tensorflow/lite/micro/kernels/arg_min_max.o tensorflow/lite/micro/kernels/reduce.o tensorflow/lite/micro/kernels/sub.o tensorflow/lite/micro/kernels/add.o tensorflow/lite/micro/kernels/softmax.o tensorflow/lite/micro/kernels/pad.o tensorflow/lite/micro/kernels/floor.o tensorflow/lite/micro/kernels/circular_buffer.o tensorflow/lite/micro/kernels/pack.o tensorflow/lite/micro/kernels/svdf.o tensorflow/lite/micro/kernels/hard_swish.o tensorflow/lite/micro/kernels/concatenation.o tensorflow/lite/micro/kernels/unpack.o tensorflow/lite/micro/kernels/neg.o tensorflow/lite/micro/kernels/quantize.o tensorflow/lite/micro/kernels/fully_connected.o tensorflow/lite/micro/kernels/maximum_minimum.o tensorflow/lite/micro/kernels/elementwise.o tensorflow/lite/micro/kernels/strided_slice.o tensorflow/lite/micro/kernels/round.o tensorflow/lite/micro/kernels/split.o tensorflow/lite/micro/kernels/logical.o tensorflow/lite/micro/kernels/comparisons.o tensorflow/lite/micro/kernels/reshape.o tensorflow/lite/micro/kernels/depthwise_conv.o tensorflow/lite/micro/memory_planner/linear_memory_planner.o tensorflow/lite/micro/memory_planner/greedy_memory_planner.o tensorflow/lite/micro/testing/test_conv_model.o tensorflow/lite/c/common.o tensorflow/lite/core/api/error_reporter.o tensorflow/lite/core/api/flatbuffer_conversions.o tensorflow/lite/core/api/op_resolver.o tensorflow/lite/core/api/tensor_utils.o tensorflow/lite/kernels/internal/quantization_util.o tensorflow/lite/kernels/kernel_util.o tensorflow/lite/micro/testing/test_utils.o tensorflow/lite/micro/examples/person_detection/person_detection_test.o tensorflow/lite/micro/tools/make/downloads/person_model_grayscale/no_person_image_data.o tensorflow/lite/micro/tools/make/downloads/person_model_grayscale/person_image_data.o tensorflow/lite/micro/examples/person_detection/model_settings.o tensorflow/lite/micro/tools/make/downloads/person_model_grayscale/person_detect_model_data.o -D__riscv__ -MMD -MP -lbench -nostartfiles -nostdlib -Wl,--gc-sections -L/home/gmartin/ETH/TRY5/arnold/pulp-builder/install/rules -Tarnold/link.ld -L/home/gmartin/ETH/TRY5/arnold/pulp-builder/install/lib/arnold -L/home/gmartin/ETH/TRY5/arnold/pulp-builder/install/lib/arnold/arnold \
	-lrt \
	-lrtio \
	-lrt \
	-lgcc \
	-lstdc++ \
	-lm
