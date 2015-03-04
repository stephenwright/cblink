/** @file gpio.hh */

#ifndef _GPIO_HH_
#define _GPIO_HH_

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mutex>
 
#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1
 
class GPIO
{
public:
	GPIO(int pin);
	~GPIO();

	int Direction(int dir);
	int Read();
	int Write(int value);

private:
	int Export();
	int Unexport();

private:
	int _pin;
	std::mutex _mtx;
		
};

#endif

