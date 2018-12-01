/*******************************************************
 * Name: 
 * 		cardtest2.c
 * Description: 
 * 		Unit test suite for adventurer card
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

#define TESTCARD "adventurer"

/*
 * REQUIREMENTS:
 * 	1. Draw cards until 2 treasure found.
 *	2. All cards should come from player's own pile.
 * 	3. All drawn cards that are not treasure are discarded.
 * 	4. No state change should occur for other players.
 * 	5. No state change should occur to the victory card piles and kingdom card piles.
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

	printf("\n----------------------- Adventurer Unit Tests ----------------------\n");	
	
	printf("\n-------------------------- Starting Test 1 -------------------------\n");
	printf("----------------- Draw card until 2 treasure found -----------------\n");
	printf("--------------------------------------------------------------------\n\n");		

	// Manipulate player's hand
	testState.hand[currentPlayer][0] = copper; 		// Enum 4
	testState.hand[currentPlayer][1] = estate;		// Enum 1
	testState.hand[currentPlayer][2] = silver;		// Enum 5
	testState.hand[currentPlayer][3] = duchy;		// Enum 2
	testState.hand[currentPlayer][4] = gold;		// Enum 6
	
	// Manipulate player's deck
	testState.deck[currentPlayer][0] = copper; 		// Enum 4
	testState.deck[currentPlayer][1] = estate;		// Enum 1
	testState.deck[currentPlayer][2] = silver;		// Enum 5 
	testState.deck[currentPlayer][3] = duchy;		// Enum 2
	testState.deck[currentPlayer][4] = gold;		// Enum 6	

	int startingDeckCount = testState.deckCount[currentPlayer]; // Number of cards in currentPlayer's deck before playing Adventurer
	int expectedDeckCount = startingDeckCount - 3; 				// Expected number of cards in currentPlayer's deck after playing Adventurer
	// This number is expected because adventurer should be counting copper and silver as treasure cards, discarding estate along the way			

	printf("Playing %s card ...\n", TESTCARD);
	cardEffect(adventurer, choice1, choice2, choice3, &testState, handPos, &bonus);	// Play the card

	printf("Expected top card in hand: enum %d, actual: enum %d\n", silver, testState.hand[currentPlayer][0]);
	myAssert(silver,testState.hand[currentPlayer][0]);

	printf("\n------------------------------ Test 2 ------------------------------\n");
	printf("---------------- Drawn cards are from player's deck ----------------\n");
	printf("--------------------------------------------------------------------\n\n");

	printf("Deck count before playing %s: %d\n", TESTCARD, startingDeckCount);
	printf("Expected deck count after: %d, actual: %d\n", expectedDeckCount, testState.deckCount[currentPlayer]);
	myAssert(expectedDeckCount, testState.deckCount[currentPlayer]);
	

	printf("\n--------------------------------------------------------------------\n");
	printf("------------------------- TESTING COMPLETE -------------------------\n");
	printf("--------------------------------------------------------------------\n\n");		

	return 0;
}