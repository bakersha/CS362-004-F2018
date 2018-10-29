####### MAKEFILE FOR TESTME.C - RANDOM TEST QUIZ #######
CC=gcc
CFLAGS=-g -Wall

TARGET=testme

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o $(TARGET)	
