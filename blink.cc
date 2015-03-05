/** @file blink.cc */

#include <exception>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include "gpio.hh"

#define LED 16
#define BTN 17

bool running = true;
std::mutex mtx;

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
	GPIO led(LED);
	GPIO btn(BTN);
	int buttonState = btn.Read();
	int newState;
	while(is_running())
	{
		newState = btn.Read();	
		if (newState != buttonState)
		{
			// button state changed, fire event
			if (newState == LOW)
				led.Write(HIGH);
			else
				led.Write(LOW);
		
			buttonState = newState;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

int main()
{
	std::cout << "Enter a command, or 'exit' to quit\n";
	std::string cmd;

	std::thread t_input(button_listen);

	try
	{
		GPIO led(LED);
		GPIO btn(BTN);

		led.Direction(OUT);
		btn.Direction(IN);

		while (cmd != "exit") 
		{
			std::cout << "> ";
			std::getline(std::cin, cmd);

			if (cmd == "on")
				led.Write(HIGH);
			else if (cmd == "off")
				led.Write(LOW);
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
	}

	set_running(false);
	t_input.join();

	return 0;
}

