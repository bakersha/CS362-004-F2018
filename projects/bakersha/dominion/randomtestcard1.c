/*******************************************************
 * Name: 
 * 		randomcardtest1.c
 * Description: 
 * 		Random test suite for smithy card
 * Author: 
 * 		Sharaya Baker (bakersha) - November 2018
 *******************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

#define TESTCARD "smithy"
#define MIN_PLAYERS 2
/* MAX_HAND, MAX_DECK, MAX_PLAYERS defined in dominion.h */

/*
 * REQUIREMENTS:
 * 	1. Current player should receive exact 3 cards.
 *	2. 3 cards should come from his own pile.
 * 	3. No state change should occur for other players.
 * 	4. No state change should occur to the victory card piles and kingdom card piles.
 *  5. Discard pile should increase by 1.
 */


// Test oracle
void myAssert(int x, int y) {
	if(x == y) {
		printf("TEST PASSED!\n");
	} else {
		printf("TEST FAILED!\n");
	}
}

int main() {

	srand(time(NULL)); // seed rand

	/* --- Variables ---  */
	struct gameState testState;	// New instance of gameState
	int victoryBefore = 0;		// Used to verify no state change occurs
	int victoryAfter = 0; 		// Used to verify no state change occurs
	int kingdomBefore = 0; 		// Used to verify no state change occurs
	int kingdomAfter = 0;		// Used to verify no state change occurs
	int i, j, n;				// Counters
	int numT1P = 0; 			// Track the number of pass / fail for Test cases
	int numT1F = 0;		
	int numT2P = 0;			
	int numT2F = 0;	
	int numT3aP =0;
	int numT3aF = 0;	
	int numT3bP = 0;
	int numT3bF = 0;	
    int numT4aP = 0;
    int numT4aF = 0;	
    int numT4bP = 0;
    int numT4bF = 0;	
	int iterFailContainer[100] = {0};
	int failCount = 0;

    // The variables below came from the cardtest4.c example provided 	
    int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;	
	int k[10] = {adventurer, embargo, village, 
				minion, mine, cutpurse, 
				sea_hag, tribute, smithy, 
				council_room};	

	/* --- START TEST --- */

	printf("\n--------------------------------------------------------------------\n");
	printf("------------------- STARTING SMITHY RANDOM TESTS ------------------\n");
	printf("--------------------------------------------------------------------");	

	for (i = 1; i < 15000; i++) {

		/* ---------- BEFORE CARD IS PLAYED ---------- */
		printf("\n\n********** Iteration %d **********\n", i+1);

		// Random seed for cardEffect()
		int seed = rand();
		printf("Seed: %d\n", seed); 

		// Get a random number of players between MIN and MAX_PLAYERS
		// https://www.geeksforgeeks.org/generating-random-number-range-c/
		int numPlayers = (rand() % (MAX_PLAYERS - MIN_PLAYERS + 1)) + MIN_PLAYERS;	
		printf("numPlayers: %d\n", numPlayers);

		// Initialize a game state and player cards
		initializeGame(numPlayers, k, seed, &testState);	

		// Get random currentPlayer 
		int currentPlayer = rand() % numPlayers;
		testState.whoseTurn = currentPlayer;
		printf("currentPlayer: %d\n", currentPlayer);

		// Number of cards in currentPlayer's hand before playing Smithy
		testState.handCount[currentPlayer] = rand() % MAX_HAND;
		int startingHandCount = testState.handCount[currentPlayer];
		printf("currentPlayer startingHandCount: %d\n", startingHandCount);
		// Expected number of cards in currentPlayer's hand after playing Smithy
		// Number is 2 instead of 3 to account for discard of smithy
		int expectedHandCount = startingHandCount + 2;	
		printf("Expected ending handCount: %d\n", expectedHandCount);
		// Number of cards in currentPlayer's deck before playing Smithy	
		testState.deckCount[currentPlayer] = rand() % MAX_DECK;
		int startingDeckCount = testState.deckCount[currentPlayer];
		printf("currentPlayer startingDeckCount: %d\n", startingDeckCount); 
		// Expected number of cards in currentPlayer's deck after playing Smithy
		int expectedDeckCount = startingDeckCount - 3; 	
		printf("Expected ending deckCount: %d\n", expectedDeckCount);

		// Starting / ending handCount and deckCount for other players		
		int otherPlayerStartingHandCount[numPlayers-1];	
		int otherPlayerStartingDeckCount[numPlayers-1];

		// Randomize the starting handCount and deckCount for the other players
		for (j = 0; j < numPlayers; j++) {
			if (j != currentPlayer) {			
				testState.handCount[j] = rand() % MAX_HAND;
				otherPlayerStartingHandCount[j] = testState.handCount[j]; 
				testState.deckCount[j] = rand() % MAX_HAND;
				otherPlayerStartingDeckCount[j] = testState.deckCount[j];
				printf("Player %d startingHandCount: %d\n", j, otherPlayerStartingHandCount[j]);
				printf("Player %d startingDeckCount: %d\n", j, otherPlayerStartingDeckCount[j]);
			} else {
				otherPlayerStartingHandCount[j] = 0;
				otherPlayerStartingDeckCount[j] = 0;
			}
		}	

		// This determines the supply of victory cards before Smithy is played
		for ( j = estate; j < copper; j++ ) {
			victoryBefore += testState.supplyCount[j];
		}

		// This determines the supply of kingdom cards before Smithy is played 	
		for ( j = 0; j < 10; j++ ) {
			n = k[j];
			kingdomBefore += testState.supplyCount[n];
		}

		printf("Starting victory pile: %d\nStarting kingdom pile: %d\n", victoryBefore, kingdomBefore);

		/* ---------- PLAY SMITHY ---------- */
		cardEffect(smithy, choice1, choice2, choice3, &testState, handPos, &bonus);

		/* ---------- AFTER CARD IS PLAYED ---------- */

		printf("\n------------------------------ Test 1 -----------------------------\n\n");
		printf("Expected: Current player received exactly 3 cards.\n");
		printf("Expected handCount: %d\n", expectedHandCount);
		printf("Actual: %d\n", testState.handCount[currentPlayer]);
		myAssert(testState.handCount[currentPlayer], expectedHandCount);

		if (testState.handCount[currentPlayer] == expectedHandCount){
			numT1P++;
		} else if (testState.handCount[currentPlayer] != expectedHandCount) {
			numT1F++;
			//iterFailContainer[failCount] = i;
			//failCount++;
		}

		printf("\n------------------------------ Test 2 -----------------------------\n\n");
		printf("Expected: currentPlayer deckCount = %d\n", expectedDeckCount);
		printf("Actual: currentPlayer deckCount = %d\n",testState.deckCount[currentPlayer]);
		myAssert(testState.deckCount[currentPlayer], expectedDeckCount);

		if (testState.deckCount[currentPlayer] == expectedDeckCount){
			numT2P++;
		} else if (testState.deckCount[currentPlayer] != expectedDeckCount) {
			numT2F++;
			//iterFailContainer[failCount] = i;
			//failCount++;
		}

		printf("\n------------------------------ Test 3 -----------------------------\n\n");
		printf("Expected: No state changes to other players.\n");

		// Determine the handCount and deckCount for other players
		for (j = 0; j < numPlayers; j++) {
			if (j != currentPlayer) {
				printf("Expected handCount, player %d: %d\n", j, otherPlayerStartingHandCount[j]);
				printf("Actual handCount: %d\n", testState.handCount[j]);
				myAssert(otherPlayerStartingHandCount[j], testState.handCount[j]);
				
				if (otherPlayerStartingHandCount[j] == testState.handCount[j]){
					numT3aP++;
				} else if (otherPlayerStartingHandCount[j] != testState.handCount[j]) {
					numT3aF++;
					iterFailContainer[failCount] = i;
					failCount++;
				}

				printf("Expected deckCount, player %d: %d\n", j, otherPlayerStartingDeckCount[j]);
				printf("Actual deckCount: %d\n", testState.deckCount[j]);
				myAssert(otherPlayerStartingDeckCount[j], testState.deckCount[j]);
				
				if (otherPlayerStartingDeckCount[j] == testState.deckCount[j]){
					numT3bP++;
				} else if (otherPlayerStartingDeckCount[j] != testState.deckCount[j]) {
					numT3bF++;
					iterFailContainer[failCount] = i;
					failCount++;
				}
			}
		}

		printf("\n------------------------------ Test 4 -----------------------------\n\n");
		printf("Expected: No state changes to victory or kingdom piles.\n");

		/* This determines the supply of victory cards before Smithy is played */
		for ( j = estate; j < copper; j++ ){
			victoryAfter += testState.supplyCount[j];
		}

		printf("Expected Victory pile: %d\n", victoryBefore);
		printf("Actual Victory pile: %d\n", victoryAfter);
		myAssert(victoryBefore, victoryAfter);

		if (victoryBefore == victoryAfter) {
			numT4aP++;
		} else if (victoryBefore != victoryAfter) {
			numT4aF++;
			iterFailContainer[failCount] = i;
			failCount++;
		}

		/* This determines the supply of kingdom cards before Smithy is played */	
		for ( j = 0; j < 10; j++ ){
			n = k[j];
			kingdomAfter += testState.supplyCount[n];
		}

		printf("Expected Kingdom pile: %d\n", kingdomBefore);
		printf("Actual Kingdom pile: %d\n", kingdomAfter);
		myAssert(kingdomBefore, kingdomAfter);

		if (kingdomBefore == kingdomAfter) {
			numT4bP++;
		} else if (kingdomBefore != kingdomAfter) {
			numT4bF++;
			iterFailContainer[failCount] = i;
			failCount++;
		}		

	}

	printf("\n------------------------------ Test 5 -----------------------------\n\n");
	printf("Expected: Discard pile increases by 1 ******** UNDER CONSTRUCTION *********\n");	

	printf("\n------------------ TOTAL PASSED / FAILED TEST CASES ------------------\n\n");
	printf("Test 1 - PASSED: %d\n", numT1P);
	printf("Test 1 - FAILED: %d\n", numT1F);
	printf("Test 2 - PASSED: %d\n", numT2P);
	printf("Test 2 - FAILED: %d\n", numT2F);
	printf("Test 3a - PASSED: %d\n", numT3aP);
	printf("Test 3a - FAILED: %d\n", numT3aF);	
	printf("Test 3b - PASSED: %d\n", numT3bP);
	printf("Test 3b - FAILED: %d\n", numT3bF);
	printf("Test 4a - PASSED: %d\n", numT4aP);
	printf("Test 4a - FAILED: %d\n", numT4aF);	
	printf("Test 4b - PASSED: %d\n", numT4bP);
	printf("Test 4b - FAILED: %d\n", numT4bF);		


	printf("ITERATIONS WHICH CONTAINED FAILURES -- NOT TEST 1 OR TEST 2:\n");
	for (i = 0; i < 100; i++) {
		if (iterFailContainer[i] != 0) {
			printf("%d\n",iterFailContainer[i]);
		}
	}


	printf("\n--------------------------------------------------------------------\n");
	printf("------------------------- TESTING COMPLETE -------------------------\n");
	printf("--------------------------------------------------------------------\n\n");	

	return 0;
}



























