/*******************************************************
 * Name: 
 * 		unittest3.c
 * Description:
 *		unit test suite for whoseTurn() function
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

int main()
{
	/* Variables */
	struct gameState testState;	
	int i;

	printf("\n---------------------- whoseTurn() Unit Tests --------------------\n");	

	printf("\n--------------------------- Starting Test --------------------------\n");
	printf("------- Verify correct number is returned when player changes ------\n");
	printf("--------------------------------------------------------------------\n\n");	

	for ( i = 0; i < MAX_PLAYERS; i++ )
	{
		testState.whoseTurn=i;
		printf("Current player expected: %d, current player returned: %d\n", i, whoseTurn(&testState));
		assert(whoseTurn(&testState)==i);
	}

	printf("\n--------------------------------------------------------------------\n");
	printf("---------------- ALL TESTS PASSED! TESTING COMPLETE ----------------\n");
	printf("--------------------------------------------------------------------\n\n");	

	return 0;
}