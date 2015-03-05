/** @file gpio.hh */

#ifndef _GPIO_HH_
#define _GPIO_HH_

#include <string>
#include <fstream>
 
#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1
 
class GPIO
{
public:
	GPIO(int pin);
	~GPIO();

	void Direction(int dir);
	int  Read();
	void Write(int value);

private:
	bool Exists();
	void Export();
	void Unexport();

private:
	int _pin;

	std::fstream _direction;
	std::fstream _value;

	static const std::string PATH_EXPORT;
	static const std::string PATH_UNEXPORT;
	static const std::string PREFIX;
		
};

#endif

