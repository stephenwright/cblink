# @file makefile

# name use for output
NAME	= blink

# commands
CC 		= g++
ECHO 	= echo

CFLAGS 	= -std=c++11 -pthread

# options
OPTS 	= 

app:
	$(CC) $(CFLAGS) -o blink blink.cc gpio.cc

debug:
	$(CC) $(CFLAGS) -o blink blink.cc gpio.cc -g

clean:
	rm ./blink

