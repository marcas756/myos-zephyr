#include "mandelbrot.h"
#include "fxp16.h"

#define XRES 80
#define YRES 40
#define MAGNIFY 1

#define FP_0_7      0x00b3
#define FP_0_5      0x0080
#define FP_3_0      0x0300
#define FP_2_0      0x0200
#define FP_100_0    0x6400

#define FP_FRAC     8
    
PROCESS(mandelbrot,mandelbrot);
PROCESS_THREAD(mandelbrot)
{
    static fxp16_t   x,tmp1,tmp2,y,cx,cy;
    static uint8_t  hx,hy,iteration;
    static uint8_t itermax;                 /* how many iterations to do	*/
    
    const char *cols = "`^\",:;Il!i~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
    
    PROCESS_BEGIN();

    itermax = strlen(cols)-1;

    for(;;)
    {
        for (hy=0; hy < YRES; hy++)  
        {
            for (hx=0; hx < XRES; hx++)  
            {
                //cx = (((float)hx)/((float)hxres)-0.5)/magnify*3.0-0.7;
                tmp1 = fxp16_int2fp(hx,FP_FRAC);
                tmp2 = fxp16_int2fp(XRES,FP_FRAC);
                cx = fxp16_div(tmp1,FP_FRAC,tmp2,FP_FRAC);
                cx = fxp16_sub(cx,FP_0_5);
                tmp1 = fxp16_int2fp(MAGNIFY,FP_FRAC);
                cx = fxp16_div(cx,FP_FRAC,tmp1,FP_FRAC);
                cx = fxp16_mult(cx,FP_FRAC,FP_3_0,FP_FRAC);
                cx = fxp16_sub(cx,FP_0_7);


                
                
                //cy = (((float)hy)/((float)hyres)-0.5)/magnify*3.0;                  
                tmp1 = fxp16_int2fp(hy,FP_FRAC);
                tmp2 = fxp16_int2fp(YRES,FP_FRAC);
                cy = fxp16_div(tmp1,FP_FRAC,tmp2,FP_FRAC);
                cy = fxp16_sub(cy,FP_0_5);
                tmp1 = fxp16_int2fp(MAGNIFY,FP_FRAC);
                cy = fxp16_div(cy,FP_FRAC,tmp1,FP_FRAC);
                cy = fxp16_mult(cy,FP_FRAC,FP_3_0,FP_FRAC);
     
                
                x = y = 0;              

                for (iteration = 0;iteration < itermax; iteration++)  
                {      
                    // xx = x*x-y*y+cx;
                    tmp1 = fxp16_mult(x,FP_FRAC,x,FP_FRAC);
                    tmp2 = fxp16_mult(y,FP_FRAC,y,FP_FRAC);
                    tmp1 = fxp16_sub(tmp1,tmp2);                       
                    tmp1 = fxp16_add(tmp1,cx);

                    
                    // y = 2.0*x*y+cy;
                    y = fxp16_mult(x,FP_FRAC,y,FP_FRAC);
                    y = fxp16_mult(FP_2_0,FP_FRAC,y,FP_FRAC);
                    y = fxp16_add(y,cy);
                    
                    x = tmp1;

                    
                    tmp1 = fxp16_mult(x,FP_FRAC,x,FP_FRAC);
                    tmp2 = fxp16_mult(y,FP_FRAC,y,FP_FRAC);
                    
                    
                    
                    if (fxp16_add(tmp1,tmp2) > FP_100_0)
                    {
                        break;    // check if this works with protothreads!              
                    }
                    
                    PROCESS_YIELD();
                    
                }
                
                
                putchar(cols[iteration]);                
            }


            putchar('\n');
        }
    }   
    
    PROCESS_END();
}