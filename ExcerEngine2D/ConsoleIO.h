#pragma once

#include <Windows.h>

#include "FSM.h"

class ConsoleIO
{
private:
	static HANDLE hConsole;
	static ConsoleIO* instance;

public:
	static void consoleCLR(int color);
	static void system_ok(std::string str, int sof);
	static void FSM_IO(State* state, int eoe);

	static ConsoleIO* get_instance()
	{
		if (instance == nullptr)
			instance = new ConsoleIO();
		return instance;
	}
};

