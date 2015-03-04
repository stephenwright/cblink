/** @file gpio.cc */

#include "gpio.hh"

#define BUFFER_MAX 3
#define DIRECTION_MAX 35
#define VALUE_MAX 30

GPIO::GPIO(int pin)
: _pin(pin)
{
	Export();
}

GPIO::~GPIO()
{
	Unexport();
}

int
GPIO::Export()
{
	std::lock_guard<std::mutex> guard(_mtx);
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
 
	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return(-1);
	}
 
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", _pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

int
GPIO::Unexport()
{
	std::lock_guard<std::mutex> guard(_mtx);
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
 
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}
 
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", _pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}
 
int
GPIO::Direction(int dir)
{
	std::lock_guard<std::mutex> guard(_mtx);
	static const char s_directions_str[]  = "in\0out";
 
	char path[DIRECTION_MAX];
	int fd;
 
	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", _pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}
 
	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}
 
	close(fd);
	return(0);
}
 
int
GPIO::Read()
{
	std::lock_guard<std::mutex> guard(_mtx);
	char path[VALUE_MAX];
	char value_str[3];
	int fd;
 
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", _pin);
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return(-1);
	}
 
	if (-1 == read(fd, value_str, 3)) {
		fprintf(stderr, "Failed to read value!\n");
		return(-1);
	}
 
	close(fd);
 
	return(atoi(value_str));
}
 
int
GPIO::Write(int value)
{
	std::lock_guard<std::mutex> guard(_mtx);
	static const char s_values_str[] = "01";
 
	char path[VALUE_MAX];
	int fd;
 
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", _pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}
 
	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return(-1);
	}
 
	close(fd);
	return(0);
}

