
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

#include "expect_fract.h" //"expect.h"
#include "pixel1_fract.h" //"pixel_xxx.h"
#include "bias_fract.h" //"bias_16.h"
#include "filter_fract.h" //"filter_8x16.h"
/*
#include "expect_fract.h"
#include "pixel_xxx.h"
#include "bias_16.h"
#include "filter_8x16.h"
*/

//#include "pixel_base.h"
int spow2 (int exp) {
  int i, ret;
  ret = 1;
  if (exp == 0) return ret;
  else
    for (i = 0; i < exp; ret = ret * 2, i++);
  return ret;
}

int prog_fll(int fll, int mult, int div) {
  int ret = (12 * (mult+1))/spow2(div?div-1:0);
  int i;
  i = 0xC0000000 | 
    (div << 26) | (0x88 << 16) | mult; // 2x RefClock enable FLL
  switch (fll) {
  case 0:
    *(unsigned int*)0x1a100008 = 0x62004107; // select ref clock as input
    *(unsigned int*)0x1a100004 = i;
    printf("FLL0 m=%d, div= %d %08x %08x %08x %08x\n",mult,div,
	   *(unsigned int*)0x1a100000,
	   *(unsigned int*)0x1a100004,
	   *(unsigned int*)0x1a100008,
	   *(unsigned int*)0x1a10000C);
    break;
  case 1:
    *(unsigned int*)0x1a100018 = 0x62004107; // select ref clock as input
    *(unsigned int*)0x1a100014 = i;
    printf("FLL1=1 m=%d, div= %d %08x %08x %08x %08x\n",mult,div,
	   *(unsigned int*)0x1a100010,
	   *(unsigned int*)0x1a100014,
	   *(unsigned int*)0x1a100018,
	   *(unsigned int*)0x1a10001C);
    break;
  case 2:
    *(unsigned int*)0x1a10002C = 0x00808000;
    *(unsigned int*)0x1a100028 = 0x62004100; // select ref clock as input
    *(unsigned int*)0x1a100024 = i;
    printf("FLL2 m=%d, div= %d %08x %08x %08x %08x\n",mult,div,
	   *(unsigned int*)0x1a100020,
	   *(unsigned int*)0x1a100024,
	   *(unsigned int*)0x1a100028,
	   *(unsigned int*)0x1a10002C);
    break;
  }
  return ret;
}

int main()
{
  int i, j, k, elapsed_clocks;
  fpga_typedef *fpga;
  unsigned int result_len, channels, filters;
  int pixel;
  unsigned int width, height;
  signed int f1,f2,f3,f4,f5,f6,f7,f8;
  unsigned int s1,s2,s3,s4,s5,s6,s7,s8;
  int print_on, debug_pixel, skip;
  
  printf("Attempted to program FLL0 %d MHz\n",prog_fll(0,66,2));  // 402 MHz
  printf("Attempted to program FLL1 %d MHz\n",prog_fll(1,14,3));   //44 MHz
  printf("Attempted to program FLL2 %d MHz\n",prog_fll(2,8,2));  // 75/8 MHz
  *(unsigned int*)0x1a100004 = 0x457000c8;
  *(unsigned int*)0x1a100008 = 0x62004107;
  *(unsigned int*)0x1a10000C = 0x00880000;
  
  channels = 8;
  filters = 16;
  width = 48;
  height = 48;
  
  if (filter_base[0] == 0) { // indicator to require scaling/re-organization
    unsigned int zero_point, o, uf;
    int sf;
    skip = 0;
    printf("initializing pixels,filters, bias and results\n");
    zero_point = filter_base[1];
    filter_base[1] = 0;
    o = channels*filters/4;
    printf("Zero_point = %d\n",zero_point);
    for (i = 0; i < filters; i++) {
      for (j = 0; j < channels; j++) {
	uf = (filter_base[o+((i*channels/4) + (j / 4))] >> (24-((j%4)*8))) & 0xff;
	sf = uf - zero_point;
	if (sf > 127) sf = 127;
	else if (sf < -129) sf = -128;
	//	printf("f%02d-c%02d[%02d] - uf = %02x (%3d) sf = %02x (%3d) - ",
	//	       i,j,(o+((i*channels/4) + (j / 4))),uf,uf,(sf&0xff),sf);
	//	printf("-> [%02d] = %08x\n", ((i/8)*2*channels)+j*2+(i/4)%2,
	//	       (unsigned int) (sf&0xff) << (24 - (i%4)*8));
	filter_base[((i/8)*2*channels)+j*2+(i/4)%2] |=  (unsigned int) (sf&0xff) << (24 - (i%4)*8);
	
      }
    }
    for (i = 0; i < filters; i++) {
      int x;
      printf("init Bias[%d] = %d  ",i,bias_base[i]);
      for (j = 0; j < channels; j++) {
	
	x = 128 *
	  (signed char)(filter_base[(i/8)*2*channels + j*2 + (i/4)%2] >>
			(24 - ((i%4) * 8)));
	bias_base[i] += x;
	//     printf("%d ",(signed char)(filter_base[(i/8)*2*channels + j*2 + (i/4)%2] >>
	//				      (24 - ((i%4) * 8)))  );
      }
      bias_base[i] = bias_base[i] & 0xffffff | 0x00000000; // add scaling here
      printf("Adj Bias[%d] = %d\n",i,bias_base[i]);
    }
  }
  else {
    printf("Skipping filter and bias adjustements\n");
    skip = 1;
  }
  //    for (i =0; i < sizeof(filter_base)/4; i++)
  //      printf("%08x %c",filter_base[i],( (i+3)%4 == 3)?'\n':0);
  for (i = 0; i < sizeof(result_base)/4; i++)
    result_base[i] = 0xdeadbeef;
  
  fpga = (fpga_typedef *)0x1a200000;
  
  apb->fpga_clk = 2; // FLL to CLK0 of FPGA
  programFPGA();
  // 2 bits per gpio pad 00=periperal, 01= gpio, 10=efpga
  
  apb->padfunc0 = 0xaaaaaaaa;  // gpio 15-00 from efpga
  apb->padfunc1 = 0xaaaaaaaa;  // gpio 31-16 from efpga
  printf("Address of reset = %x\n", &apb->fpga_reset);
  apb->fpga_reset = 1;
  apb->fpga_reset = 0xF;
  
  apb->fpga_gate  = 0xFFFF;
  fpga->debug_mux = 0x02;

  //   printf("Hello TFL convolution 2D debug mux = %x\n",fpga->debug_mux);
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
    
  printf("Width = %d\n", fpga->width);
  printf("Height = %d\n", fpga->height);
  printf("Channels = %d\n", fpga->channels);
  printf("Filters = %d\n", fpga->filters);
  
  printf("Total_pixels = %d\n", fpga->total_pixels);
  printf("Pixel_base   = 0x%05x (%08x)\n", fpga->pixel_base,pixel_base);
  printf("Filter_base  = 0x%05x (%08x)\n", fpga->filter_base,filter_base);
  printf("Bias_base    = 0x%05x (%08x)\n", fpga->bias_base,bias_base);
  printf("Result_base  = 0x%05x (%08x)\n", fpga->result_base,result_base);
  printf("expect base  = (%08x)\n",expect);  
  /*
  elapsed_clocks = fpga->clocks;

  fpga->control = 2; // control in control
  fpga->int_addr = 0;
  for (i = 0; i < 2048; i++)
    fpga->filter_data = i;
  fpga->int_addr = 0;
  for (i = 0; i < 2048; i++) {
    int data;
    if ((data = fpga->filter_data) != i)
      printf ("Filter ram fail %i = %08x\n",i,data);
  }
  
  printf("f[0-10]\n");
  for (i = 0; i < (filters*channels)/4;) {
    printf("%08x %08x %08x %08x \n",
	   filter_base[i],filter_base[i+1],
	   filter_base[i+2],filter_base[i+3]);
    i = i + 4;
  }
  printf("bias[0-10]\n");
  for (i = 0; i < (filters)+(filters/2);) {
    printf("%08x %08x %08x %08x \n",
	   bias_base[i],bias_base[i+1],
	   bias_base[i+2],bias_base[i+3]);
    i = i + 4;
  }
  printf("pixel[0-10]\n");
  for (i = 0; i < 16; ){
    printf("%02x ",
	   pixel_base[i]);
    i++;
  }
  printf("\n");

  printf("Clock at start = %d\n",elapsed_clocks);
       /*
  for (i = 0 ; i < 10; i++)
    printf ("pre Result[%4d] = 0x%08x\n",i,result_base[i]);

  
  printf("f[0-10]");
  for (i=0; i < 10; i++)
    printf(" %x",filter_base[i]);
       */  
  fpga->control = 1;  // start 2dconv
  printf("\n");
  
  while (fpga->control & 0x1) { } // wait till start is cleared
  printf ("Elapsed Clocks = %d\n",fpga->clocks - elapsed_clocks);
  fpga->control = 2; // control in control
  fpga->int_addr = 0;
  //  for (i=0; i < 0x100; i++)
  //    printf("%2x-%8x%c",i,fpga->filter_data,i%4?' ':'\n');



  /*
  fpga->int_addr = 0;
    for (i = 0 ; i < 10; i++)
      printf ("oper1[%4d] = 0x%08x exp = 0x%08x\n",i,fpga->pixel_data,pixel_base[i]);
  fpga->int_addr = 0;
    for (i = 0 ; i < 32; i++)
      printf ("coef[%4d] = 0x%08x exp = 0x%08x\n",i,fpga->filter_data,filter_base[i]);
  fpga->int_addr = 0x200;
    for (i = 0 ; i < 10; i++)
      printf ("bias[%4d] = 0x%08x exp = 0x%08x\n",i,fpga->bias_data,bias_base[i]);

  
  */
  print_on = 0;
  debug_pixel =100000; // change to a lower number to see intermmediate calcs
  if (skip == 0) {
    for (i = 0; i < (width*height); i++) { // for each input pixel
      for (j = 0; j < filters/8; j++) {  // for each group of 8 filters
	f1 = bias_base[j*8] & 0x00800000 ? 0xff000000 | bias_base[j*8] : bias_base[j*8]&0x00ffffff ;    // load the bias and scalers
	s1 = bias_base[j*8] >> 24;
	f2 = bias_base[j*8+1] & 0x00800000 ? 0xff000000 | bias_base[j*8+1] : bias_base[j*8+1]&0x00ffffff ;   
	s2 = bias_base[j*8+1] >> 24;
	f3 =  bias_base[j*8+2] & 0x00800000 ? 0xff000000 | bias_base[j*8+2] : bias_base[j*8+2]&0x00ffffff ;   
	s3 = bias_base[j*8+2] >> 24;
	f4 =  bias_base[j*8+3] & 0x00800000 ? 0xff000000 | bias_base[j*8+3] : bias_base[j*8+3] &0x00ffffff;   
	s4 = bias_base[j*8+3] >> 24;
	f5 =  bias_base[j*8+4] & 0x00800000 ? 0xff000000 | bias_base[j*8+4] : bias_base[j*8+4]&0x00ffffff ;   
	s5 = bias_base[j*8+4] >> 24;
	f6 =  bias_base[j*8+5] & 0x00800000 ? 0xff000000 | bias_base[j*8+5] : bias_base[j*8+5]&0x00ffffff ;   
	s6 = bias_base[j*8+5] >> 24;
	f7 =  bias_base[j*8+6] & 0x00800000 ? 0xff000000 | bias_base[j*8+6] : bias_base[j*8+6]&0x00ffffff ;   
	s7 = bias_base[j*8+6] >> 24;
	f8 = bias_base[j*8+7] & 0x00800000 ? 0xff000000 | bias_base[j*8+7] : bias_base[j*8+7]&0x00ffffff ;   
	
	s8 = bias_base[j*8+7] >> 24;
	
	if (print_on || ((i*(filters/4) + j*2) >= debug_pixel)) {
	  printf("bias[%x] = %06x %06x %06x %06x",j*8,f1,f2,f3,f4);
	  printf(" %06x %06x %06x %06x",f5,f6,f7,f8);
	  printf("s[%x] = %02x %02x %02x %02x",j*8,s1,s2,s3,s4);
	  printf(" %02x %02x %02x %02x\n",s5,s6,s7,s8);
	}
	for (k = 0; k < channels;  k++) {  // for each channel in a filter
	  pixel = (signed char)((pixel_base[(i*channels)/4 + (k/4)] >> (24-((k%4)*8))) & 0xff);
	  if (print_on || ((i*(filters/4) + j*2) >= debug_pixel))
	    printf("Pixel[0x%03x] = %x f=%03x ch=%08x-%08x ",i,pixel,
		   (j*channels*2)+k*2,
		   filter_base[(j*channels*2)+k*2],filter_base[(j*channels*2)+k*2+1] ); 
	  f1 += pixel*(signed char)((filter_base[(j*channels*2)+k*2] >> 24) & 0xff);
	  f2 += pixel*(signed char)((filter_base[(j*channels*2)+k*2] >> 16) & 0xff);
	  f3 += pixel*(signed char)((filter_base[(j*channels*2)+k*2] >> 8) & 0xff);
	  f4 += pixel*(signed char)((filter_base[(j*channels*2)+k*2] >> 0) & 0xff);
	  f5 += pixel*(signed char)((filter_base[(j*channels*2)+k*2+1] >> 24) & 0xff);
	  f6 += pixel*(signed char)((filter_base[(j*channels*2)+k*2+1] >> 16) & 0xff);
	  f7 += pixel*(signed char)((filter_base[(j*channels*2)+k*2+1] >> 8) & 0xff);
	  f8 += pixel*(signed char)((filter_base[(j*channels*2)+k*2+1] >> 0) & 0xff);
	  if (print_on || ((i*(filters/4) + j*2) >= debug_pixel)) {
	    
	    printf("i,j,k = %x %x %x - %05x %05x %05x %05x %05x %05x %05x %05x \n",i,j,k,f1,f2,f3,f4,f5,f6,f7,f8);
	  }
	}
	//       printf("expect[%d] =%06x %06x %06x %06x %06x %06x %06x %06x \n",
	//            	      i*8,f1,f2,f3,f4,f5,f6,f7,f8);
	//       printf("scale      =%06d %06d %06d %06d %06d %06d %06d %06d \n",
	//             	     s1,s2,s3,s4,s5,s6,s7,s8);
	expect[i*(filters/4) + j*2] = (((f1 >> (s1)) & 0xff) << 24)|
	  (((f2 >> (s2)) & 0xff) << 16) |
	  (((f3 >> (s3)) & 0xff) << 8) |
	  ((f4 >> (s4)) & 0xff) ;
	expect[i*(filters/4) + j*2+1] = (((f5 >> (s5)) & 0xff) << 24) |
	  (((f6 >> (s6)) & 0xff) << 16) |
	  (((f7 >> (s7)) & 0xff) << 8) |
	  ((f8 >> (s8)) & 0xff) ;
	if (print_on|| ((i*(filters/4) + j*2) >= debug_pixel)) //debug_pixel))
	  printf("expect[%4d] = 0x%08x - 0x%08x, i,j,k = %x %x %x\n",
		 i*(filters/4) + j*2 ,
		 expect[i*(filters/4) + j*2], expect[i*(filters/4) + j*2 +1],i,j,k);
	print_on = 0;
	
      }
    }
  }
  
 
  for (i = 0 ; i < result_len ; i++) {
    if (result_base[i] != expect[i])
      printf ("post Result[0x%4x] = 0x%08x exp = 0x%08x\n",
	      i,result_base[i],expect[i]);
  }
  /*
  fpga->control = 2; // control in control
  fpga->int_addr = 0;
    for (i = 0 ; i < 10; i++)
      printf ("oper1[%4d] = 0x%08x exp = 0x%08x\n",i,fpga->pixel_data,pixel_base[i]);
  fpga->int_addr = 0;
    for (i = 0 ; i < 32; i++)
      printf ("coef[%4d] = 0x%08x exp = 0x%08x\n",i,fpga->filter_data,filter_base[i]);
  fpga->int_addr = 0x200;
    for (i = 0 ; i < 10; i++)
      printf ("bias[%4d] = 0x%08x exp = 0x%08x\n",i,fpga->bias_data,bias_base[i]);
  */

  return 0;
}

	 
