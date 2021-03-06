#pragma once
#ifndef PROMPTS_H
#define PROMPTS_H

/*
	File:			prompts.h
	Description:	Header where prompts, message displays, console manipulation wrappers and other values and utilities are defined..
	Authors:		Name							ID:			Mat.:
					F�lix Garduza Guti�rrez			3429399		24500597
					Axel Adr�an Flores				3431762		24500628
					Ignacio Augusto Rodr�guez D�az	3424369		24500586
					Cindy Berenice Castro Aguilar	3376177		24400254
	Last updated:	06/12/2020
	Version:		1.0
*/

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>

// define invalid int values
#define NOT_VALID_INT -214748364
#define NOT_VALID_INT2 214748364

// define console manipulation wrappers to be used in Windows systems
#if		defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
	#include <Windows.h>
	//  clear screen function wrapper
	#define CLEAR_SCREEN std::system("cls")
	//  sleep function wrapper
	#define sleep_ Sleep
	// color values in Windows
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
	
	// move cursor to (x,y)
	void gotoxy(short int x, short int y) {
		COORD pos = { x, y };
		HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(output, pos);
	}
	// save current cursor pos
	void wherexy(int& x, int& y) {
		CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
		HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
		if (!GetConsoleScreenBufferInfo(hStd, &screenBufferInfo))
			printf("GetConsoleScreenBufferInfo (%d)\n", GetLastError());
		x = screenBufferInfo.dwCursorPosition.X;
		y = screenBufferInfo.dwCursorPosition.Y;
	}
	// set console text color to a certain value
	int set_text_color(const int color) {
		return SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	}
	// clear console starting from a certain position until end
	int clear_from(int x, int y) {
		// Check for double buffering using a CreateConsoleScreenBuffer.
		// https://docs.microsoft.com/en-us/windows/console/writeconsoleoutput
		CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
		HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
		if (!GetConsoleScreenBufferInfo(hStd, &screenBufferInfo)) {
			printf("GetConsoleScreenBufferInfo (%d)\n", GetLastError());
			return 0;
		}
		int size_x = screenBufferInfo.dwSize.X;
		int size_y = screenBufferInfo.dwSize.Y;

		CHAR_INFO* clearRect = new CHAR_INFO[((size_y - y) + 1) * ((size_x - x) + 1)]{ ' ' };

		SMALL_RECT srctWriteRect;
		COORD coordBufSize;
		COORD coordBufCoord;

		srctWriteRect.Left = x;
		srctWriteRect.Top = y;
		srctWriteRect.Right = size_x - 1;
		srctWriteRect.Bottom = size_y - 1;

		coordBufSize.X = size_y - y;
		coordBufSize.Y = size_x - x;

		coordBufCoord.X = 0;
		coordBufCoord.Y = 0;

		WriteConsoleOutput(hStd, clearRect, coordBufSize, coordBufCoord, &srctWriteRect);
		delete[] clearRect;
		return 1;
	}

	// scroll console up until reaching coords (x,y)
	int scroll_up_to(int x, int y) {
		CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
		SMALL_RECT srctWindow;

		if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo)){
			std::cout << "GetConsoleScreenBufferInfo (" << GetLastError() << ")\n";
			return 0;
		}

		srctWindow = csbiInfo.srWindow;
		int iRows =  srctWindow.Top - y;	

		if (iRows >= 0) {
			srctWindow.Top -= (SHORT)iRows;     // move top up
			srctWindow.Bottom -= (SHORT)iRows;  // move bottom up

			if (!SetConsoleWindowInfo(
				GetStdHandle(STD_OUTPUT_HANDLE),          // screen buffer handle
				TRUE,             // absolute coordinates
				&srctWindow))     // specifies new location
			{
				std::cout << "SetConsoleWindowInfo (" << GetLastError() << ")\n";
				return 0;
			}
			return iRows;
		}
		else return 0;

	}

#else // TO-DO: define console manipulation wrappers for POSIX-based systems
	#include <unistd.h>
	#define CLEAR_SCREEN std::system("clear")
#endif

// display error-type message
void error_msg(const char* msg) {
	int oc = set_text_color(conRED);
	std::cout << ">> �\t" << " ERROR:\t\t";
	set_text_color(conLIGHTRED);
	std::cout << msg << "\n";
	set_text_color(conWHITE);
}
// display info-type message
void info_msg(const char* msg) {
	set_text_color(conBLUE);
	std::cout << ">> (i)\t" << " INFO:\t\t";
	set_text_color(conLIGHTBLUE);
	std::cout << msg << "\n";
	set_text_color(conWHITE);
}
// display warning-type message
void warn_msg(const char* msg) {
	int oc = set_text_color(conBROWN);
	std::cout << ">> !\t" << " WARNING:\t\t";
	set_text_color(conYELLOW);
	std::cout << msg << "\n";
	set_text_color(conWHITE);
}
// display success-type message
void succ_msg(const char* msg) {
	int oc = set_text_color(conGREEN);
	std::cout << ">> (OK)\t" << " SUCCESS:\t";
	set_text_color(conLIGHTGREEN);
	std::cout << msg << "\n";
	set_text_color(conWHITE);
}
// make program wait until Enter (or any other key) is inputted
inline void wait_key() {
	std::cout << "\nPress Enter to continue.\n";
	std::cout << "-----------------------------------------------------------------------------------------------\n";
	std::cin.ignore();
}
// set console text color to a certain value, depending of the type of character c is
void set_char_color(char c) {

	switch (c) {
	case '#': set_text_color(conBROWN); break;
	case 'S': set_text_color(conYELLOW); break;
	case 'X': set_text_color(conLIGHTGREEN); break;
	case '.': set_text_color(conWHITE); break;
	}
}
// prompt for int values
const int int_prompt(const char* prompt, const char* invalid_msg) {
	int p;
	char* endchar;
	std::string input;
	int x, y;
	wherexy(x, y);
	do {
		std::cout << prompt;
		std::cin >> input;
		std::cin.ignore(INT_MAX, '\n');
		p = strtol(input.c_str(), &endchar, 0);
		if (*endchar) {
			error_msg(invalid_msg);
			p = NOT_VALID_INT;
			wait_key();
			gotoxy(x, y);
			clear_from(x, y);
		}
	} while (p == NOT_VALID_INT);
	return p;
}
// prompt for int values between certain bounds
const int int_prompt_bounds(const char* prompt, const char *invalid_msg1, int lb, int ub, const char* invalid_msg2) {
	int p;
	char* endchar;
	std::string input;
	int x, y;
	wherexy(x, y);
	do {
		std::cout << prompt;
		std::cin >> input;
		std::cin.ignore(INT_MAX, '\n');
		p = strtol(input.c_str(), &endchar, 0);
		if (*endchar) {
			error_msg(invalid_msg1);
			p = NOT_VALID_INT;
			wait_key();
			gotoxy(x, y);
			clear_from(x, y);
		}
		else if (p < lb || p > ub) {
			error_msg(invalid_msg2);
			p = NOT_VALID_INT;
			wait_key();
			gotoxy(x, y);
			clear_from(x, y);
		}
	} while (p == NOT_VALID_INT);
	return p;
}
// prompt for string values
const std::string string_prompt(const std::string& prompt) {
	std::string input = "";
	std::cout << prompt;
	std::cin >> input;
	std::cin.ignore(INT_MAX, '\n');

	return input;
}
// prompt for expecting only 'y' or 'n'
const int y_n_prompt(const std::string& prompt, const std::string& invalid_msg) {
	char p = 0;
	int x, y;
	wherexy(x, y);
	do {
		std::cout << prompt;
		std::cin >> p;
		std::cin.ignore(INT_MAX, '\n');
		p = toupper(p);
		if (p != 'Y' && p != 'N') {
			std::cout << invalid_msg << "\n";
			p = 0;
			wait_key();
			gotoxy(x, y);
			clear_from(x, y);
		}
	} while (!p);
	return p == 'Y' ? 1 : 0;
}

// display message in a title style
void title1(const std::string& s, const int text_color, const int border_color) {
	/*
		*****************************
		|                           |
		*****************************
	*/

	std::vector<std::string>lines;
	int max_len = 0;

	for (int it = 0; it < (int) s.length();) {
		auto pos = s.find(it, '\n');
		if (pos == s.npos) pos = s.length() - 1;
		int len = pos - it;
		if (len > max_len) max_len = len;
		lines.push_back(s.substr(it, len));
		it = pos + 1;
	}

	int l = max_len + 6;

	int orig_color = set_text_color(border_color);
	for (int i = 0; i < l + 2; i++) std::cout << '*';
	for (int i = 0; i < (int)lines.size(); i++) {
		std::cout << '|';
		int padding_size = l - (int)lines[i].length();
		for (int j = 0; j < padding_size / 2; j++) std::cout << ' ';
		set_text_color(text_color);
		std::cout << lines[i];
		set_text_color(border_color);
		if (padding_size % 2 == 0) std::cout << " ";
		for (int j = 0; j < padding_size / 2; j++) std::cout << ' ';
		std::cout << "|\n";
	}
	for (int i = 0; i < l + 2; i++) std::cout << '*';
	set_text_color(orig_color);
}

// display message in a title style
void title2(const std::string& s, const int text_color, const int border_color) {
	/*
		�===========================�
		|                           |
		�===========================�
	*/

	std::vector<std::string>lines;
	int max_len = 0;

	for (int it = 0; it < (int) s.length();) {
		auto pos = s.find(it, '\n');
		if (pos == s.npos) pos = (int) s.length() - 1;
		int len = pos - it;
		if (len > max_len) max_len = len;
		lines.push_back(s.substr(it, len+1));
		it = pos + 1;
	}
	max_len++;
	int l = max_len + 6;

	int orig_color = set_text_color(border_color);
	std::cout << '�';
	for (int i = 0; i < l; i++) std::cout << '=';
	std::cout << "�\n";
	for (int i = 0; i < (int)lines.size(); i++) {
		std::cout << '|';
		int padding_size = l - lines[i].length();
		for (int j = 0; j < padding_size / 2; j++) std::cout << ' ';
		set_text_color(text_color);
		std::cout << lines[i];
		set_text_color(border_color);
		if (padding_size % 2 == 0) std::cout << " ";
		for (int j = 0; j < padding_size / 2; j++) std::cout << ' ';
		std::cout << "|\n";
	}
	std::cout << '�';
	for (int i = 0; i < l; i++) std::cout << '=';
	std::cout << "�\n";
	set_text_color(orig_color);
}

// display info about the program
void info() {
	set_text_color(conYELLOW);
	std::cout << "\n";
	int scr_px, scr_py;
	wherexy(scr_px, scr_py);
	std::cout << "\t\t////////////////////////////\n";
	std::cout << "\t\t//   ABOUT THIS PROJECT   //\n";
	std::cout << "\t\t////////////////////////////\n";
	set_text_color(conWHITE);
	std::cout << "\nThis project is meant to serve as an excercise to evaluate the understanding of\n";
	std::cout << "its authors about different search algorihms, as well as a sim that demonstrates \n";
	std::cout << "how said algorithms work. It was written for the Aritificial Intelligence, taught by \n";
	std::cout << "Dr. Vittorio Zanella Palacios.\n\n";
	std::cout << "Authors:\n";
	std::cout << "F�lix Garduza Guti�rrez\t\t\t24500597\n";
	std::cout << "Ignacio Augusto Rodr�guez D�az\t24500586\n";
	std::cout << "Axel Adri�n Flores �lvarez\t\t24500628\n";
	std::cout << "Cindy Berenice Castro Aguilar\t24400254\n\n";
	std::cout << "Version\t\t: 1.0\n";
	std::cout << "Last Updated\t: 06/12/2020\n";
	scroll_up_to(scr_px, scr_py);
}

// display help for using the program
void help() {
	std::cout << "\n";
	int scr_px, scr_py;
	wherexy(scr_px, scr_py);
	set_text_color(conYELLOW);
	std::cout << "\t\t////////////////////////////\n";
	std::cout << "\t\t/////   HELP SECTION   /////\n";
	std::cout << "\t\t////////////////////////////\n\n\n";
	set_char_color('.');
	std::cout << "[.] -----> Free space. Possible to move through these spaces.\n";
	set_char_color('#');
	std::cout << "[#] -----> Obstacle. Impossible to move through these spaces.\n";
	set_char_color('S');
	std::cout << "[S]-----> Starting point. This is where the search starts from.\n";
	set_char_color('X');
	std::cout << "[X] -----> Goal point. The objective of the search is to reach this point from the starting point.\n";
	set_text_color(conWHITE);
	scroll_up_to(scr_px, scr_py);
}

#endif // PROMPTS_H
