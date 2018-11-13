/*******************************************************
 * Name: 
 * 		unittest1.c
 * Description: 
 * 		Unit test suite for getCost() function
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

int main(){

	/* Variables */
	
	int testCardNum[29]; // An array to hold all card types plus 3 extra test numbers 
	int i; 				 // For loop counter
	int testCost;		 // The expected cost of each card

	// Array declaration
	for (i = 0; i < 27; i++) // 27 because there are 27 cards the other numbers will be set later
	{
  		testCardNum[i]=i;
  	}

	/* Test 1: Verify return value of every card 
	 * I used the ENUM values to verify that the correct number assignments are being used
	 */
	printf("\n------------------------ getCost() Unit Tests ----------------------\n");
	
	printf("\n-------------------------- Starting Test 1 -------------------------\n");
	printf("------ Verifying the return values for every card that exists ------\n");
	printf("--------------------------------------------------------------------\n\n");

	for (i = 0; i < 27; i++)
	{
		if (testCardNum[i] == curse || testCardNum[i] == copper)
		{
			testCost=0;
		}
		else if (testCardNum[i] == estate || testCardNum[i] == embargo)
		{
			testCost=2;
		}
		else if (testCardNum[i] == silver || testCardNum[i] == village || testCardNum[i] == great_hall || 
			testCardNum[i] == steward || testCardNum[i] == ambassador)
		{
			testCost=3;
		}
		else if (testCardNum[i] == feast || testCardNum[i] == gardens || testCardNum[i] == remodel || 
			testCardNum[i] == smithy || testCardNum[i] == baron || testCardNum[i] == cutpurse || 
			testCardNum[i] == salvager || testCardNum[i] == sea_hag || testCardNum[i] == treasure_map)
		{
			testCost=4;
		}
		else if (testCardNum[i] == duchy || testCardNum[i] == council_room || testCardNum[i] == mine || 
			testCardNum[i] == minion || testCardNum[i] == tribute || testCardNum[i] == outpost)
		{
			testCost=5;
		}
		else if (testCardNum[i] == gold || testCardNum[i] == adventurer)
		{
			testCost=6;
		}
		else if (testCardNum[i] == province)
		{
			testCost=8;
		}
		printf("Cost of card# %d is %d, expected is %d\n", testCardNum[i], getCost(testCardNum[i]), testCost);
		assert(getCost(testCardNum[i]) == testCost);
	};

	/* Test 2: Check for card type that does not exist */
	printf("\n-------------------------- Starting Test 2 -------------------------\n");
	printf("------ Verifying the return values for cards that don't exist ------\n");
	printf("--------------------------------------------------------------------\n\n");

	testCost=-1;		// Expecting all results to be -1 because they are invalid cards
	testCardNum[27]=-1; // Lower boundary
	printf("Cost of card# %d is %d, expected is %d\n", testCardNum[27], getCost(testCardNum[27]), testCost);
	assert(getCost(testCardNum[27]) == testCost);

	testCardNum[28]=27;	// Upper boundary
	printf("Cost of card# %d is %d, expected is %d\n", testCardNum[28], getCost(testCardNum[28]), testCost);
	assert(getCost(testCardNum[28]) == testCost);	

	testCardNum[29]=0.5;
	printf("The input card number is 0.5. Int should truncate to 0.\n");
	printf("Cost of card# %d is %d, expected is 0\n", testCardNum[29], getCost(testCardNum[29]));
	assert(getCost(testCardNum[29]) == 0);	


	/* Test 3: Check return values for card out of order from list and duplicated */
	printf("\n-------------------------- Starting Test 3 -------------------------\n");
	printf("-- Verifying return values when listed out of order or duplicated --\n");
	printf("--------------------------------------------------------------------\n\n");

	testCardNum[0]=copper;			// Cost: 0
	testCardNum[1]=adventurer;		// Cost: 6
	testCardNum[2]=treasure_map;	// Cost: 4
	testCardNum[3]=smithy; 			// Cost: 4
	testCardNum[4]=province;		// Cost: 8
	testCardNum[5]=adventurer;		// Cost: 6

	for (i = 0; i < 6; i++)
	{
		if (testCardNum[i] == curse || testCardNum[i] == copper)
		{
			testCost=0;
		}
		else if (testCardNum[i] == estate || testCardNum[i] == embargo)
		{
			testCost=2;
		}
		else if (testCardNum[i] == silver || testCardNum[i] == village || testCardNum[i] == great_hall || 
			testCardNum[i] == steward || testCardNum[i] == ambassador)
		{
			testCost=3;
		}
		else if (testCardNum[i] == feast || testCardNum[i] == gardens || testCardNum[i] == remodel || 
			testCardNum[i] == smithy || testCardNum[i] == baron || testCardNum[i] == cutpurse || 
			testCardNum[i] == salvager || testCardNum[i] == sea_hag || testCardNum[i] == treasure_map)
		{
			testCost=4;
		}
		else if (testCardNum[i] == duchy || testCardNum[i] == council_room || testCardNum[i] == mine || 
			testCardNum[i] == minion || testCardNum[i] == tribute || testCardNum[i] == outpost)
		{
			testCost=5;
		}
		else if (testCardNum[i] == gold || testCardNum[i] == adventurer)
		{
			testCost=6;
		}
		else if (testCardNum[i] == province)
		{
			testCost=8;
		}

		printf("Cost of card# %d is %d, expected is %d\n", testCardNum[i], getCost(testCardNum[i]), testCost);
		assert(getCost(testCardNum[i]) == testCost);
	}

	printf("\n--------------------------------------------------------------------\n");
	printf("---------------- ALL TESTS PASSED! TESTING COMPLETE ----------------\n");
	printf("--------------------------------------------------------------------\n\n");	

	return 0;	

}