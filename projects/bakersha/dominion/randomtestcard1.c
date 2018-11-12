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
#include <math.h>

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

enum TCP { 	
	t1P = 0,	// Track the number of pass for Test cases
	t2P,
	t3aP,
	t3bP,
	t4P
};

enum TCF { 	
	t1F = 0,	// Track the number of fail for Test cases
	t2F,	
	t3aF,
	t3bF,
	t4F
};

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
	int i, j;								// Counters
	int supplyBefore  				= 0;	// Used to verify no state change occurs
	int supplyAfter   				= 0; 	// Used to verify no state change occurs
	int tcPass[5] 					= {0};	// Holds passed TC count
	int tcFail[5] 					= {0};	// Holds failed TC count
	int iterFailContainer[40000] 	= {0};
	int failCount 					= 0;

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
		printf("\n\n********** Iteration %d **********\n", i);

		// Random seed for cardEffect()
		// Set up Random() from rngs.c
		// Using numbers from testDrawCard.c
		SelectStream(2);
		int seed = rand() + 1;

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

		// Non-randomized hand / deck counts for currentPlayer
		/*
		int startingHandCount = testState.handCount[currentPlayer];
		printf("currentPlayer startingHandCount: %d\n", startingHandCount);
		*/

		// Number of cards in currentPlayer's hand before playing Smithy
		testState.handCount[currentPlayer] = floor(Random() * MAX_HAND);
		int startingHandCount = testState.handCount[currentPlayer];
		printf("currentPlayer startingHandCount: %d\n", startingHandCount);

		// Expected number of cards in currentPlayer's hand after playing Smithy
		// Number is 2 instead of 3 to account for discard of smithy
		int expectedHandCount = startingHandCount + 2;	
		printf("Expected ending handCount: %d\n", expectedHandCount);

		// Number of cards in currentPlayer's deck before playing Smithy
		testState.deckCount[currentPlayer] = floor(Random() * MAX_DECK);
		int startingDeckCount = testState.deckCount[currentPlayer];
		printf("currentPlayer startingDeckCount: %d\n", startingDeckCount); 

		// Non-random version
		//int startingDeckCount = testState.deckCount[currentPlayer];	

		// Num cards in currentPlayer's discard pile before playing Smithy
        testState.discardCount[currentPlayer] = floor(Random() * MAX_DECK);
        int discCount = testState.discardCount[currentPlayer];
        printf("currentPlayer discardCount: %d\n", testState.discardCount[currentPlayer]);

		// Expected number of cards in currentPlayer's deck after playing Smithy
		int expectedDeckCount;
		if (startingDeckCount >= 3) {
			expectedDeckCount = startingDeckCount - 3;
		} else {
			expectedDeckCount = 0;
		}

		printf("Expected ending deckCount: %d\n", expectedDeckCount);

		// Starting / ending handCount and deckCount for other players		
		int otherPlayerStartingHandCount[numPlayers];	
		int otherPlayerStartingDeckCount[numPlayers];
		int otherPlayerExpectedHandCount[numPlayers];	
		int otherPlayerExpectedDeckCount[numPlayers];

		// Starting handCount, deckCount, discardCount for the other players
		for (j = 0; j < numPlayers; j++) {
			if (j != currentPlayer) {	
				// discard count
				testState.discardCount[j] = floor(Random() * MAX_DECK);
				otherPlayerStartingDiscardCount[j] = testState.discardCount[j];					
				//testState.handCount[j] = rand() % MAX_HAND;
				otherPlayerStartingHandCount[j] = testState.handCount[j]; 
				otherPlayerExpectedHandCount[j] = otherPlayerStartingHandCount[j];
				//testState.deckCount[j] = rand() % MAX_HAND;
				otherPlayerStartingDeckCount[j] = testState.deckCount[j];
				otherPlayerExpectedDeckCount[j] = otherPlayerStartingDeckCount[j];
				// discard count
				testState.discardCount[j] = floor(Random() * MAX_DECK);
				printf("Player %d startingHandCount: %d\n", j, otherPlayerStartingHandCount[j]);
				printf("Player %d startingDeckCount: %d\n", j, otherPlayerStartingDeckCount[j]);
				printf("Player %d expected endingHandCount: %d\n", j, otherPlayerExpectedHandCount[j]);
				printf("Player %d expected endingDeckCount: %d\n", j, otherPlayerExpectedDeckCount[j]);				
			} else {
				otherPlayerStartingHandCount[j] = 0;
				otherPlayerStartingDeckCount[j] = 0;
				otherPlayerExpectedHandCount[j] = 0;
				otherPlayerExpectedDeckCount[j] = 0;	
			}
		}

		// Determines supply count of all cards
		for (j = curse; j < treasure_map; j++) {
			supplyBefore += testState.supplyCount[j];
		}
		printf("Starting total supplyCount: %d\n", supplyBefore);

		/* ---------- PLAY SMITHY ---------- */
		cardEffect(smithy, choice1, choice2, choice3, &testState, handPos, &bonus);

		/* ---------- AFTER CARD IS PLAYED ---------- */

		printf("\n------------------------------ Test 1 -----------------------------\n\n");
		printf("Expected: Current player received exactly 3 cards.\n");
		printf("Expected handCount: %d\n", expectedHandCount);
		printf("Actual: %d\n", testState.handCount[currentPlayer]);
		myAssert(testState.handCount[currentPlayer], expectedHandCount);

		if (testState.handCount[currentPlayer] == expectedHandCount){
			tcPass[t1P] += 1;
		} else if (testState.handCount[currentPlayer] != expectedHandCount) {
			tcFail[t1F] += 1;
			iterFailContainer[failCount] = i;
			failCount++;
		}

		printf("\n------------------------------ Test 2 -----------------------------\n\n");
		printf("Expected: currentPlayer deckCount = %d\n", expectedDeckCount);
		printf("Actual: currentPlayer deckCount = %d\n",testState.deckCount[currentPlayer]);
		myAssert(testState.deckCount[currentPlayer], expectedDeckCount);

		if (testState.deckCount[currentPlayer] == expectedDeckCount){
			tcPass[t2P] += 1;
		} else if (testState.deckCount[currentPlayer] != expectedDeckCount) {
			tcFail[t2F] += 1;
			iterFailContainer[failCount] = i;
			failCount++;
		}

		printf("\n------------------------------ Test 3 -----------------------------\n\n");
		printf("Expected: No state changes to other players.\n");

		// Determine the handCount and deckCount for other players
		for (j = 0; j < numPlayers; j++) {
			if (j != currentPlayer) {
				printf("Expected handCount, player %d: %d\n", j, otherPlayerStartingHandCount[j]);
				printf("Actual handCount: %d\n", testState.handCount[j]);
				myAssert(otherPlayerStartingHandCount[j], testState.handCount[j]);
				
				if (otherPlayerExpectedHandCount[j] == testState.handCount[j]){
					tcPass[t3aP] += 1;
				} else if (otherPlayerExpectedHandCount[j] != testState.handCount[j]) {
					tcFail[t3aF] += 1;
					iterFailContainer[failCount] = i;
					failCount++;
				}

				printf("Expected deckCount, player %d: %d\n", j, otherPlayerStartingDeckCount[j]);
				printf("Actual deckCount: %d\n", testState.deckCount[j]);
				myAssert(otherPlayerStartingDeckCount[j], testState.deckCount[j]);
				
				if (otherPlayerExpectedDeckCount[j] == testState.deckCount[j]){
					tcPass[t3bP] += 1;
				} else if (otherPlayerExpectedDeckCount[j] != testState.deckCount[j]) {
					tcFail[t3bF] += 1;
					iterFailContainer[failCount] = i;
					failCount++;
				}
			}
		}

		printf("\n------------------------------ Test 4 -----------------------------\n\n");
		printf("Expected: No state changes to supplyCount.\n");
		// Determines supply count of all cards
		for (j = curse; j < treasure_map; j++) {
			supplyAfter += testState.supplyCount[j];
		}
		printf("Expected supplyCount: %d\n", supplyBefore);
		printf("Actual supplyCount: %d\n", supplyAfter);
		myAssert(supplyBefore,supplyAfter);

		if (supplyBefore == supplyAfter) {
			tcPass[t4P] += 1;
		} else {
			tcFail[t4F] += 1;
			iterFailContainer[failCount] = i;
			failCount++;
		}

		// Reset supply counters
		supplyBefore = 0;
		supplyAfter = 0;		
	}

	printf("\n--------------------------------------------------------------------\n");
	printf("------------------------- TESTING COMPLETE -------------------------\n");
	printf("--------------------------------------------------------------------\n\n");	

	if (failCount == 0) {
		printf("ALL TESTS PASSED!\n");
	} else {
		for (i = 0; i < 5; i++) {
			switch(i) {
				case t1F:
					if (tcFail[t1F] != 0) {
						printf("Test 1 Fail count: %d\n", tcFail[t1F]);
					}
					continue;
				case t2F:
					if (tcFail[t2F] != 0) {
						printf("Test 2 Fail count: %d\n", tcFail[t2F]);
					}
					continue;
				case t3aF:
					if (tcFail[t3aF] != 0) {
						printf("Test 3a Fail count: %d\n", tcFail[t3aF]);
					}
					continue;
				case t3bF:
					if (tcFail[t3bF] != 0) {
						printf("Test 3b Fail count: %d\n", tcFail[t3bF]);
					}
					continue;
				case t4F:
					if (tcFail[t4F] != 0) {
						printf("Test 4 Fail count: %d\n", tcFail[t4F]);
					}
					continue;
			}	
		}
		printf("\nTHE FOLLOWING ITERATIONS CONTAINED FAILURES:\n");
		for (j = 0; j < failCount; j++) {
			if (iterFailContainer[j] != 0) {
				printf("%d\n",iterFailContainer[j]);
			}
		}
	}

	return 0;
}



























