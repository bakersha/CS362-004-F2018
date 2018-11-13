/*******************************************************
 * Name: 
 * 		unittest3.c
 * Description:
 *		unit test suite for supplyCount() function
 * Author:
 *		Sharaya Baker (bakersha) - October 28, 2018
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
	struct gameState testState;	
	int testCardNum[29]; // An array to hold all card types plus 3 extra test numbers 
	int i;

	// Array declaration
	for (i = 0; i < 29; i++)
	{
		testCardNum[i]=i;
	}

	printf("\n---------------------- supplyCount() Unit Tests --------------------\n");
	
	printf("\n-------------------------- Starting Test 1 -------------------------\n");
	printf("--------------- Test for all cards at various amounts --------------\n");
	printf("--------------------------------------------------------------------\n\n");	

	for (i = 0; i < 26; i++)
	{
		if (testCardNum[i] % 2 == 0)
		{
			testState.supplyCount[i]=2;	
			printf("Supply count for card %d. Expected: 2, actual: %d\n", testCardNum[i], supplyCount(testCardNum[i], &testState));
			assert(supplyCount(testCardNum[i], &testState) == 2);
		}
		else if (testCardNum[i] % 3 == 0)
		{
			testState.supplyCount[i]=3;		
			printf("Supply count for card %d. Expected: 3, actual: %d\n", testCardNum[i], supplyCount(testCardNum[i], &testState));
			assert(supplyCount(testCardNum[i], &testState) == 3);		
		}
		else if (testCardNum[i] % 5 == 0)
		{
			testState.supplyCount[i]=5;		
			printf("Supply count for card %d. Expected: 5, actual: %d\n", testCardNum[i], supplyCount(testCardNum[i], &testState));
			assert(supplyCount(testCardNum[i], &testState) == 5);	
		}		
		else
		{
			testState.supplyCount[i]=1;		
			printf("Supply count for card %d. Expected: 1, actual: %d\n", testCardNum[i], supplyCount(testCardNum[i], &testState));
			assert(supplyCount(testCardNum[i], &testState) == 1);
		}			
	}

	printf("\n-------------------------- Starting Test 2 -------------------------\n");
	printf("----------------- Test for cards that do not exist -----------------\n");
	printf("--------------------------------------------------------------------\n\n");	

	testCardNum[27]=26; // Boundary
	testState.supplyCount[27]=5;
	printf("Supply count for card %d. Expected: 0, actual: %d\n", testCardNum[27], supplyCount(testCardNum[i], &testState));	
	assert(supplyCount(testCardNum[i], &testState) == 0);

	testCardNum[28]=-1; // Boundary
	testState.supplyCount[28]=5;
	printf("Supply count for card %d. Expected: 0, actual: %d\n", testCardNum[28], supplyCount(testCardNum[i], &testState));	
	assert(supplyCount(testCardNum[i], &testState) == 0);

	testCardNum[29]=45; // Far outside boundary
	testState.supplyCount[29]=5;
	printf("Supply count for card %d. Expected: 0, actual: %d\n", testCardNum[29], supplyCount(testCardNum[i], &testState));	
	assert(supplyCount(testCardNum[i], &testState) == 0);

	printf("\n--------------------------------------------------------------------\n");
	printf("---------------- ALL TESTS PASSED! TESTING COMPLETE ----------------\n");
	printf("--------------------------------------------------------------------\n\n");	

	return 0;

}
