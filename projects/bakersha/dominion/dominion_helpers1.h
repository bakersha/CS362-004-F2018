#ifndef _DOMINION_HELPERS_H
#define _DOMINION_HELPERS_H

#include "dominion.h"

int drawCard(int player, struct gameState *state);
int updateCoins(int player, struct gameState *state, int bonus);
int discardCard(int handPos, int currentPlayer, struct gameState *state, 
		int trashFlag);
int gainCard(int supplyPos, struct gameState *state, int toFlag, int player);
int getCost(int cardNumber);
int cardEffect(int card, int choice1, int choice2, int choice3, 
	       struct gameState *state, int handPos, int *bonus);

// Adding definitions for refactored card code
int adventurerRefactor(int currentPlayer, int temphand[], int z, struct gameState *state);
int council_roomRefactor(int currentPlayer, int handPos, struct gameState *state);
int smithyRefactor(int currentPlayer, int handPos, struct gameState *state);
int villageRefactor(int currentPlayer, int handPos, struct gameState *state);
int stewardRefactor(int currentPlayer, int choice1, int choice2, int choice3, int handPos, struct gameState *state);


#endif
