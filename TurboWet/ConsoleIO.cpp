#include <iostream>

#include "ConsoleIO.h"

ConsoleIO* ConsoleIO::instance = nullptr;
HANDLE ConsoleIO::hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void ConsoleIO::consoleCLR(int color)
{

	/*
	0   BLACK
	1   BLUE
	2   GREEN
	3   CYAN
	4   RED
	5   MAGENTA
	6   BROWN
	7   LIGHTGRAY
	8   DARKGRAY
	9   LIGHTBLUE
	10  LIGHTGREEN
	11  LIGHTCYAN
	12  LIGHTRED
	13  LIGHTMAGENTA
	14  YELLOW
	15  WHITE
	*/

	SetConsoleTextAttribute(hConsole, color);
}

void ConsoleIO::system_ok(std::string str, int sof) //"str" variable is for the name of the thing being checked, sof variable is for OK/FAIL. 1 for OK, 0 for FAIL.
{
	if (sof == 1)
	{
		consoleCLR(2); //green
		std::cout << "[OK] ";
	}
	else if (sof == 0)
	{
		consoleCLR(4); //red
		std::cout << "[FAIL] ";
	}
	else
	{
		consoleCLR(3);
		std::cout << "[???] ";
	}

	consoleCLR(15); //white
	std::cout << str << "\n";
}

void ConsoleIO::FSM_IO(State* state, int eoe) //eoe is enter or exit. enter = 0, exit = 1
{
	if (eoe == 0)
	{
		consoleCLR(7); //light gray
		std::cout << "\nENTER ";
		consoleCLR(14);
		std::cout << state->get_state_name() << "\n";
	}
	else if (eoe == 1)
	{
		consoleCLR(8); //dark gray
		std::cout << "EXIT ";
		consoleCLR(14);
		std::cout << state->get_state_name() << "\n";
	}
	else
	{
		consoleCLR(4); // red
		std::cout << "'eoe' NOT SPECIFIED ";
		consoleCLR(14);
		std::cout << state->get_state_name() << "\n";
	}

	consoleCLR(15);
}


