#pragma once

#include "prompts.h"
#include "map_operations.h"

#include <string>
#include <algorithm>

void replace_square(maze& m, int x1, int y1, int x2, int y2, char c) {
	if (x1 > x2) std::swap(x1, x2);
	if (y1 > y2) std::swap(y1, y2);
	int dw = x2 - x1;
	int dh = y2 - y1;
	for (int i = 0; i <= dh; i++)
		for (int j = 0; j <= dw; j++)
			if(m.map[(y1 + i) * m.w + x1 + j] != 'S' && m.map[(y1 + i) * m.w + x1 + j]!= 'X') m.map[(y1 + i) * m.w + x1 + j] = c;
}

void set_point(maze &m, int x, int y, char c, bool &flag, int &cx, int &cy) {
	if (m.map[y * m.w + x] == '#') std::cout << "The starting point cannot be set to (" << x << "," << y << ") since there is an obstacle in that position.\n";
	else {
		if (flag) m.map[cy * m.w + cx] = '.';
		m.map[y * m.w + x] = c;
		cy = y; cx = x;
		flag = true;
	}
}

int build_map(maze &m) {

	bool should_close_editor = 0;

	std::cout << "Input the width of the map (how many columns) .\n";
	m.w = int_prompt("\t==> ", "Please, input only numeric values.");
	std::cout << "Input the height of the map (how many rows).\n";
	m.h = int_prompt("\t==> ", "Please, input only numeric values.");

	m.map.resize(m.w * m.h);
	for (auto it = m.map.begin(); it != m.map.end(); ++it) *it = '.';

	do {
		std::cout << "\t\t////////////////\n";
		std::cout << "\t\t// MAP EDITOR //\n";
		std::cout << "\t\t////////////////\n";
		m.show_layout_info();
		std::cout << "\t\tSelect an operation:\n";
		std::cout << "1) Set obstacle\n";
		std::cout << "2) Remove obstacle\n";
		std::cout << "3) Set/move start point\n";
		std::cout << "4) Set/move goal\n";
		std::cout << "5) Finish editing\n";

		switch (int op = int_prompt("\t\t==> ", "Please, input only numeric values.")) {
		case 1: replace_square(m,
			int_prompt_bounds("\tSet upper-left corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			int_prompt_bounds("\tSet upper-left corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			int_prompt_bounds("\tSet lower-right corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			int_prompt_bounds("\tSet lower-right corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			'#'); break;
		case 2: replace_square(m,
			int_prompt_bounds("\tSet upper-left corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			int_prompt_bounds("\tSet upper-left corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			int_prompt_bounds("\tSet lower-right corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			int_prompt_bounds("\tSet lower-right corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			'.'); break;
		case 3: set_point(m,
			int_prompt_bounds("\tSet start point column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			int_prompt_bounds("\tSet start point row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			'S', m.start_set, m.sx, m.sy); break;
		case 4: set_point(m,
			int_prompt_bounds("\tSet goal point column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			int_prompt_bounds("\tSet goal point row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			'X', m.goal_set, m.gx, m.gy); break;
		case 5: should_close_editor = true; break;
		default: std::cout << "Option not recognized. Please, insert only numeric values between 1 and 5.\n"; break;
		}
		std::cout << "\nPress Enter to continue.\n";
		std::cin.ignore();
		std::cout << "-----------------------------------------------------------------------------------------------\n";
		
	} while (!should_close_editor);
	return 1;
}

int edit_map(maze &m) {

	bool should_close_editor = 0;

	do {
		CLEAR_SCREEN;
		std::cout << "\t\t////////////////\n";
		std::cout << "\t\t// MAP EDITOR //\n";
		std::cout << "\t\t////////////////\n";
		m.show_layout_info();
		std::cout << "\t\tSelect an operation:\n";
		std::cout << "1) Set obstacle\n";
		std::cout << "2) Remove obstacle\n";
		std::cout << "3) Set/move start point\n";
		std::cout << "4) Set/move goal\n";
		std::cout << "5) Finish editing\n";

		switch (int op = int_prompt("\t\t==> ", "Please, input only numeric values.")) {
		case 1: replace_square(m,
			int_prompt_bounds("\tSet upper-left corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			int_prompt_bounds("\tSet upper-left corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			int_prompt_bounds("\tSet lower-right corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			int_prompt_bounds("\tSet lower-right corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			'#'); break;
		case 2: replace_square(m,
			int_prompt_bounds("\tSet upper-left corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			int_prompt_bounds("\tSet upper-left corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			int_prompt_bounds("\tSet lower-right corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			int_prompt_bounds("\tSet lower-right corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			'.'); break;
		case 3: set_point(m,
			int_prompt_bounds("\tSet start point column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			int_prompt_bounds("\tSet start point row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			'S', m.start_set, m.sx, m.sy); break;
		case 4: set_point(m,
			int_prompt_bounds("\tSet goal point column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			int_prompt_bounds("\tSet goal point row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			'X', m.goal_set, m.gx, m.gy); break;
		case 5: should_close_editor = true; break;
		default: std::cout << "Option not recognized. Please, insert only numeric values between 1 and 5.\n"; break;
		}
		std::cout << "-----------------------------------------------------------------------------------------------\n";
		std::cout << "Press Enter to continue.\n";
		std::cin.ignore();
	} while (!should_close_editor);
	return 1;
}