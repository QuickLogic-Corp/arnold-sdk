
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.



#include <stdio.h>
#include "pulp.h"
#include "programFPGA.h"
#include "arnold_apb_ctl.h"
#include "apb_conv2d.h"


#include "result.h"
#include "data.h"
#include "fll.h"



int main()
{
  int i, j, k, elapsed_clocks;
  fpga_typedef *fpga;
  unsigned int result_len; 
  int pixel;
  signed int f1,f2,f3,f4,f5,f6,f7,f8;
  unsigned int s1,s2,s3,s4,s5,s6,s7,s8;
  int print_on, debug_pixel, skip;

  print_on = 0;
  skip = 0;
  k = 9155;
  prog_fll(0,20000,2);
  prog_fll(0,28000,2);  // 456
  prog_fll(1,6100,3);   //50 MHz
  prog_fll(2,k,3);  // 9155 = 75MHz
  dump_fll(0);
  dump_fll(1);
  dump_fll(2);

    //  rt_freq_set(2,50*1000*1000);

  printf("\n***Test data from %s\n\n",testcase);



  
  for (i = 0; i < sizeof(result_base)/4; i++)
    result_base[i] = 0xdeadbeef;
  static unsigned int *tmp ;
  if (print_on) {
    printf("Pixels:\n");
    tmp = (unsigned int*)pixel_base;
    for (i = 0; i < 16; i++) {
      printf("%08x %c",tmp[i],(i+1) % 4 ? 0 : 0xA);
    }
    printf("Filters:\n");
    tmp = (unsigned int*)filter_base;
    for (i = 0; i < 16; i++) {
      printf("%08x %c",tmp[i],(i+1) % 4 ? 0 : 0xA);
    }
    printf("Bias:\n");
    tmp = (unsigned int*)bias_base;
    for (i = 0; i < sizeof(bias_base)/4; i++) {
      printf("%08x %c",tmp[i],(i+1) % 4 ? 0 : 0xA);
    }
    printf("Expect:\n");
    tmp = (unsigned int*)expect;
    for (i = 0; i < 16; i++) {
      printf("%08x %c",tmp[i],(i+1) % 4 ? 0 : 0xA);
    }
  }

  
  fpga = (fpga_typedef *)0x1a200000;
  
  apb->fpga_clk = 2; // FLL to CLK0 of FPGA
  programFPGA();
  
  apb->padfunc0 = 0xaaa82aaa;  // gpio 15-00 from efpga .. uart
  apb->padfunc1 = 0xaaaaaaaa;  // gpio 31-16 from efpga

  apb->fpga_reset = 0;
  apb->fpga_reset = 0xF;
  
  apb->fpga_gate  = 0xFFFF;
  fpga->debug_mux = 0x42;

  printf("****Debug Mux = %x\n", fpga->debug_mux);
	 
  result_len = (filters * width * height)/4;
  fpga->width = width;
  fpga->height = height;
  fpga->channels = channels;
  fpga->filters = filters;
  fpga->total_pixels = fpga->width * fpga->height;
  
  fpga->pixel_base = pixel_base;
  fpga->filter_base = filter_base;
  fpga->bias_base = bias_base;
  fpga->result_base = result_base;
  fpga->quant = quant;
  
  printf("Width = %d\n", fpga->width);
  printf("Height = %d\n", fpga->height);
  printf("Channels = %d\n", fpga->channels);
  printf("Filters = %d\n", fpga->filters);
  printf("Quant = %x\n", fpga->quant);
    
  printf("Total_pixels = %d\n", fpga->total_pixels);
  printf("Pixel_base   = 0x%05x (%08x)\n", fpga->pixel_base,pixel_base);
  printf("Filter_base  = 0x%05x (%08x)\n", fpga->filter_base,filter_base);
  printf("Bias_base    = 0x%05x (%08x)\n", fpga->bias_base,bias_base);
  printf("Result_base  = 0x%05x (%08x)\n", fpga->result_base,result_base);
  printf("Expect       = (%08x)\n",expect);  


  fpga->control = 1;  // start 2dconv
    
  while (fpga->control & 0x1) { } // wait till start is cleared

  printf ("Elapsed Clocks = %d \n",fpga->clocks );
  j = 0;
  tmp = (unsigned int*)expect;
  for (i = 0; i < result_len ; i++) {
    if (result_base[i] != tmp[i]) {
      j = j + 1;
      if (print_on) {
	printf ("post Result[0x%4x] = 0x%08x exp = 0x%08x\n",
		i,result_base[i],tmp[i]);
      }
    }
  }

  if (j == 0)
    printf("****All data passed!\n");
  else
    printf("**** %d mismatches detected -- Test Failed\n",j);

  if (skip) {
    while (j == 0) {
      k += 122;  // Plus 1 MHz
      printf("Attempted to program FLL2 %d Hz\n",prog_fll(2,k,3)); 
//	rt_freq_set(2,k*1000*1000);
      printf (" Running again\n");

  
      fpga->control = 1;  // start 2dconv
    
      while (fpga->control & 0x1) { } // wait till start is cleared

      printf ("Elapsed Clocks = %d \n",fpga->clocks );


      j = 0;
      tmp = expect;
      for (i = 0; i < result_len ; i++) {
	if (result_base[i] != tmp[i]) {
	  j = j + 1;
	  if (print_on) {
	    printf ("post Result[0x%4x] = 0x%08x exp = 0x%08x\n",
		    i,result_base[i],tmp[i]);
	  }
	}
      }

      if (j == 0)
	printf("****All data passed!\n");
      else
	printf("**** %d mismatches detected -- Test Failed\n",j);
    }
  }


  return 0;
}

	 
