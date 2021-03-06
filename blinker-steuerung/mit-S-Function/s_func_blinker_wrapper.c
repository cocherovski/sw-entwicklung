

/*
 * Include Files
 *
 */
#if defined(MATLAB_MEX_FILE)
#include "tmwtypes.h"
#include "simstruc_types.h"
#else
#include "rtwtypes.h"
#endif

/* %%%-SFUNWIZ_wrapper_includes_Changes_BEGIN --- EDIT HERE TO _END */
#include <math.h>
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define u_width 1
#define y_width 1
/*
 * Create external references here.
 *
 *
 */
#include<mex.h>
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
/* extern double func(double a); */
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */
#define AUS 0
#define LINKS 1
#define RECHTS 2
#define BEIDE 3

/*
 * Output functions
 *
 */

    int counter1ms, counter100ms;
    real_T ANZEIT;
    
void s_func_blinker_Outputs_wrapper(const real_T *Blink_prio,
			const real_T *PWM,
			real_T *VL,
			real_T *HL,
			real_T *VR,
			real_T *HR)
{
    
    if(*Blink_prio==AUS){
            *VL=0; *VR=0; *HL=0; *HR=0;
            counter1ms=0; counter100ms=0;
    }
    else if(*Blink_prio==LINKS){
            ANZEIT=*PWM;
            counter1ms++;
            counter1ms=counter1ms%100;
            if(counter1ms==0){
                ANZEIT=*PWM;
                if(*Blink_prio==1){
                    counter100ms++;
                }
                else if(*Blink_prio==2){
                    counter100ms=0;
                }
                else if(*Blink_prio==3){
                    counter100ms=0;
                }
                else{;}
            }
            else if((counter1ms<=ANZEIT)&&((counter100ms%10)<5)){
                *VL=1; *HL=1; *VR=0; *HR=0;
            }
            else{
                *VL=0; *VR=0; *HL=0; *HR=0;
            }
    }
    else if(*Blink_prio==RECHTS){
            ANZEIT=*PWM;            
            counter1ms++;
            counter1ms=counter1ms%100;
            if(counter1ms==0){
                ANZEIT=*PWM;
                if(*Blink_prio==2){
                    counter100ms++;
                }
                else if(*Blink_prio==3){
                    counter100ms=0;
                }
                else if(*Blink_prio==1){
                    counter100ms=0;
                }
                else{;}
            }
            else if((counter1ms<=ANZEIT)&&((counter100ms%10)<5)){
                *VL=0; *HL=0; *VR=1; *HR=1;
            }
            else{
                *VL=0; *VR=0; *HL=0; *HR=0;
            }
    }
    else if(*Blink_prio==BEIDE){
            ANZEIT=*PWM;            
            counter1ms++;
            counter1ms=counter1ms%100;
            if(counter1ms==0){
                ANZEIT=*PWM;
                if(*Blink_prio==3){
                    counter100ms++;
                }
                else if(*Blink_prio==1){
                    counter100ms=0;
                }
                else if(*Blink_prio==2){
                    counter100ms=0;
                }
                else{;}
            }
            else if((counter1ms<=ANZEIT)&&((counter100ms%10)<5)){
                *VL=1; *HL=1; *VR=1; *HR=1;
            }
            else{
                *VL=0; *VR=0; *HL=0; *HR=0;
            }
    }
    else{;}
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
/* This sample sets the output equal to the input
      y0[0] = u0[0]; 
 For complex signals use: y0[0].re = u0[0].re; 
      y0[0].im = u0[0].im;
      y1[0].re = u1[0].re;
      y1[0].im = u1[0].im;
*/
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}
