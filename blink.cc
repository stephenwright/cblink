/** @file blink.cc */

#include <exception>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include "gpio.hh"

#define LED 16
#define BTN 17

// Command message queue
// ============================================================================

std::mutex mtx;
std::queue<std::string> message_queue;

void 
q_push(std::string msg)
{
	std::lock_guard<std::mutex> guard(mtx);
	message_queue.push(msg);
}

std::string 
q_pop()
{
	std::lock_guard<std::mutex> guard(mtx);
	if (message_queue.empty())
		return "";

	std::string msg = message_queue.front();
	message_queue.pop();
	return msg;
}

// ============================================================================

bool is_running = true;
bool do_blink = false;

/*
 * Continuously blink the LED until instructive otherwise
 */
void 
blink()
{
	// ignore if already blinking
	if (do_blink)
		return;

	do_blink = true;
	bool is_on = false;

	while (do_blink)
	{
		q_push((is_on = !is_on) ? "on" : "off");
		std::this_thread::sleep_for(std::chrono::milliseconds(800));
	}

	if (is_on)
		q_push("off");
}

/*
 * Listen for button presses
 */
void 
button_listen()
{
	GPIO btn(BTN);
	btn.Direction(IN);

	int buttonState = btn.Read();
	int newState;
	while(is_running)
	{
		newState = btn.Read();	
		if (newState != buttonState)
		{
			// button state changed, fire event
			if (newState == LOW)
				q_push("on");
			else
				q_push("off");
		
			buttonState = newState;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

/*
 * Command prompt for user input
 */
void 
prompt()
{
	std::cout << "Enter a command, or 'exit' to quit\n";
	std::string cmd;

	while (is_running) 
	{
		std::cout << "> ";
		std::getline(std::cin, cmd);

		if (cmd == "exit" || cmd == "quit" || cmd == "q")
			is_running = false;
		else
			q_push(cmd);
	}
}

/*
 * Command processor
 */
void 
controller()
{
	GPIO led(LED);
	led.Direction(OUT);
	
	std::string msg;
	std::thread t_blink;
	
	while (is_running)
	{
		msg = q_pop();
		if (msg == "")
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		else if (msg == "on")
		{
			led.Write(HIGH);
		}
		else if (msg == "off")
		{
			led.Write(LOW);
		}
		else if (msg == "start" || msg == "blink")
		{
			if (!do_blink) { t_blink = std::thread(blink); }
		}
		else if (msg == "stop")
		{
			if (do_blink) { do_blink = false; t_blink.join(); }
		}
	}
	if (do_blink) { do_blink = false; t_blink.join(); }
}

// ============================================================================

/*
 * Program entry point
 */
int 
main()
{
	std::thread t_controller(controller);
	std::thread t_input(button_listen);
	std::thread t_prompt(prompt);

	t_prompt.join();
	t_input.join();
	t_controller.join();

	return 0;
}

