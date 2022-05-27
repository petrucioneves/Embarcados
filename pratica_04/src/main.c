/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  24/04/2022 00:49:00
 *       Revision:  none
 *       Compiler:  arm-none-eabi-gcc
 *
 *         Author:  Petrucio Neves , petrucioneves22@gmail.com
 *
 * =====================================================================================
 */

#include	"hw_types.h"
#include	"soc_AM335x.h"

/*****************************************************************************
 *                 INTERNAL MACRO DEFINITIONS
*****************************************************************************/

#define TIME                                                    1000000
#define TOGGLE                                                  (0x01u)
#define PINPOS(x)(1<<x)

#define CM_PER_GPIO1_CLKCTRL									0xAC
#define CM_PER_GPIO2_CLKCTRL									0xB0
#define CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE   				(0x2u)
#define CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK   			(0x00040000u)
#define CM_PER_GPIO2_CLKCTRL_MODULEMODE_ENABLE   				(0x2u)
#define CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK   			(0x00040000u)



#define CM_conf_gpmc_ben1      	 								0x0878
#define CM_conf_gpmc_a5         								0x0854
#define CM_conf_gpmc_a6         								0x0858
#define CM_conf_gpmc_a7										    0x085C
#define CM_conf_gpmc_a8         								0x0860
#define CM_conf_gpmc_clk										0x088C

#define GPIO_OE                 								0x134
#define GPIO_CLEARDATAOUT       								0x190
#define GPIO_SETDATAOUT         								0x194
#define GPIO_DATAIN												0x138

unsigned int flagBlink;
unsigned int sequencia;
/*****************************************************************************
**                INTERNAL FUNCTION PROTOTYPES
*****************************************************************************/
static void delay();
static void ledInit();
static void ledSeq0();
static void ledSeq1();
static void pushButtom();
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int _main(void){

	flagBlink=0;	//init flag
  	
	/* Configure the green LED control pin. */
  	ledInit();
	
  	while (1){
    	/* Change the state of the green LED. */
		pushButtom();
		delay();
	}

	return(0);
} /* ----------  end of function main  ---------- */


/*FUNCTION*-------------------------------------------------------
*
* Function Name : Delay
* Comments      :
*END*-----------------------------------------------------------*/
static void delay(){
	volatile unsigned int ra;
	for(ra = 0; ra < TIME; ra ++);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledInit
 *  Description:  
 * =====================================================================================
 */
void ledInit( ){
	
	unsigned int val_temp; 	
	/*-----------------------------------------------------------------------------
	 *  configure clock GPIO in clock module
	 *-----------------------------------------------------------------------------*/
	HWREG(SOC_CM_PER_REGS+CM_PER_GPIO1_CLKCTRL) |= CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK | CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE;
	
	/*-----------------------------------------------------------------------------
	 * configure mux pin in control module
	 *-----------------------------------------------------------------------------*/
	
	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_ben1) |= 7;
	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_a5) |= 7;
   	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_a6) |= 7;
   	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_a7) |= 7;
   	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_a8) |= 7;   
	 
	/*-----------------------------------------------------------------------------
	 *  set pin direction 
	 *-----------------------------------------------------------------------------*/
	val_temp = HWREG(SOC_GPIO_1_REGS+GPIO_OE);
	val_temp &= ~PINPOS(21);
	val_temp &= ~PINPOS(22);
	val_temp &= ~PINPOS(23);
	val_temp &= ~PINPOS(24);
	val_temp |= PINPOS(28);
	HWREG(SOC_GPIO_1_REGS+GPIO_OE) = val_temp;
	}/* -----  end of function ledInit  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledSequencia0
 *  Description:  
 * =====================================================================================
 */
void ledSeq0( ){
	HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) |= PINPOS(21);
	delay();
	HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) |= PINPOS(21);
	delay();
	HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) |= PINPOS(22);
	delay();
	HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) |= PINPOS(22);
	delay();
	HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) |= PINPOS(23);
	delay();
	HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) |= PINPOS(23);
	delay();
	HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) |= PINPOS(24);
	delay();
	HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) |= PINPOS(24);
	delay();
}/* -----  end of function ledSequencia0  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledSeq1
 *  Description:  
 * =====================================================================================
 */
void ledSeq1( ){
	HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) |= PINPOS(21) | PINPOS(22) | PINPOS(23)| PINPOS(24);
	delay();
	HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) |= PINPOS(21) | PINPOS(22)| PINPOS(23)|PINPOS(24);
	
}/* -----  end of function ledSeq1  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  pushButtom
 *  Description:  
 * =====================================================================================
 */
void pushButtom( ){
	if(HWREG(SOC_GPIO_1_REGS+GPIO_DATAIN) & PINPOS(28)){ 
		flagBlink^=TOGGLE;
		if(flagBlink==1) ledSeq1();
		else ledSeq0();	
	}else{
		flagBlink=flagBlink;
		if(flagBlink==1) ledSeq1();
		else ledSeq0();	
	}
	
}/* -----  end of function pushButtom  ----- */