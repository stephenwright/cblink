/** @file blink.cc */

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include "gpio.hh"

#define LED 16
#define BTN 17

bool running = true;
std::mutex mtx;
GPIO led(LED);
GPIO btn(BTN);
int buttonState = HIGH;

bool is_running()
{
	std::lock_guard<std::mutex> guard(mtx);
	return running;
}

void set_running(bool val)
{
	std::lock_guard<std::mutex> guard(mtx);
	running = val;
}

void button_listen()
{
	while(is_running())
	{
		if (btn.Read() == LOW)
			led.Write(HIGH);
		else
			led.Write(LOW);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main(void)
{
	std::cout << "Enter a command, or 'exit' to quit\n";
	std::string cmd;

	led.Direction(OUT);
	btn.Direction(IN);

	std::thread t_input(button_listen);

	while (cmd != "exit") 
	{
		std::cout << "> ";
		std::getline(std::cin, cmd);

		if (cmd == "on")
			led.Write(HIGH);
		else if (cmd == "off")
			led.Write(LOW);
	}

	set_running(false);
	t_input.join();

	return 0;
}

