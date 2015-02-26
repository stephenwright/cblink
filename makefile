# @file makefile

# name use for output
NAME	= blink

# commands
CC 		= g++
ECHO 	= echo

CFLAGS 	= -std=c++11

# options
OPTS 	= 

app:
	$(CC) $(CFLAGS) -o blink blink.c

