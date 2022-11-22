#include "game.h"
#include "segmentlcd.h"
#include "segmentlcd_individual.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "em_device.h"
#include "em_chip.h"
#include <em_gpio.h>
#include <em_cmu.h>
#include "bsp.h"
#include "bsp_trace.h"

int main(void){
	/*Chip errata*/
		CHIP_Init();
	/*LCD*/
		SegmentLCD_Init(false);
	/*Gecko symbol :) */
		SegmentLCD_Symbol(LCD_SYMBOL_GECKO,1);
	/*Speed setup*/
		CMU_HFRCOBandSet(cmuHFRCOBand_1MHz);
	/*Setup Systick timer for 1msec ITd	*/
		if(SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE)/1000)){
			while(1);
		}
	/*Buttons*/
		BSP_ButtonsInit();
	/*Starting screen*/
		SegmentLCD_Write("BME-MIT");
		Delay(1000);
		SegmentLCD_Write("BAMBI");
		Delay(1000);
		SegmentLCD_Write("BANANA");
		Delay(1000);
		SegmentLCD_Write(" GAME");
		Delay(1000);
		SegmentLCD_Write("PRSSBTN");
	/*Start the game*/
		for(;;){
			gameInit();
		}

}

