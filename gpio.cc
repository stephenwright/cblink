/** @file gpio.cc */

#include <exception>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "gpio.hh"

const std::string GPIO::PATH_EXPORT 	= "/sys/class/gpio/export";
const std::string GPIO::PATH_UNEXPORT 	= "/sys/class/gpio/unexport";
const std::string GPIO::PREFIX 			= "/sys/class/gpio/gpio";

GPIO::GPIO(int pin)
: _pin(pin)
{
	Export();

	std::stringstream directionPath;
	std::stringstream valuePath;

	directionPath << PREFIX << _pin << "/direction";
	valuePath << PREFIX << _pin << "/value";

	_direction.open(directionPath.str(), std::ios::out);
	_value.open(valuePath.str());
}

GPIO::~GPIO()
{
	Unexport();
}

bool
GPIO::Exists()
{
	std::stringstream path;
	path << PREFIX << _pin;

	std::fstream fs;
	fs.open(path.str());
	return fs.good();
}

void
GPIO::Export()
{
	if (Exists())
		return;

	std::fstream fs;
	std::stringstream ss;

	ss << _pin;
	fs.open(PATH_EXPORT, std::ios::out);
	fs << ss.str();
}

void
GPIO::Unexport()
{
	if (!Exists())
		return;

	std::fstream fs;
	std::stringstream ss;

	ss << _pin;
	fs.open(PATH_UNEXPORT, std::ios::out);
	fs << ss.str();
}
 
void
GPIO::Direction(int dir)
{
	_direction.seekp(0);
	_direction << (dir == IN ? "in" : "out") << std::endl;

	if (!_direction.good())
		throw std::runtime_error("Error setting direction.");
}
 
int
GPIO::Read()
{
	std::string value;
	_value.seekg(0);
	_value >> value;

	if (value == "0") return LOW;
	if (value == "1") return HIGH;

	throw std::logic_error("Invalid GPIO value.");
}
 
void
GPIO::Write(int value)
{
	if (value != HIGH && value != LOW)
		throw std::logic_error("Invalid GPIO value.");

	_value.seekg(0);
	_value << (value == HIGH ? "1" : "0") << std::endl;

	if (!_value.good())
		throw std::runtime_error("Error writing value.");
}

