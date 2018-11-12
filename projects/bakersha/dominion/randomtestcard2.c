/*******************************************************
 * Name: 
 * 		randomcardtest1.c
 * Description: 
 * 		Random test suite for council room card
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
 * 	1. currentPlayer draws 4 cards
 *	2. currentPlayer gets 1 Buy
 * 	3. Other players must draw 1 card
 * 	4. Played card moves to discard pile
 *  5. No change to other card piles
 */

enum TCP { 	
	t1P = 0,	// Track the number of pass / fail for Test cases
	t2P,
	t3P,	
	t4aP,
	t4bP,
	t5P
};

enum TCF { 	
	t1F = 0,	// Track the number of pass / fail for Test cases
	t2F,
	t3F,	
	t4aF,
	t4bF,
	t5F
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
	struct gameState testState;				// New instance of gameState
	int i, j;								// Counters
	int supplyBefore  				= 0;	// Used to verify no state change occurs
	int supplyAfter   				= 0; 	// Used to verify no state change occurs
	int tcPass[6] 					= {0};	// Holds passed TC count
	int tcFail[6] 					= {0};	// Holds failed TC count
	int iterFailContainer[40000] 	= {0};  // Holds iterations that had failed TCs
	int failCount 	  				= 0;	// Index for iterFailContainer[]	

    // The variables below came from the cardtest4.c example provided 	
    int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;	
	int k[10] = {adventurer, embargo, village, 
				minion, mine, cutpurse, 
				sea_hag, tribute, smithy, 
				council_room};	

	/* --- START TEST --- */

	printf("\n--------------------------------------------------------------------\n");
	printf("---------------- STARTING COUNCIL ROOM RANDOM TESTS ----------------\n");
	printf("--------------------------------------------------------------------");	

	for (i = 0; i < 15000; i++) {

		/* ---------- BEFORE CARD IS PLAYED ---------- */
		printf("\n\n********** Iteration %d **********\n", i);

		// Random seed for cardEffect()
		// Set up Random() from rngs.c
		// Using numbers from testDrawCard.c
		SelectStream(2);
		int seed = rand() + 1;

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


		// Non-randomized hand / deck counts for currentPlayer
		/*
		int startingHandCount = testState.handCount[currentPlayer];
		printf("currentPlayer startingHandCount: %d\n", startingHandCount);
		*/

		// Number of cards in currentPlayer's hand before playing Council Room
		testState.handCount[currentPlayer] = floor(Random() * MAX_HAND);
		int startingHandCount = testState.handCount[currentPlayer];
		printf("currentPlayer startingHandCount: %d\n", startingHandCount);		

		// Expected number of cards in currentPlayer's hand after playing Council Room
		// Number is 3 because thoguh 4 cards are drawn, the played card is discarded
		int expectedHandCount = startingHandCount + 3;	
		printf("Expected ending handCount: %d\n", expectedHandCount);

		// Number of cards in currentPlayer's deck before playing Council Room	
		testState.deckCount[currentPlayer] = floor(Random() * MAX_DECK);
		int startingDeckCount = testState.deckCount[currentPlayer];
		printf("currentPlayer startingDeckCount: %d\n", startingDeckCount); 

		// Non-random version
		//int startingDeckCount = testState.deckCount[currentPlayer];				

		// Num cards in currentPlayer's discard pile before playing Smithy
        testState.discardCount[currentPlayer] = floor(Random() * MAX_DECK);
        int discCount = testState.discardCount[currentPlayer];
        printf("currentPlayer discardCount: %d\n", testState.discardCount[currentPlayer]);

		// Expected number of cards in currentPlayer's deck after playing Council Room
		int expectedDeckCount;
		if (startingDeckCount >= 4) {
			 expectedDeckCount = startingDeckCount - 4;
		} else {
			expectedDeckCount = startingDeckCount + discCount - 4;
		}
		 	
		printf("Expected ending deckCount: %d\n", expectedDeckCount);

		// Starting / ending handCount and deckCount for other players		
		int otherPlayerStartingHandCount[numPlayers];	
		int otherPlayerStartingDeckCount[numPlayers];
		int otherPlayerExpectedHandCount[numPlayers];	
		int otherPlayerExpectedDeckCount[numPlayers];
		int otherPlayerStartingDiscardCount[numPlayers];

		// Starting handCount and deckCount for the other players - NOT RANDOM
		for (j = 0; j < numPlayers; j++) {
			if (j != currentPlayer) {			
				// discard count
				testState.discardCount[j] = floor(Random() * MAX_DECK);
				otherPlayerStartingDiscardCount[j] = testState.discardCount[j];
				//testState.handCount[j] = rand() % MAX_HAND;
				otherPlayerStartingHandCount[j] = testState.handCount[j]; 
				otherPlayerExpectedHandCount[j] = otherPlayerStartingHandCount[j] + 1;
				//testState.deckCount[j] = rand() % MAX_HAND;
				otherPlayerStartingDeckCount[j] = testState.deckCount[j];
				if (otherPlayerStartingDeckCount[j] > 0) {
					otherPlayerExpectedDeckCount[j] = otherPlayerStartingDeckCount[j] - 1;
				} else {
					otherPlayerStartingDeckCount[j] = otherPlayerStartingDeckCount[j] + otherPlayerStartingDiscardCount[j] - 1;
				}

				printf("Player %d startingHandCount: %d\n", j, otherPlayerStartingHandCount[j]);
				printf("Player %d startingDeckCount: %d\n", j, otherPlayerStartingDeckCount[j]);
				printf("Player %d expected endingHandCount: %d\n", j, otherPlayerExpectedHandCount[j]);
				printf("Player %d expected endingDeckCount: %d\n", j, otherPlayerExpectedDeckCount[j]);				
				printf("Player %d starting discardCount: %d\n", j, testState.discardCount[j]);
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

		/* ---------- PLAY COUNCIL ROOM ---------- */
		cardEffect(council_room, choice1, choice2, choice3, &testState, handPos, &bonus);			

		/* ---------- AFTER CARD IS PLAYED ---------- */

		printf("\n------------------------------ Test 1 -----------------------------\n\n");
		printf("Expected: Current player received exactly 4 cards.\n");
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
		printf("NUM BUYS +1 for currentPlayer\n");
		printf("Expected buys: %d\n", 2);
		printf("Actual: %d\n", testState.numBuys);
		myAssert(testState.numBuys,2);

		if (testState.numBuys == 2) {
			tcPass[t3P] += 1;
		} else {
			tcFail[t3F] += 1;
			iterFailContainer[failCount] = i;
			failCount++;
		}			


		printf("\n------------------------------ Test 4 -----------------------------\n\n");
		printf("Expected: Other players each draw 1 card.\n");

		// Determine the handCount and deckCount for other players
		for (j = 0; j < numPlayers; j++) {
			if (j != currentPlayer) {
				printf("Expected handCount, player %d: %d\n", j, otherPlayerExpectedHandCount[j]);
				printf("Actual handCount: %d\n", testState.handCount[j]);
				myAssert(otherPlayerExpectedHandCount[j], testState.handCount[j]);
				
				if (otherPlayerExpectedHandCount[j] == testState.handCount[j]){
					tcPass[t4aP] += 1;
				} else if (otherPlayerExpectedHandCount[j] != testState.handCount[j]) {
					tcFail[t4aF] += 1;
					iterFailContainer[failCount] = i;
					failCount++;
				}

				printf("Expected deckCount, player %d: %d\n", j, otherPlayerExpectedDeckCount[j]);
				printf("Actual deckCount: %d\n", testState.deckCount[j]);
				myAssert(otherPlayerExpectedDeckCount[j], testState.deckCount[j]);
				
				if (otherPlayerExpectedDeckCount[j] == testState.deckCount[j]){
					tcPass[t4bP] += 1;
				} else if (otherPlayerExpectedDeckCount[j] != testState.deckCount[j]) {
					tcFail[t4bF] += 1;
					iterFailContainer[failCount] = i;
					failCount++;
				}
			}
		}

		printf("\n------------------------------ Test 5 -----------------------------\n\n");
		printf("Expected: No state changes to supplyCount.\n");
		// Determines supply count of all cards
		for (j = curse; j < treasure_map; j++) {
			supplyAfter += testState.supplyCount[j];
		}
		printf("Expected supplyCount: %d\n", supplyBefore);
		printf("Actual supplyCount: %d\n", supplyAfter);
		myAssert(supplyBefore,supplyAfter);

		if (supplyBefore == supplyAfter) {
			tcPass[t5P] += 1;
		} else {
			tcFail[t5F] += 1;
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
		for (i = 0; i < 6; i++) {
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
				case t3F:
					if (tcFail[t3F] != 0) {
						printf("Test 3 Fail count: %d\n", tcFail[t3F]);
					}
					continue;
				case t4aF:
					if (tcFail[t4aF] != 0) {
						printf("Test 4a Fail count: %d\n", tcFail[t4aF]);
					}
					continue;
				case t4bF:
					if (tcFail[t4bF] != 0) {
						printf("Test 4b Fail count: %d\n", tcFail[t4bF]);
					}
					continue;
				case t5F:
					if (tcFail[t5F] != 0) {
						printf("Test 5 Fail count: %d\n", tcFail[t5F]);
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
