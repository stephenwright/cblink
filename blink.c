
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "gpio.h"

#define LED 16
#define BTN 17

int main(void)
{
	std::cout << "Enter a command, or 'exit' to quit\n";
	std::string cmd;

	GPIOExport(LED);
	GPIOExport(BTN);

	GPIODirection(LED, OUT);
	GPIODirection(BTN, IN);

	while (cmd != "exit") {
		std::cout << "> ";
		std::getline(std::cin, cmd);

		if (cmd == "on")
			GPIOWrite(LED, HIGH);
		else if (cmd == "off")
			GPIOWrite(LED, LOW);
		else if (cmd == "button"){
			GPIOWrite(LED, HIGH);
			while (GPIORead(BTN) == HIGH)
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			GPIOWrite(LED, LOW);
		}
	}

	GPIOUnexport(LED);
	GPIOUnexport(BTN);
	return 0;
}

