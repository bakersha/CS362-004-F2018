/*******************************************************
 * Name: 
 * 		cardtest1.c
 * Description: 
 * 		Unit test suite for smithy card
 * Author: 
 * 		Sharaya Baker (bakersha) - April 29, 2018
 *******************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "Smithy"

/*
 * REQUIREMENTS:
 * 	1. Current player should receive exact 3 cards.
 *	2. 3 cards should come from his own pile.
 * 	3. No state change should occur for other players.
 * 	4. No state change should occur to the victory card piles and kingdom card piles.
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
	int otherPlayerStartingHandCount[3]; // Array to hold the starting handCount values for each of the other players
	int otherPlayerEndingHandCount[3];   // Array to hold the ending handCount values for each of the other players
	int otherPlayerStartingDeckCount[3]; // Array to hold the starting deckCount values for each of the other players
	int otherPlayerEndingDeckCount[3];   // Array to hold the ending deckCount values for each of the other players	
	int victoryBefore = 0;
	int victoryAfter = 0; 
	int kingdomBefore = 0; 
	int kingdomAfter = 0;
	int i;	
    /* The variables below came from the cardtest4.c example provided */	
    int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;	
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};	
    int seed = 1000;
	
	// Initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &testState);	

	printf("\n---------------------- Smithy Unit Tests ---------------------\n");	
	
	/* Test 1: Current player should receive exactly 3 cards. */	
	printf("\n-------------------------- Starting Test 1 -------------------------\n");
	printf("--- 1. Current player should receive exactly 3 cards ---------------\n");
	printf("--- 2. The cards should come from the current player's pile --------\n");	
	printf("--------------------------------------------------------------------\n\n");	   

	int startingHandCount = testState.handCount[currentPlayer]; // Number of cards in currentPlayer's hand before playing Smithy
	int expectedHandCount = startingHandCount + 3;				// Expected number of cards in currentPlayer's hand after playing Smithy
	int startingDeckCount = testState.deckCount[currentPlayer]; // Number of cards in currentPlayer's deck before playing Smithy
	int expectedDeckCount = startingDeckCount - 4; 				// Expected number of cards in currentPlayer's deck after playing Smithy
	// This is -4 instead of -3 because of the discardCard() in addition to moving 3 cards from deck to hand

	/* This determines how many cards are in the hands of the other players before Smithy is played */
	for ( i = (currentPlayer + 1); i < MAX_PLAYERS; i++ )
	{
		if ( i == currentPlayer)
			i++;
		if (currentPlayer == 3) // If the last player
			i = 0; 				// Set back to player 1 (0)
		otherPlayerStartingHandCount[i] = testState.handCount[i];	
	}

	/* This determines how many cards are in the decks of the other players before Smithy is played */
	for ( i = (currentPlayer + 1); i < MAX_PLAYERS; i++ )
	{
		if ( i == currentPlayer)
			i++;		
		if (currentPlayer == 3) // If the last player
			i = 0; 				// Set back to player 1 (0)
		otherPlayerStartingDeckCount[i] = testState.deckCount[i];	
	}	

	/* This determines the supply of victory cards before Smithy is played */
	for ( i = estate; i < copper; i++ )
	{
		victoryBefore += testState.supplyCount[i];
	}

	/* This determines the supply of kingdom cards before Smithy is played */	
	int j;	
	for ( i = 0; i < 10; i++ )
	{
		j = k[i];
		kingdomBefore += testState.supplyCount[j];
	}

	printf("Current player (%d) handCount before %s: %d\n", currentPlayer, TESTCARD, startingHandCount);
	printf("Current player (%d) deckCount before %s: %d\n", currentPlayer, TESTCARD, startingDeckCount);

	cardEffect(smithy, choice1, choice2, choice3, &testState, handPos, &bonus);	// Play the card

	printf("Current player (%d) handCount after %s: %d, expected: %d\n", currentPlayer, TESTCARD, testState.handCount[currentPlayer], expectedHandCount);
	myAssert(testState.handCount[currentPlayer], expectedHandCount);

	printf("Current player (%d) deckCount after %s: %d, expected: %d\n", currentPlayer, TESTCARD, testState.deckCount[currentPlayer], expectedDeckCount);
	myAssert(testState.deckCount[currentPlayer], expectedDeckCount);

	printf("\n-------------------------- Starting Test 3 -------------------------\n");
	printf("---------------- No state change to other players ------------------\n");
	printf("--------------------------------------------------------------------\n\n");	 	

	/* This determines how many cards are in the hands of the other players after Smithy is played */
	for ( i = (currentPlayer + 1); i < MAX_PLAYERS; i++ )
	{
		if ( i == currentPlayer)
			i++;		
		if (currentPlayer == 3) // If the last player
			i = 0; 				// Set back to player 1 (0)
		otherPlayerEndingHandCount[i] = testState.handCount[i];	
		printf("Player %d handCount before %s: %d\n", i, TESTCARD, otherPlayerStartingHandCount[i]);	
		printf("Player %d handCount after %s: %d\n", i, TESTCARD, otherPlayerEndingHandCount[i]);				
		myAssert(otherPlayerStartingHandCount[i], otherPlayerEndingHandCount[i]);
	}

	/* This determines how many cards are in the decks of the other players after Smithy is played */
	for ( i = (currentPlayer + 1); i < MAX_PLAYERS; i++ )
	{
		if ( i == currentPlayer)
			i++;		
		if (currentPlayer == 3) // If the last player
			i = 0; 				// Set back to player 1 (0)
		otherPlayerEndingDeckCount[i] = testState.deckCount[i];
		printf("Player %d deckCount before %s: %d\n", i, TESTCARD, otherPlayerStartingDeckCount[i]);	
		printf("Player %d deckCount after %s: %d\n", i, TESTCARD, otherPlayerEndingDeckCount[i]);				
		myAssert(otherPlayerStartingDeckCount[i], otherPlayerEndingDeckCount[i]);			
	}			

	printf("\n-------------------------- Starting Test 4 -------------------------\n");
	printf("----------- No state changes to victory or kingdom piles -----------\n");
	printf("--------------------------------------------------------------------\n\n");	 

	/* This determines the supply of victory cards before Smithy is played */
	for ( i = estate; i < copper; i++ )
	{
		victoryAfter += testState.supplyCount[i];
	}

	/* This determines the supply of kingdom cards before Smithy is played */	
	for ( i = 0; i < 10; i++ )
	{
		j = k[i];
		kingdomAfter += testState.supplyCount[j];
	}	

	printf("Victory pile count before %s: %d\n", TESTCARD, victoryBefore);
	printf("Victory pile count after %s: %d\n", TESTCARD, victoryAfter);
	myAssert(victoryBefore, victoryAfter);
	printf("Kingdom pile count before %s: %d\n", TESTCARD, kingdomBefore);
	printf("Kingdom pile count after %s: %d\n", TESTCARD, kingdomAfter);
	myAssert(kingdomBefore, kingdomAfter);	

	printf("\n--------------------------------------------------------------------\n");
	printf("------------------------- TESTING COMPLETE -------------------------\n");
	printf("--------------------------------------------------------------------\n\n");		

	return 0;
}

