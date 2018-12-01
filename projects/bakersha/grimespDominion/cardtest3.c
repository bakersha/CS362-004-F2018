/*******************************************************
 * Name: 
 * 		cardtest3.c
 * Description: 
 * 		Unit test suite for council_room card
 * Author: 
 * 		Sharaya Baker (bakersha) - October 28, 2018
 *******************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "council_room"

/*
 * REQUIREMENTS:
 * 
 */

void myAssert(x,y)
{
	if (x == y)
		printf("TEST PASSED!\n");
	else
		printf("TEST FAILED!\n");
}

int main()
{
	/* --- Variables --- */
	struct gameState testState;	
	int numPlayers = MAX_PLAYERS;
	int currentPlayer = 0;			
    /* The variables below came from the cardtest4.c example provided */	
    int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;	
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};	
    int seed = 1000;	

	// Initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &testState);

	printf("\n---------------------- Council Room Unit Tests ---------------------\n");	
	
	printf("\n-------------------------- Starting Test 1 -------------------------\n");
	printf("----------------- Draw card until 2 treasure found -----------------\n");
	printf("--------------------------------------------------------------------\n\n");		
	return 0;
}
