# tensorflow lite micro person_detection example

## Background
This app started with the TfLu person_detection_test, which uses canned image data, and adapted it to run on Arnold
and use the eFPGA in Arnold to accelerate many conv2d operations.  We plan to add depthwiseconv2d and fuse that with the conv2d.

## Structure
The top-level structure of the arnold-sdk, after installation is:
~~~
arnold-sdk
├── LICENSE
├── README.md
├── apps
│   └── gpio-toggle
├── install.bash
├── install.log
├── patches
│   └── 
├── pulp-builder
│   └── 
├── run-hello.bash
├── setup-pulp.bash
├── tensorflow
│   └── 
└── tflu-apps
    ├── hw-standalone
    └── person-detection
~~~
The vast majority of the TfLu code is unchanged.  Following the TfLu conventions, the routines that are changed are collected in the arnold target directory
~~~
arnold-sdk/tensorflow/tensorflow/lite/micro/quicklogic-arnold
├── README.md
├── accel.h
├── apb_conv2d.h
├── apb_fcb.h
├── arnold_apb_ctl.h
├── conv-simd.cc
├── conv.cc
├── debug_log.cc
├── fc_config.h
├── fll.cc
├── fll.h
├── gpio.cc
├── gpio.h
├── micro_interpreter.cc
├── micro_interpreter.h
├── person_detection_test.cc
├── programFPGA.cc
├── programFPGA.h
├── rt_conf.c
└── rt_pad_conf.c
~~~
The details of what has been changed are covered further down in this document.

## making and running person_detection
The make file in arnold-sdk/tflu-apps/person-detection has three main steps:
1. Use TfLu make to 'generate' the person_detection project.  This process pulls in the necessary data (flatfiles and images) for the person-detection application,
and creates a merged and pruned tensorflow tree located at 
~~~
arnold-sdk/tensorflow/tensorflow/lite/micro/tools/make/gen/quicklogic-arnold_rv32imfcxpulpv2/prj/person_detection_test
~~~
>The merging uses the files in the quicklogic-arnold directory to replace standard tensorflow files of the same name,
and the pruning removes any files that are not required.
>The generation process also creates an Arnold specific makefile using
~~~
arnold-sdk/tensorflow/tensorflow/lite/micro/tools/make/targets/quicklogic-arnold_makefile.inc
~~~

2. Compile and link for Arnold using the RI5CY tool chain and Arnold run-time routines. The linker script is located at:
~~~
arnold-sdk/tensorflow/tensorflow/lite/micro/tools/make/targets/quicklogic-arnold/arnold.ld
~~~
>Since the generated project is unaware of changes to source files located in the quicklogic-arnold directory, prior to compilation certain ofthe sources in
quicklogc-arnold are copied into the appropriate place in the generated project.  This is done to reduce cycle time during experiments, however, since the copies
were introduced manually they are not necessarily comprehensive.  The safest action is always to do a 'make clean; make' which will regenerate the entire project.

3. Load and run on Arnold

## Description of code
The basic framework of TfLu is a list of nodes each of which describes an operation and the data (tensor) required for that operation.
The standard tensorflow implemenation uses 32 bit integers for calcualtions, although the activations and filters are expressed as 8 bit values.
As far as I can tell, there is a mechanism that takes higher resolution tensors from tensorflow lite, quantizes them and places them in an array that
is included during compile time. At start up the main routine, in person_detection.cc, calls
~~~
 interpreter.AllocateTensors();
~~~
which processes the flat datastructure and creates tensor structures that point to the appropriate offsets in the static data, and work out where the
dynamic data should be places in teh tensor arena.
### Modifying the data (micro_interpreter.cc)
The way the data is organized for TfLu is suitable for software, but not suitable for accleration.
Arnold has an Arnold specific verion of micro_interpreter.cc that add a new routine, Accel_PrepareForAccel, at the end of MicroInterpreter::AllocateTensors().
PrepareForAccel performs the following steps:
1. Identifies the largest RW tensor for use as temporary space during data reformatting
2. Loops through all nodes doing the following: [__ideally__ these steps would be done by flatbuf generator prior to compilation]
	- Determine if the node matches the acceleration template -- conv2d, 1x1, number of filters is a multiple of 8
	- If it does, hacks *node->custom_initial_data_size* to indicate that this node should get special treatment in conv.cc
	- Converts filter data (coefficients) to true int8_t
		- The filter data is uint8_t with a zero point.  In the s/w version the zero point is subtracted from the filter coefficient to get an int32_t.
		Empirically the resulting value is very nearly an int8_t, however there are a few values that are slightly larger or smaller than a legal int8_t.
		These values are clamped to be true int8_t.  I suspect that the intent was to make the values true int8_t, and that there is some defct in the mapping
		from TfL to TfLu.  Also note that best practice would be train on the int8_t model, not on a higher precison model and then quantize.
	- The software routines multiply the uint8_t activations by the int8_t filter coefficients.  The Arnold multipliers require both operands to be either
	signed or unsigned and do not support one signed and one unsigned. The accelerator therefore converts all activations to int8_t by subtracting 128 from
	all activations.  This obvioulsy affects the result of the dot product, however, since the filter coefficients are staic and known, it is possible to calculate
	a correction factor.  And since the TfL algorithms already specify an output bias, the correction factor can be added to the bias.
	- The software has an output zero point adjustment that is performed after the scaling for quantization.  This is adjust appropriately and added to the bias.
	- The software uses an 32bit coefficient plus a shift (floating point) to scale prior to quantization.  Since the acclerator can do more 16 bit calculations
	than 32bit, the code scales the 32bit coeff and shift to fit a 16b calculation that is convenient for the accelerator. (I tried going to 8 bit calcualtions, but the results
	were not close to the 'correct' results.  I did not expect that so I wonder if training was done with a true 8 bit model the results would be better.)
	- The final step is to reorder the data to conform to an 8 channel SIMD structure.

### Modifying which convolution to call (conv.cc)
Near the end of conv.cc is the defintion of *Eval(TfLiteContext* context, TfLiteNode* node)*.  This routine examines the data characteristics to determine which
convolution implementation to call.  This has been modifed for the case of uint8_t data to call either:
1. the standard software kernel
2. a software implementation of the acclerator kernel that models the SIMD behavior and uses the correct precision arithmetic as well as working from the
accelerator formatted data structures.  This version served as a fast iteration test bed for developing and confirm the various transformations required to fit the eFPGA hardware,
as well as printed the data required for simualtion test benches.
3. the wrapper for the eFPGA accelerator

The relevant code fragment is:
~~~
case kTfLiteUInt8:
	// printf("node->custom_initial_data_size=%d\n", node->custom_initial_data_size);
      if (node->custom_initial_data_size == 0) {
        EvalQuantized(context, node, params, data, input, filter, bias, nullptr,
                      nullptr, output);
      } else if (node->custom_initial_data_size & accel_sw) {
        EvalQuantizedSW_SIMD(context, node, params, data, input, filter, bias, nullptr,
                      nullptr, output, (node->custom_initial_data_size & accel_print) ? true : false);
      } else if (node->custom_initial_data_size & accel_fpga) {
        EvalQuantizedFPGA_SIMD(context, node, params, data, input, filter, bias, nullptr,
                      nullptr, output, (node->custom_initial_data_size & accel_print) ? true : false);
      } else {
        printf("ERROR: Unknown accel type = 0x%x\n", node->custom_initial_data_size);
      }
~~~

### Implementations of the acclerated convolutions (conv-simd.cc)
A new file, conv-simd.cc, that has two routines, *ConvSW_SIMD()* and *ConvFPGA_SIMD()*.  The SW version is the tst bed for development and verification.
The FPGA version will call the accelerator.