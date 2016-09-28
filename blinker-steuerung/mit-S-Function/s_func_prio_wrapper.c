

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
 */
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
void REWB(real_T  *in, real_T *out);
void REBH(real_T  *in, real_T *out);

void s_func_prio_Outputs_wrapper(const real_T *WB,
			const real_T *BH,
			real_T *Blink_prio)
{    
    static real_T Prio=0; // 0=WB, 1=BH
    real_T BHedge=0, WBedge=0;
    
    REWB(WB,&WBedge);
    REBH(BH,&BHedge);
    
    if(Prio==0){
        if(WBedge==1){Prio=1;}
        else if(*BH==1){*Blink_prio=LINKS;}
        else if(*BH==2){*Blink_prio=RECHTS;}
        else if(*WB==1){*Blink_prio=BEIDE;}
        else{*Blink_prio=AUS;}
    }
    else{
        if(BHedge==1){Prio=0;}
        else if(*WB==1){*Blink_prio=BEIDE;}
        else if(*BH==1){*Blink_prio=LINKS;}
        else if(*BH==2){*Blink_prio=RECHTS;}
        else{*Blink_prio=AUS;}
    }
      
     
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

void REWB(real_T  *in, real_T *out){
        static int WBaltIn = 0;
        
        if (*in > WBaltIn){
            *out  = 1;
        }
        else { *out = 0;}
        
        WBaltIn = *in;
}

void REBH(real_T  *in, real_T *out){
        static int BHaltIn = 0;
        
        if (*in > BHaltIn){
            *out  = 1;
        }
        else { *out = 0;}
        
        BHaltIn = *in;
}  