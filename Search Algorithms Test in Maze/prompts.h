#pragma once
#ifndef PROMPTS_H
#define PROMPTS_H

#include <iostream>
#include <stdlib.h>

#define NOT_VALID_INT -2147483648

#if		defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#include <Windows.h>
#define CLEAR_SCREEN std::system("cls")

#define conBLACK			0
#define conBLUE				1
#define conGREEN			2
#define conCYAN				3
#define conRED				4
#define conMAGENTA			5
#define conBROWN			6
#define conLIGHTGRAY		7
#define conDARKGRAY			8
#define conLIGHTBLUE		9
#define conLIGHTGREEN		10
#define conLIGHTCYAN		11
#define conLIGHTRED			12
#define conLIGHTMAGENTA		13
#define conYELLOW			14
#define conWHITE			15

void gotoxy(short int x, short int y) {
	COORD pos = { x, y };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}

void wherexy(int& x, int& y) {
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!GetConsoleScreenBufferInfo(hStd, &screenBufferInfo))
		printf("GetConsoleScreenBufferInfo (%d)\n", GetLastError());
	x = screenBufferInfo.dwCursorPosition.X;
	y = screenBufferInfo.dwCursorPosition.Y;
}

void set_text_color(const int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

#else
#include <unistd.h>
#define CLEAR_SCREEN std::system("clear")
#endif

const int int_prompt(const std::string& prompt, const std::string &invalid_msg) {
	int p;
	char* endchar;
	std::string input;
	do {
		std::cout << prompt;
		std::cin >> input;
		p = strtol(input.c_str(), &endchar, 0);
		if (*endchar) {
			std::cout << invalid_msg << "\n";
			p = NOT_VALID_INT;
		}
	} while (p == NOT_VALID_INT);
	std::cin.ignore(INT_MAX, '\n');
	return p;
}

const int int_prompt_bounds(const std::string& prompt, const std::string& invalid_msg1, int lb, int ub, const std::string& invalid_msg2) {
	int p;
	char* endchar;
	std::string input;
	do {
		std::cout << prompt;
		std::cin >> input;
		p = strtol(input.c_str(), &endchar, 0);
		if (*endchar) {
			std::cout << invalid_msg1 << "\n";
			p = NOT_VALID_INT;
		}
		else if (p < lb || p > ub) {
			std::cout << invalid_msg2 << "\n";
			p = NOT_VALID_INT;
		}
	} while (p == NOT_VALID_INT);
	std::cin.ignore(INT_MAX, '\n');
	return p;
}

const std::string string_prompt(const std::string& prompt) {
	std::string input = "";
	std::cout << prompt;
	std::cin >> input;
	return input;
}

const int y_n_prompt(const std::string& prompt, const std::string& invalid_msg) {
	char p = 0;
	
	do {
		std::cout << prompt;
		std::cin >> p;
		p = toupper(p);
		if (p != 'Y' && p != 'N') {
			std::cout << invalid_msg << "\n";
			p = 0;
		}
	} while (!p);
	std::cin.ignore(INT_MAX, '\n');
	return p == 'Y' ? 1 : 0;
}

void info() {
	set_text_color(conYELLOW);
	std::cout << "\n";
	std::cout << "\t\t////////////////////////////\n";
	std::cout << "\t\t//   ABOUT THIS PROJECT   //\n";
	std::cout << "\t\t////////////////////////////\n";
	set_text_color(conWHITE);
	std::cout << "\nThis project is meant to serve as an excercise to evaluate the understanding of\n";
	std::cout << "its authors about different search algorihms, as well as a sim that demonstrates \n";
	std::cout << "how said algorithms work. It was written for the Aritificial Intelligence, taught by \n";
	std::cout << "Dr. Vittorio Zanella Palacios.\n\n";
	std::cout << "Authors:\n";
	std::cout << "Félix Garduza Gutiérrez, 24500597\n";
	std::cout << "Ignacio, [matricula]\n";
	std::cout << "Axel Adrián Flores, [matricula]\n";
	std::cout << "Cindy, [matricula]\n\n";
	std::cout << "Version\t\t: 1.0\n";
	std::cout << "Last Updated\t: 18/11/2020\n";
}

#endif // PROMPTS_H
