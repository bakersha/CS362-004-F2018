/*******************************************************
 * Name: 
 * 		randomcardtestadventurer.c
 * Description: 
 * 		Random test suite for adventurer card
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
 * 	1. Draw cards until 2 treasure found.
 *	2. All cards should come from player's own pile.
 * 	3. All drawn cards that are not treasure are discarded.
 * 	4. No state change should occur for other players.
 * 	5. No state change should occur to the supplyCount
 */

enum TCP { 	
	t1P = 0,	// Track the number of pass / fail for Test cases
	t2aP,
	t2bP,
	t2cP,
	t3P
};

enum TCF { 	
	t1F = 0,	// Track the number of pass / fail for Test cases
	t2aF,
	t2bF,
	t2cF,
	t3F,	
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
	int treasureBefore				= 0;	// Count the number of treasure cards in player's hand
	int treasureAfter				= 0;	// Count the number of treasure cards in player's hand
	int copperBefore				= 0;
	int copperAfter					= 0;	
	int silverBefore				= 0;
	int silverAfter					= 0;	
	int goldBefore					= 0;
	int goldAfter					= 0;
	int tcPass[5] 					= {0};	// Holds passed TC count
	int tcFail[5] 					= {0};	// Holds failed TC count
	int iterFailContainer[40000] 	= {0};  // Holds iterations that had failed TCs
	int failCount 	  				= 0;	// Index for iterFailContainer[]	
	int currentPlayer 				= 0;	
	int seed 						= 0;	
	int numPlayers 					= 0;
	int startingHandCount 			= 0;
	int startingDeckCount 			= 0;

    // The variables below came from the cardtest4.c example provided 	
    int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;	
	int k[10] = {adventurer, embargo, village, 
				minion, mine, cutpurse, 
				sea_hag, tribute, smithy, 
				council_room};	

	/* --- START TEST --- */

	printf("\n--------------------------------------------------------------------\n");
	printf("----------------- STARTING ADVENTURER RANDOM TESTS -----------------\n");
	printf("--------------------------------------------------------------------");	

	for (i = 0; i < 15000; i++) {

		/* ---------- BEFORE CARD IS PLAYED ---------- */
		printf("\n\n********** Iteration %d **********\n", i);

		// Random seed for cardEffect()
		// Set up Random() from rngs.c
		// Using numbers from testDrawCard.c
		SelectStream(2);
		seed = rand() + 1;

		printf("Seed: %d\n", seed); 

		// Get a random number of players between MIN and MAX_PLAYERS
		// https://www.geeksforgeeks.org/generating-random-number-range-c/
		numPlayers = (rand() % (MAX_PLAYERS - MIN_PLAYERS + 1)) + MIN_PLAYERS;	
		printf("numPlayers: %d\n", numPlayers);

		// Initialize a game state and player cards
		initializeGame(numPlayers, k, seed, &testState);	

		// Get random currentPlayer 
		currentPlayer = rand() % numPlayers;
		testState.whoseTurn = currentPlayer;
		printf("currentPlayer: %d\n", currentPlayer);

		/********* RANDOMIZATION OF DECK, HAND, DISCARD FROM TESTDRAWCARD *********/

		// Non-randomized hand / deck counts for currentPlayer
		/*
		int startingHandCount = testState.handCount[currentPlayer];
		printf("currentPlayer startingHandCount: %d\n", startingHandCount);
		*/


		// Number of cards in currentPlayer's hand before playing Adventurer
		testState.handCount[currentPlayer] = floor(Random() * MAX_HAND);
		startingHandCount = testState.handCount[currentPlayer];
		printf("currentPlayer startingHandCount: %d\n", startingHandCount);		

		// Expected number of cards in currentPlayer's hand after playing Adventurer
		//int expectedHandCount = startingHandCount + 2;	
		//printf("Expected ending handCount: %d\n", expectedHandCount);

		// Number of cards in currentPlayer's deck before playing Adventurer	
		testState.deckCount[currentPlayer] = floor(Random() * MAX_DECK);
		startingDeckCount = testState.deckCount[currentPlayer];
		printf("currentPlayer startingDeckCount: %d\n", startingDeckCount); 

		// Non-random version
		//int startingDeckCount = testState.deckCount[currentPlayer];				

		// Num cards in currentPlayer's discard pile before playing Smithy
        testState.discardCount[currentPlayer] = floor(Random() * MAX_DECK);
        //int discCount = testState.discardCount[currentPlayer];
        printf("currentPlayer discardCount: %d\n", testState.discardCount[currentPlayer]);

		// Determine num treasure cards in currentPlayer's hand before playing card
        for (j = 0; j < startingHandCount; j++) {
        	if (testState.hand[currentPlayer][j] == copper) {
        		copperBefore++;
        	} else if (testState.hand[currentPlayer][j] == silver) {
        		silverBefore++;
        	} else if (testState.hand[currentPlayer][j] == gold) {
        		goldBefore++;
        	}
        }

        treasureBefore = copperBefore + silverBefore + goldBefore;
        printf("\nTotal treasure in hand before: %d\n", treasureBefore);
        printf("    Copper: %d\n", copperBefore);
        printf("    Silver: %d\n", silverBefore);
        printf("    Gold: %d\n\n", goldBefore);

		int otherPlayerStartingHandCount[numPlayers];	
		int otherPlayerStartingDeckCount[numPlayers];	
		int otherPlayerStartingDiscardCount[numPlayers];		        
		
		// Starting handCount and deckCount for the other players 
		for (j = 0; j < numPlayers; j++) {
			if (j != currentPlayer) {			
				// discard count
				//testState.discardCount[j] = floor(Random() * MAX_DECK);
				otherPlayerStartingDiscardCount[j] = testState.discardCount[j];
				//testState.handCount[j] = floor(Random() * MAX_DECK);
				otherPlayerStartingHandCount[j] = testState.handCount[j]; 
				//testState.deckCount[j] = floor(Random() * MAX_HAND);
				otherPlayerStartingDeckCount[j] = testState.deckCount[j];
				printf("Player %d startingHandCount: %d\n", j, otherPlayerStartingHandCount[j]);
				printf("Player %d starting discardCount: %d\n", j, testState.discardCount[j]);
			} else {
				otherPlayerStartingHandCount[j] = 0;
				otherPlayerStartingDeckCount[j] = 0;
				otherPlayerStartingDiscardCount[j] = 0;			
			}
		} 	

		// Determines supply count of all cards
		for (j = curse; j < treasure_map; j++) {
			supplyBefore += testState.supplyCount[j];
		}
		printf("Starting total supplyCount: %d\n", supplyBefore);

		/* ---------- PLAY COUNCIL ROOM ---------- */
		cardEffect(adventurer, choice1, choice2, choice3, &testState, handPos, &bonus);			

		/* ---------- AFTER CARD IS PLAYED ---------- */

		printf("\n------------------------------ Test 1 -----------------------------\n\n");
		printf("Expected: Current player draws until 2 treasure drawn.\n");
		printf("CurrentPlayer handCount: %d\n", testState.handCount[currentPlayer]);
		printf("CurrentPlayer deckCount: %d\n", testState.deckCount[currentPlayer]);
		printf("CurrentPlayer discardCount: %d\n", testState.discardCount[currentPlayer]);
		// Determine num treasure cards in currentPlayer's hand after playing card
        for (j = 0; j < testState.handCount[currentPlayer]; j++) {
        	if (testState.hand[currentPlayer][j] == copper) {
        		copperAfter++;
        	} else if (testState.hand[currentPlayer][j] == silver) {
        		silverAfter++;
        	} else if (testState.hand[currentPlayer][j] == gold) {
        		goldAfter++;
        	}
        }
        treasureAfter = copperAfter + silverAfter + goldAfter;
        printf("\nTotal treasure in hand after: %d\n", treasureAfter);
        printf("    Copper: %d\n", copperAfter);
        printf("    Silver: %d\n", silverAfter);
        printf("    Gold: %d\n\n", goldAfter);	
        myAssert(treasureAfter,treasureBefore+2);

        if (treasureAfter == treasureBefore+2) {
        	tcPass[t1P] +=1;
        } else {
        	tcFail[t1F] +=1;
			iterFailContainer[failCount] = i;
			failCount++;        	
        }

		printf("\n------------------------------ Test 2 -----------------------------\n\n");
		printf("Expected: No state change to other players\n");		
		// Determine the handCount and deckCount for other players
		for (j = 0; j < numPlayers; j++) {
			if (j != currentPlayer) {
				printf("Expected handCount, player %d: %d\n", j, otherPlayerStartingHandCount[j]);
				printf("Actual handCount: %d\n", testState.handCount[j]);
				myAssert(otherPlayerStartingHandCount[j], testState.handCount[j]);
				
				if (testState.handCount[j] == otherPlayerStartingHandCount[j]){
					tcPass[t2aP] += 1;
				} else if (testState.handCount[j] != otherPlayerStartingHandCount[j]) {
					tcFail[t2aF] += 1;
					iterFailContainer[failCount] = i;
					failCount++;
				}

				printf("Expected deckCount, player %d: %d\n", j, otherPlayerStartingDeckCount[j]);
				printf("Actual deckCount: %d\n", testState.deckCount[j]);
				myAssert(otherPlayerStartingDeckCount[j], testState.deckCount[j]);
				
				if (testState.deckCount[j] == otherPlayerStartingDeckCount[j]){
					tcPass[t2bP] += 1;
				} else if (testState.deckCount[j] != otherPlayerStartingDeckCount[j]) {
					tcFail[t2bF] += 1;
					iterFailContainer[failCount] = i;
					failCount++;
				}

				printf("Expected discardCount, player %d: %d\n", j, otherPlayerStartingDiscardCount[j]);
				printf("Actual discardCount: %d\n", testState.discardCount[j]);
				myAssert(otherPlayerStartingDiscardCount[j], testState.discardCount[j]);
				
				if (testState.discardCount[j] == otherPlayerStartingDiscardCount[j]){
					tcPass[t2cP] += 1;
				} else if (testState.discardCount[j] != otherPlayerStartingDiscardCount[j]) {
					tcFail[t2cF] += 1;
					iterFailContainer[failCount] = i;
					failCount++;
				}
			}
		}
		printf("\n------------------------------ Test 3 -----------------------------\n\n");
		printf("Expected: No state changes to supplyCount.\n");
		// Determines supply count of all cards
		for (j = curse; j < treasure_map; j++) {
			supplyAfter += testState.supplyCount[j];
		}
		printf("Expected supplyCount: %d\n", supplyBefore);
		printf("Actual supplyCount: %d\n", supplyAfter);
		myAssert(supplyBefore,supplyAfter);

		if (supplyBefore == supplyAfter) {
			tcPass[t3P] += 1;
		} else {
			tcFail[t3F] += 1;
			iterFailContainer[failCount] = i;
			failCount++;
		}

		// Reset counters
		supplyBefore = 0;
		supplyAfter = 0;
		treasureBefore = 0;	
		treasureAfter = 0;	
		copperBefore = 0;
		copperAfter = 0;	
		silverBefore = 0;
		silverAfter = 0;	
		goldBefore = 0;
		goldAfter = 0;		
	}
	printf("\n--------------------------------------------------------------------\n");
	printf("------------------------- TESTING COMPLETE -------------------------\n");
	printf("--------------------------------------------------------------------\n\n");	
	
	if (failCount == 0) {
		printf("ALL TESTS PASSED!\n\n");
	} else {
		for (i = 0; i < 6; i++) {
			switch(i) {
				case t1F:
					if (tcFail[t1F] != 0) {
						printf("Test 1 Fail count: %d\n", tcFail[t1F]);
					}
					continue;
				case t2aF:
					if (tcFail[t2aF] != 0) {
						printf("Test 2a Fail count: %d\n", tcFail[t2aF]);
					}
					continue;
				case t2bF:
					if (tcFail[t2bF] != 0) {
						printf("Test 2b Fail count: %d\n", tcFail[t2bF]);
					}
					continue;
				case t2cF:
					if (tcFail[t2cF] != 0) {
						printf("Test 2c Fail count: %d\n", tcFail[t2cF]);
					}
					continue;
				case t3F:
					if (tcFail[t3F] != 0) {
						printf("Test 3 Fail count: %d\n", tcFail[t3F]);
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
	printf("\n");

	return 0;
}
