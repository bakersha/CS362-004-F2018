/*******************************************************
 * Name: 
 * 		unittest2.c
 * Description:
 *		unit test suite for isGameOver() function
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
	int i;

	/* 
	 * Test 1: 
	 * Test various input values for the first "if" statement
	 * Values: 
	 *		  0 (expected), 
	 *		 -1 (boundary),
	 * 		 .5 (non int value), 
	 *		  1 (boundary), 
	 *      501 (one above the max deck amount)
	 */
	printf("\n---------------------- isGameOver() Unit Tests ---------------------\n");	
	
	printf("\n-------------------------- Starting Test 1 -------------------------\n");
	printf("------- Test various input values for the first if statement -------\n");
	printf("--------------------------------------------------------------------\n\n");	

	// Set all of the supply counts to 5 in order to test only the first if statement
	for (i = 0; i < 25; i++)
	{
		testState.supplyCount[i]=5;
	}

	testState.supplyCount[province]=0; // expected exit
	printf("Test input for province supply: %d, expected rtn val: 1, actual: %d\n", testState.supplyCount[province], isGameOver(&testState));
	assert(isGameOver(&testState)==1);

	testState.supplyCount[province]=-1; // boundary
	printf("Test input for province supply: %d, expected rtn val: 0, actual: %d\n", testState.supplyCount[province], isGameOver(&testState));
	assert(isGameOver(&testState)==0);	

	testState.supplyCount[province]=0.5; // expected to behave as if 0
	printf("Test input for province supply: 0.5, expected rtn val: 1, actual: %d\n", isGameOver(&testState));
	assert(isGameOver(&testState)==1);		

	testState.supplyCount[province]=1; // should not exit
	printf("Test input for province supply: %d, expected rtn val: 0, actual: %d\n", testState.supplyCount[province], isGameOver(&testState));
	assert(isGameOver(&testState)==0);	

	testState.supplyCount[province]=501; // should not exit
	printf("Test input for province supply: %d, expected rtn val: 0, actual: %d\n", testState.supplyCount[province], isGameOver(&testState));
	assert(isGameOver(&testState)==0);		

	/* 
	 * Test 2: 
	 * Test 0 supply count for various numbers of cards and card types
	 * 		None
	 * 		First 2
	 *		First 3
	 *		All
	 *		Last 2
	 *		Last 3
	 *		3 cards in the "middle"
	 */
	printf("\n-------------------------- Starting Test 2 -------------------------\n");
	printf("--- Test supply count for various numbers of cards and card types --\n");
	printf("--------------------------------------------------------------------\n\n");

	// With all supplies set to 5, the function should return 0
	testState.supplyCount[province]=5;
	printf("All supplyCount set to 5, expected rtn val: 0, actual: %d\n", isGameOver(&testState));	
	assert(isGameOver(&testState)==0);

	// Set first 2 supply counts to 0, function should return 0 (boundary)
	for (i = 0; i < 2; i++)
	{
		testState.supplyCount[i]=0;
	}
	printf("First 2 supplyCount set to 0, expected rtn val: 0, actual: %d\n", isGameOver(&testState));	
	assert(isGameOver(&testState)==0);	

	// Set the first 3 supply counts to 0, function should return 1
	for (i = 0; i < 3; i++)
	{
		testState.supplyCount[i]=0;
	}
	printf("First 3 supplyCount set to 0, expected rtn val: 1, actual: %d\n", isGameOver(&testState));	
	assert(isGameOver(&testState)==1);

	// Set all supply counts to 0
	for (i = 0; i < 25; i++)
	{
		testState.supplyCount[i]=0;
	}	
	printf("All supplyCount set to 0, expected rtn val: 1, actual: %d\n", isGameOver(&testState));	
	assert(isGameOver(&testState)==1);		

	// Set all supply counts back to 5, then change the last two to 0 (boundary)
	for (i = 0; i < 25; i++)
	{
		testState.supplyCount[i]=5;
	}
	testState.supplyCount[23]=0;
	testState.supplyCount[24]=0;
	printf("Last 2 supplyCount set to 0, expected rtn val: 0, actual: %d\n", isGameOver(&testState));	
	assert(isGameOver(&testState)==0);	

	// Change the last 3 supply counts to 0
	testState.supplyCount[22]=0;
	printf("Last 3 supplyCount set to 0, expected rtn val: 1, actual: %d\n", isGameOver(&testState));	
	assert(isGameOver(&testState)==1);	

	/* Set all supply counts back to 5, then change three in the middle to verify the function
	 * is not dependent on the first or last part of the array in order to count correctly
	 */
	for (i = 0; i < 25; i++)
	{
		testState.supplyCount[i]=5;
	}
	testState.supplyCount[5]=0;
	testState.supplyCount[13]=0;
	testState.supplyCount[21]=0;	
	printf("3 middle supplyCount set to 0, expected rtn val: 1, actual: %d\n", isGameOver(&testState));	
	assert(isGameOver(&testState)==1);	

	printf("\n--------------------------------------------------------------------\n");
	printf("---------------- ALL TESTS PASSED! TESTING COMPLETE ----------------\n");
	printf("--------------------------------------------------------------------\n\n");	

	return 0;
}