#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

// Produces a random ascii char in the range of the target statement
char inputChar()
{
	// A simple random range can be generated with the following:
	// (rand() %(max + 1 - min)) + min
	// This can produce a biased result, but should work with int values
	// https://stackoverflow.com/questions/2509679/how-to-generate-a-random-integer-number-from-within-a-range
	
	// Lowest ascii Dec for target is a ' ' which is 32 (min)
	// Highest ascii Dec for target is '}' which is 125 (max)
	char randChar = (rand() % (125 + 1 - 32)) + 32;

	return randChar;
}


// Produces a random string of 6 char length
char *inputString()
{
	int i;				// Index for loop
	char randChar;		// Random char to push into array
	char randStr[6];	// Char array to hold rand chars
	
	memset(randStr,'\0',6);	// Fully initalize the string

	// Use same formula to get a range of random chars
	// Min is 'e' = ascii Dec 101
	// Max is 't' = ascii Dec 116
	// The NULL terminator should be at the end of the array anyway
	// So, loop through the other elements and place random chars
	// But force the NULL to be the final element
	for(i = 0; i < sizeof(randStr)-1; i++) {
		randChar = (rand() % (116 + 1 - 101)) + 101;
		randStr[i] = randChar;
	}
	
	randStr[5] = '\0';

	return randStr;	
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
	testme();
    return 0;
}
