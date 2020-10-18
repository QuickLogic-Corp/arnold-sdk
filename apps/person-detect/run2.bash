PRJ_ROOT=../tf-arnold/tensorflow/lite/micro/tools/make/gen/quicklogic-arnold_rv32imfcxpulpv2/prj
BUILD_DIR=~/ETH/TRY5/arnold/pulp-builder/pulp-rt-examples/arnold_tests/new_example/build
TFLM_DIR=../tf-arnold/tensorflow/lite/micro
EXAMPLE=person_detection_test

touch ../tf-arnold/tensorflow/lite/micro/quicklogic-arnold/person_detection_test.cc
touch ../tf-arnold/tensorflow/lite/micro/kernels/pooling.cc
make ${PRJ_ROOT}/${EXAMPLE}/make/${EXAMPLE}
