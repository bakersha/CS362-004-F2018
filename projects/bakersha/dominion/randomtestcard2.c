/*
 Helpful Tips:

 An overview of the random generator structure:
 1. Identify the method under test/ (CUT)
 2. Identify all the dependencies (parameters)
 3. Write code to generate random inputs for the chosen method
 4. Invoke the method (execute the method)
 5. Check if stopping criterion (like time or number of loops) 
	is not satisfied go to step 2.
 
 Note: steps 3:
 a. If the input is a primitive data type, generate a random 
	primitive value, etc.
 b. If the input is an array, create an array and initialize it 
	with some random values, etc.
 c. Try to “stay random” but shift the probability space 
	(e.g., if you choose int numCoppersInDeck = rand() % 20 and int 
	numAdventurersToPlay = rand() % 10. There must be a logical 
	reason for choosing 20 and 10). You might look at the coverage 
	and the code and you see some numbers, or you look at the coverage 
	and you see that you need to choose such values to cover a 
	particular code!
 d. You also need to improve your oracles (step 5) (i.e., assertions 
	"if/print in our case") until you feel that all the problems that 
	should be caught actually are caught! As you can see it is up to you 
	whether you want to produce a “rock solid” random generator or just 
	a random generator.
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

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


// Test oracle
void myAssert(int x, int y) {
	if(x == y) {
		printf("TEST PASSED!\n");
	} else {
		printf("TEST FAILED!\n");
	}
}


/*

int council_roomRefactor(int currentPlayer, int handPos, struct gameState *state)
{
  int i;
  //+4 Cards
  for (i = 0; i < 4; i++)
  {
    drawCard(currentPlayer, state);
  } 

  //+1 Buy
  state->numBuys++;
    
  //Each other player draws a card
  for (i = 0; i < state->numPlayers; i++)
  {
    if ( i != currentPlayer )
      {
        drawCard(i, state);
      }
  }
  
  //put played card in played card pile
  discardCard(handPos, currentPlayer, state, 0);
  
  return 0;  
}
*/

int main() {

	srand(time(NULL)); // seed rand

	/* --- Variables ---  */
	struct gameState testState;	// New instance of gameState
	int i, j, n;				// Counters
	int victoryBefore = 0;		// Used to verify no state change occurs
	int victoryAfter  = 0; 		// Used to verify no state change occurs
	int kingdomBefore = 0; 		// Used to verify no state change occurs
	int kingdomAfter  = 0;		// Used to verify no state change occurs
	int numT1P  	  = 0; 		// Track the number of pass / fail for Test cases
	int numT1F  	  = 0;		
	int numT2P  	  = 0;			
	int numT2F  	  = 0;	
	int numT3P  	  = 0;			
	int numT3F  	  = 0;	
	int numT4aP 	  = 0;
	int numT4aF 	  = 0;	
	int numT4bP 	  = 0;
	int numT4bF 	  = 0;
	int numT5P  	  = 0;
	int numT5F  	  = 0;	
    int numT6aP 	  = 0;
    int numT6aF 	  = 0;	
    int numT6bP 	  = 0;
    int numT6bF 	  = 0;	
	int failCount 	  = 0;
	int iterFailContainer[100] = {0};

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

	for (i = 1; i < 11; i++) {

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

		// Number of cards in currentPlayer's hand before playing Council Room
		testState.handCount[currentPlayer] = rand() % MAX_HAND;
		int startingHandCount = testState.handCount[currentPlayer];
		printf("currentPlayer startingHandCount: %d\n", startingHandCount);
		// Expected number of cards in currentPlayer's hand after playing Council Room
		// Number is 3 because thoguh 4 cards are drawn, the played card is discarded
		int expectedHandCount = startingHandCount + 3;	
		printf("Expected ending handCount: %d\n", expectedHandCount);
		// Number of cards in currentPlayer's deck before playing Council Room	
		testState.deckCount[currentPlayer] = rand() % MAX_DECK;
		int startingDeckCount = testState.deckCount[currentPlayer];
		printf("currentPlayer startingDeckCount: %d\n", startingDeckCount); 
		//Expected number of cards in currentPlayer's deck after playing Council Room


/******************* MAKE THE HAND AND DECK COUNT CHANGES TO SMITHY ********************/


		int expectedDeckCount = startingDeckCount - 4; 	
		printf("Expected ending deckCount: %d\n", expectedDeckCount);

		// Starting / ending handCount and deckCount for other players		
		int otherPlayerStartingHandCount[numPlayers-1];	
		int otherPlayerStartingDeckCount[numPlayers-1];
		int otherPlayerExpectedHandCount[numPlayers-1];	
		int otherPlayerExpectedDeckCount[numPlayers-1];
		//int otherPlayerEndingHandCount[numPlayers-1];	
		//int otherPlayerEndingDeckCount[numPlayers-1];		

		// Randomize the starting handCount and deckCount for the other players
		for (j = 0; j < numPlayers; j++) {
			if (j != currentPlayer) {			
				testState.handCount[j] = rand() % MAX_HAND;
				otherPlayerStartingHandCount[j] = testState.handCount[j]; 
				otherPlayerExpectedHandCount[j] = otherPlayerStartingHandCount[j] + 1;
				testState.deckCount[j] = rand() % MAX_HAND;
				otherPlayerStartingDeckCount[j] = testState.deckCount[j];
				otherPlayerExpectedDeckCount[j] = otherPlayerStartingDeckCount[j] - 1;
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

		// This determines the supply of victory cards before Council Room is played
		for ( j = estate; j < copper; j++ ) {
			victoryBefore += testState.supplyCount[j];
		}

		// This determines the supply of kingdom cards before Council Room is played 	
		for ( j = 0; j < 10; j++ ) {
			n = k[j];
			kingdomBefore += testState.supplyCount[n];
		}

		printf("Starting victory pile: %d\nStarting kingdom pile: %d\n", victoryBefore, kingdomBefore);

		/* ---------- PLAY COUNCIL ROOM ---------- */
		cardEffect(council_room, choice1, choice2, choice3, &testState, handPos, &bonus);
						

		/* ---------- AFTER CARD IS PLAYED ---------- */

		printf("\n------------------------------ Test 1 -----------------------------\n\n");
		printf("Expected: Current player received exactly 4 cards.\n");
		printf("Expected handCount: %d\n", expectedHandCount);
		printf("Actual: %d\n", testState.handCount[currentPlayer]);
		myAssert(testState.handCount[currentPlayer], expectedHandCount);

		if (testState.handCount[currentPlayer] == expectedHandCount){
			numT1P++;
		} else if (testState.handCount[currentPlayer] != expectedHandCount) {
			numT1F++;
			iterFailContainer[failCount] = i;
			failCount++;
		}

		printf("\n------------------------------ Test 2 -----------------------------\n\n");
		printf("Expected: currentPlayer deckCount = %d\n", expectedDeckCount);
		printf("Actual: currentPlayer deckCount = %d\n",testState.deckCount[currentPlayer]);
		myAssert(testState.deckCount[currentPlayer], expectedDeckCount);

		if (testState.deckCount[currentPlayer] == expectedDeckCount){
			numT2P++;
		} else if (testState.deckCount[currentPlayer] != expectedDeckCount) {
			numT2F++;
			iterFailContainer[failCount] = i;
			failCount++;
		}

		printf("\n------------------------------ Test 3 -----------------------------\n\n");
		printf("NUM BUYS +1 *** UNDER CONSTRUCTION ***\n");
		// WRITE THIS


		printf("\n------------------------------ Test 4 -----------------------------\n\n");
		printf("Expected: Other players each draw 1 card.\n");

		// Determine the handCount and deckCount for other players
		for (j = 0; j < numPlayers; j++) {
			if (j != currentPlayer) {
				printf("Expected handCount, player %d: %d\n", j, otherPlayerExpectedHandCount[j]);
				printf("Actual handCount: %d\n", testState.handCount[j]);
				myAssert(otherPlayerExpectedHandCount[j], testState.handCount[j]);
				
				if (otherPlayerExpectedHandCount[j] == testState.handCount[j]){
					numT4aP++;
				} else if (otherPlayerExpectedHandCount[j] != testState.handCount[j]) {
					numT4aF++;
					iterFailContainer[failCount] = i;
					failCount++;
				}

				printf("Expected deckCount, player %d: %d\n", j, otherPlayerExpectedDeckCount[j]);
				printf("Actual deckCount: %d\n", testState.deckCount[j]);
				myAssert(otherPlayerExpectedDeckCount[j], testState.deckCount[j]);
				
				if (otherPlayerExpectedDeckCount[j] == testState.deckCount[j]){
					numT4bP++;
				} else if (otherPlayerExpectedDeckCount[j] != testState.deckCount[j]) {
					numT4bF++;
					iterFailContainer[failCount] = i;
					failCount++;
				}
			}
		}

		printf("\n------------------------------ Test 5 -----------------------------\n\n");
		printf("Expected: Played card moved to discard pile. *** UNDER CONSTRUCTION ***\n");
		// Need to write
			// get starting discard pile amount before card is played
			// compare to new discard pile amount
			// if 1:1 , pass
			// should add this to smithy tests



		printf("\n------------------------------ Test 6 -----------------------------\n\n");
		printf("Expected: No state changes to victory or kingdom piles.\n");

		/* This determines the supply of victory cards before Smithy is played */
		for ( j = estate; j < copper; j++ ){
			victoryAfter += testState.supplyCount[j];
		}

		printf("Expected Victory pile: %d\n", victoryBefore);
		printf("Actual Victory pile: %d\n", victoryAfter);
		myAssert(victoryBefore, victoryAfter);

		if (victoryBefore == victoryAfter) {
			numT6aP++;
		} else if (victoryBefore != victoryAfter) {
			numT6aF++;
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
			numT6bP++;
		} else if (kingdomBefore != kingdomAfter) {
			numT6bF++;
			iterFailContainer[failCount] = i;
			failCount++;
		}		

	}

	printf("------------------ TOTAL PASSED / FAILED TEST CASES ------------------\n\n");
	printf("Test 1  - PASSED: %d\n", numT1P);
	printf("Test 1  - FAILED: %d\n", numT1F);
	printf("Test 2  - PASSED: %d\n", numT2P);
	printf("Test 2  - FAILED: %d\n", numT2F);
	printf("Test 3  - PASSED: %d\n", numT3P);
	printf("Test 3  - FAILED: %d\n", numT3F);	
	printf("Test 4a - PASSED: %d\n", numT4aP);
	printf("Test 4a - FAILED: %d\n", numT4aF);	
	printf("Test 4b - PASSED: %d\n", numT4bP);
	printf("Test 4b - FAILED: %d\n", numT4bF);
	printf("Test 5  - PASSED: %d\n", numT5P);
	printf("Test 5  - FAILED: %d\n", numT5F);
	printf("Test 6a - PASSED: %d\n", numT6aP);
	printf("Test 6a - FAILED: %d\n", numT6aF);	
	printf("Test 6b - PASSED: %d\n", numT6bP);
	printf("Test 6b - FAILED: %d\n", numT6bF);		


	printf("\nITERATIONS WHICH CONTAINED FAILURES -- NOT TEST 1 OR TEST 2:\n");
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