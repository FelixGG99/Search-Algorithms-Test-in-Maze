#pragma once

#include "prompts.h"
#include "map_operations.h"

#include <string>
#include <algorithm>

void update_map(maze& m, int x1, int y1, int x2, int y2, int ox, int oy) {
	if (x1 > x2) std::swap(x1, x2);
	if (y1 > y2) std::swap(y1, y2);
	int dw = x2 - x1;
	int dh = y2 - y1;
	for (int i = 0; i <= dh; i++) {
		gotoxy(ox + x1, oy + y1 + i);
		for (int j = 0; j <= dw; j++) {
			set_char_color(m.map[(y1 + i) * m.w + x1 + j]);
			std::cout << m.map[(y1 + i) * m.w + x1 + j];
		}
	}
	set_text_color(conWHITE);
}

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

	set_text_color(conYELLOW);
	std::cout << "\t\t////////////////\n";
	std::cout << "\t\t// MAP EDITOR //\n";
	std::cout << "\t\t////////////////\n";
	std::cout << "\n\t\tCurrent Maze Layout:\n\t\t-------------------\n\n";
	set_text_color(conWHITE);
	int ox, oy;
	wherexy(ox, oy);
	m.show_layout_info();
	std::cout << "\t\tSelect an operation:\n";
	std::cout << "1) Set obstacle\n";
	std::cout << "2) Remove obstacle\n";
	std::cout << "3) Set/move start point\n";
	std::cout << "4) Set/move goal\n";
	std::cout << "5) Finish editing\n";
	
	int ax, ay;
	wherexy(ax, ay);
	int p1, p2, p3, p4;
	do {
		switch (int op = int_prompt("\t\t==> ", "Please, input only numeric values.")) {
		case 1:
			p1 = int_prompt_bounds("\tSet upper-left corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + ".");
			p2 = int_prompt_bounds("\tSet upper-left corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + ".");
			p3 = int_prompt_bounds("\tSet lower-right corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + ".");
			p4 = int_prompt_bounds("\tSet lower-right corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + ".");
			replace_square(m,p1, p2, p3, p4, '#'); update_map(m, p1, p2, p3, p4, ox, oy); break;
		case 2: replace_square(m,
			p1 = int_prompt_bounds("\tSet upper-left corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			p2 = int_prompt_bounds("\tSet upper-left corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			p3 = int_prompt_bounds("\tSet lower-right corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			p4 = int_prompt_bounds("\tSet lower-right corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			'.'); update_map(m, p1, p2, p3, p4, ox, oy); break;
		case 3: set_point(m,
			p1 = int_prompt_bounds("\tSet start point column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			p2 = int_prompt_bounds("\tSet start point row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			'S', m.start_set, m.sx, m.sy); update_map(m, p1, p2, p1, p2, ox, oy); break;
		case 4: set_point(m,
			p1 = int_prompt_bounds("\tSet goal point column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			p2 = int_prompt_bounds("\tSet goal point row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			'X', m.goal_set, m.gx, m.gy); update_map(m, p1, p2, p1, p2, ox, oy); break;
		case 5: should_close_editor = true; break;
		default: std::cout << "Option not recognized. Please, insert only numeric values between 1 and 5.\n"; break;
		}
		clear_from(ax, ay);
		gotoxy(ax, ay);
	} while (!should_close_editor);
	return 1;
}

int edit_map(maze &m) {

	bool should_close_editor = 0;
	set_text_color(conYELLOW);
	std::cout << "\t\t////////////////\n";
	std::cout << "\t\t// MAP EDITOR //\n";
	std::cout << "\t\t////////////////\n";
	set_text_color(conWHITE);
	std::cout << "\n\t\tCurrent Maze Layout:\n\t\t-------------------\n\n";
	int ox, oy;
	wherexy(ox, oy);
	m.show_layout_info();
	std::cout << "\t\tSelect an operation:\n";
	std::cout << "1) Set obstacle\n";
	std::cout << "2) Remove obstacle\n";
	std::cout << "3) Set/move start point\n";
	std::cout << "4) Set/move goal\n";
	std::cout << "5) Finish editing\n";

	int ax, ay;
	wherexy(ax, ay);
	int p1, p2, p3, p4;
	do {
		switch (int op = int_prompt("\t\t==> ", "Please, input only numeric values.")) {
		case 1:
			p1 = int_prompt_bounds("\tSet upper-left corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + ".");
			p2 = int_prompt_bounds("\tSet upper-left corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + ".");
			p3 = int_prompt_bounds("\tSet lower-right corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + ".");
			p4 = int_prompt_bounds("\tSet lower-right corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + ".");
			replace_square(m, p1, p2, p3, p4, '#'); update_map(m, p1, p2, p3, p4, ox, oy); break;
		case 2: replace_square(m,
			p1 = int_prompt_bounds("\tSet upper-left corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			p2 = int_prompt_bounds("\tSet upper-left corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			p3 = int_prompt_bounds("\tSet lower-right corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			p4 = int_prompt_bounds("\tSet lower-right corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			'.'); update_map(m, p1, p2, p3, p4, ox, oy); break;
		case 3: set_point(m,
			p1 = int_prompt_bounds("\tSet start point column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			p2 = int_prompt_bounds("\tSet start point row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			'S', m.start_set, m.sx, m.sy); update_map(m, p1, p2, p1, p2, ox, oy); break;
		case 4: set_point(m,
			p1 = int_prompt_bounds("\tSet goal point column ==> ", "Please, input only numeric values.", 0, m.w - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + "."),
			p2 = int_prompt_bounds("\tSet goal point row ==> ", "Please, input only numeric values.", 0, m.h - 1, "Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + "."),
			'X', m.goal_set, m.gx, m.gy); update_map(m, p1, p2, p1, p2, ox, oy); break;
		case 5: should_close_editor = true; break;
		default: std::cout << "Option not recognized. Please, insert only numeric values between 1 and 5.\n"; break;
		}
		clear_from(ax, ay);
		gotoxy(ax, ay);
	} while (!should_close_editor);
	return 1;
}