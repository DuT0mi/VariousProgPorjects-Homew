#include "game.h"
/*Constants*/
	#define ZERO 0;								// irritated our eyes the  0, so we changed it to ZERO
	static int playedGames = 0;					// counting the played games
	static time_t seeder;						// for seeding
	static time_t seederCounter = 0;			// for seeding
	const uint8_t defaultLatency = 15;			// [ms]
	const uint8_t defaultFallTimeChange = 25;	// banana falling time
	const uint8_t maxNumOfBananas = 25;			// uint8 0..255 its enough
/*Game variables*/
	bool isGameStarted = false;					// game variable
	uint8_t currNumOfBananas;					// banana counter
	uint8_t	difficultyLvl = EASY;				// difficulty levels, must have starter value (if just pressed the PB1, without pressing PB0)
	uint8_t basketCurrDir;						// direction of basket
	uint16_t timeOfFalling = 500;				// [ms], the default value is: 500 ms (half second)
	volatile uint32_t msTicks;					// counter of passed milliseconds
/*	LCD's segment controlling
 * 	from: 	segmentlcd_individual.(h/c) by: Naszaly Gabor
 */
	SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS];
	SegmentLCD_UpperCharSegments_TypeDef upperCharSegments[SEGMENT_LCD_NUM_OF_UPPER_CHARS];
/*Implementations*/
	void SysTick_Handler(void){
		msTicks++;
	}
	void gameInit(){
		//	If PB0 is pressed then adjust the difficulty level
			if(!BSP_ButtonGet(0)){
				diffController();
				Delay(200);				// for avoid flickering
			}if(!BSP_ButtonGet(1)){
				// The game starts
				if(!isGameStarted){
					startTheGame();
					isGameStarted = gameOnGoing();
				}
			}
	}
	bool gameOnGoing(){
		// variables for the game
			// clearing the previous values
			setConstants();
			//for making real random numbers (depends on time), also the first game should be different any times we start the device
			(playedGames == 0)?	srand(abs((unsigned)(seeder + msTicks)))
								:
								srand(abs((unsigned)seeder));
		// others
			// for counting the missed bananas
				uint16_t j = 0;
			// for counting the caught bananas (right top of the screen)
				uint8_t i = 0;
		// Game's logic
			while(currNumOfBananas != maxNumOfBananas){
				// we are (or the user) playing on 4 segments so,
				// we must to create the random numbers (bananas) spawn on these 4 segments
					uint8_t randBanana = rand() % 4;
				// turn on the random_th segment
					lowerCharSegments[randBanana].a = 1;
					// Now we working on that segment with timeOfFalling time-steps
						SegmentLCD_LowerSegments(lowerCharSegments);
						// We have to move a basket BUT,
						// we can think about as an "IT"/ input (handling)
						// and also add some delay which the human eye can't see
							gameOnInterrupt(timeOfFalling/6);
						// getting (reading) the basket's directions
							chooseBasketDir();
							Delay(timeOfFalling/5);
							gameOnInterrupt(timeOfFalling/6);
				// now the falling simulation with blinking the proper segments
					// turning off
							lowerCharSegments[randBanana].a = 0;
							SegmentLCD_LowerSegments(lowerCharSegments);
					// stalking
							chooseBasketDir();
					// falling adjust (j) -> see: segmentlcd_individual.h
							lowerCharSegments[randBanana].j = 1;
							SegmentLCD_LowerSegments(lowerCharSegments);
							gameOnInterrupt(timeOfFalling/6);
							chooseBasketDir();
							Delay(timeOfFalling/5);
							gameOnInterrupt(timeOfFalling/6);
					// falling adjust (j) -> see: segmentlcd_individual.h
							lowerCharSegments[randBanana].j = 0;
							SegmentLCD_LowerSegments(lowerCharSegments);
							chooseBasketDir();
					// falling adjust (p) -> see: segmentlcd_individual.h
							lowerCharSegments[randBanana].p = 1;
							SegmentLCD_LowerSegments(lowerCharSegments);
							chooseBasketDir();
							gameOnInterrupt(timeOfFalling/6);
							Delay(timeOfFalling/5);
							gameOnInterrupt(timeOfFalling/6);
							chooseBasketDir();
					// check if the basket collided with banana,
					// us or the user caught the banana with the basket
					// collided
							if(basketCurrDir == randBanana){
								i++;
								SegmentLCD_Number((i * 100 + j));
							}
							else{
								j++;
								SegmentLCD_Number((i * 100 + j));
							}
					// falling adjust (p) -> see: segmentlcd_individual.h
						lowerCharSegments[randBanana].p = 0;
						SegmentLCD_LowerSegments(lowerCharSegments);
					// Counting the bananas (it is independent from if it caught or not)
						currNumOfBananas++;
			}
		// Turning off the actual segment (cls the screen), for the next game
			// falling adjust (d) -> see: segmentlcd_individual.h
			lowerCharSegments[basketCurrDir].p = 0;
			SegmentLCD_LowerSegments(lowerCharSegments);
		// Some contact with the player
			SegmentLCD_Write("  END");
			Delay(2000);
		// For one bug:
			// the previous basket does not wanna be removed (for one moving) when one new game starts
			// despite of we call that function at startTheGame()
			setConstants();
			clearingSegments();
		//Adjust the seeder for making random number
			seeder += (seederCounter + msTicks);
		// Counting the played games, and  increase the seederCounter
			playedGames++;
			seederCounter++;
		// return with false,
		// important because of the isGameStarted variable
		return false;
	}
	void clearingSegments(){
		uint8_t iterator;
		for(iterator = 0; iterator < 4; iterator++)
		{
			lowerCharSegments[iterator].d = 0;
			SegmentLCD_LowerSegments(lowerCharSegments);
		}
	}
	void gameOnInterrupt(uint32_t pTicks){
		// almost similar to delay()...
		uint32_t currTicks;
			// save the parameter ticks to count the time with it
			currTicks = pTicks;
		// algorithm
			// algorithm variable for multiply
				uint8_t algoHelper = 1;
			// waiting pTicks time
				while((msTicks - currTicks) < pTicks){
					// stalking the input/"interrupt" within defaultLatency
					if((msTicks - currTicks) > (defaultLatency * algoHelper)){
						algoHelper++;
						chooseBasketDir();
					}
				}
	}
	void chooseBasketDir(){
		if(!BSP_ButtonGet(0)){
			// PB0 is pressed then go to left with the basket
			basketMoveToL();
		}
		if(!BSP_ButtonGet(1)){
			// PB1 is pressed then go to right with the basket
			basketMoveToR();
		}
	}
	void basketMoveToL(){
		// save the current value for working with it, before
				// we change the basketCurrDir
					uint8_t prevSegment = basketCurrDir;
				// If the basket on the first segment then go to last
					if(basketCurrDir == START_EDGE){
						basketCurrDir = END_EDGE;
					}
					else{
						basketCurrDir -= 1;
					}
				// turning off the previous state's segment
					lowerCharSegments[prevSegment].d = 0;
					SegmentLCD_LowerSegments(lowerCharSegments);
				// turning on the current
					lowerCharSegments[basketCurrDir].d = 1;
					SegmentLCD_LowerSegments(lowerCharSegments);
	}
	void basketMoveToR(){
		// save the current value for working with it, before
		// we change the basketCurrDir
			uint8_t prevSegment = basketCurrDir;
		// If the basket on the last segment then go to first
			if(basketCurrDir == END_EDGE){
				basketCurrDir = START_EDGE;
			}
			else{
				basketCurrDir += 1;
			}
		// turning off the previous state's segment
			lowerCharSegments[prevSegment].d = 0;
			SegmentLCD_LowerSegments(lowerCharSegments);
		// turning on the current
			lowerCharSegments[basketCurrDir].d = 1;
			SegmentLCD_LowerSegments(lowerCharSegments);
	}
	void startTheGame(){
		isGameStarted = true;
		setConstants();
		if(difficultyLvl == EASY){
			// The user just pushed instantly the PB1 then turn of the 0th ring segment
			SegmentLCD_ARing(EASY,1);
		}
		// Now the LCD
			SegmentLCD_Number(0);
			SegmentLCD_Symbol(LCD_SYMBOL_COL10,1);
		// Set the basket to the default stage, on the left edge
			lowerCharSegments[basketCurrDir].d = 1;
			SegmentLCD_LowerSegments(lowerCharSegments);
		// Some interaction for the User
			SegmentLCD_Write(" READY");
			Delay(1000);	// ms
			SegmentLCD_Write(" START");
			Delay(1000);
			SegmentLCD_Write("   3");
			Delay(1000);
			SegmentLCD_Write("   2");
			Delay(1000);
			SegmentLCD_Write("   1");
			Delay(1000);
			SegmentLCD_Write("  GO");
			Delay(1000);
	}
	void setConstants(){
		currNumOfBananas = ZERO;
		basketCurrDir = ZERO;
	}
	void Delay(uint32_t pDelayTicks){
		uint32_t currTicks;
		currTicks = msTicks;
		while((msTicks - currTicks) < pDelayTicks);
	}
	void diffController(){
		if(difficultyLvl == LEGENDARY){
			while(difficultyLvl != EASY){
				SegmentLCD_ARing(difficultyLvl,0);
				difficultyLvl--;
				timeOfFalling += defaultFallTimeChange;
			}
			SegmentLCD_Write("RESETED");
			SegmentLCD_ARing(difficultyLvl,1);
		}else{
			if(playedGames == 0){	// If the device reseted and the user just pressed the PB0, then it is on the second difficulty lever, but the first ring segment does not active
				SegmentLCD_ARing(difficultyLvl,1);
			}
			difficultyLvl++;
			timeOfFalling -= defaultFallTimeChange;
			SegmentLCD_ARing(difficultyLvl,1);
			SegmentLCD_Write("DIFF+");
			setConstants();
		}
	}




