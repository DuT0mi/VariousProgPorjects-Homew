#ifndef GAME_H
#define GAME_H
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
/*	Created by:
 * 		TobiDani
 * 		DuTomi
 *
*/
/*Game variables*/
	typedef enum b {
			START_EDGE,
			SECOND,
			THIRD,
			END_EDGE
		}basketPos;
	typedef enum d {
	EASY,
	INTERMEDIATE,
	HARD,
	LEGENDARY
}diffLvl;
/*Functions' declarations*/
	/* @brief:
	 * Changing the basket's direction to the right
	*/
	void basketMoveToR();
	/* @brief:
	 * Shifting the basket with one segment to the left
	*/
	void basketMoveToL();
	/* @brief:
	 * Choose the basket's direction
	*/
	void diffController();
	/* @brief:
	 * Initialize the game
	*/
	void gameInit();
	/* @brief:
	 * Makes a decision from buttons, how where the player like'd to go with the basket
	*/
	void chooseBasketDir();
	/* @brief:
	 * Similar as Delay(), just for the game and not for the system
	*/
	void gameOnInterrupt(uint32_t);
	/* @brief:
	 * It has a responsible about manage the game
	*/
	bool gameOnGoing();
	/* @brief:
	 * Setting the start value for constants(more elegant)
	*/
	void setConstants();
	/* @brief:
	 * "Speaking" with the player
	*/
	void startTheGame();
	/* @brief:
	 * Makes latency
	*/
	void Delay(uint32_t);
/*  @brief:
 * 	Interrupt handler function (weak type), which is increasing the msTicks variable with one in every second
*/
	void SysTick_Handler(void);
#endif
