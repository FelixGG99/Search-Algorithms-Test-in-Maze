#pragma once

/*
	File:			map_editor.h
	Description:	Header where the map editor and the functions used in it are defined.
	Authors:		Name							ID:			Mat.:
					Félix Garduza Gutiérrez			3429399		24500597
					Axel Adrían Flores				3431762		24500628
					Ignacio Augusto Rodríguez Díaz	3424369		24500586
					Cindy Berenice Castro Aguilar	3376177		24400254
	Last updated:	06/12/2020
	Version:		1.0
*/

#include "prompts.h"					// Prompts, console manipulation, formatting, colors
#include "map_operations.h"				// Maze class definition, save, load, etc

#include <string>						// string class
#include <algorithm>					// swap

void update_map(maze& m, int x1, int y1, int x2, int y2, int ox, int oy) {

	// make (x1,y1) point the upper-left corner and (x2,y2) the lower-right corner for easier manipulation
	if (x1 > x2) std::swap(x1, x2);
	if (y1 > y2) std::swap(y1, y2);
	// Calculate dw and dh, the width and height difference between both points
	int dw = x2 - x1;
	int dh = y2 - y1;
	// Loops to update each position in a square of dw*dh dimensions, starting from (x1,y1)
	// For each i between 0 and dh...
	for (int i = 0; i <= dh; i++) {
		gotoxy(ox + x1, oy + y1 + i);
		// For each j between 0 and dw...
		for (int j = 0; j <= dw; j++) {
			// Print the symbol ín [(x1 + j), (y1 + i)] in a color corresponding with its kind
			set_char_color(m.map[(y1 + i) * m.w + x1 + j]);
			std::cout << m.map[(y1 + i) * m.w + x1 + j];
		}
	}
	set_text_color(conWHITE);
}

void replace_square(maze& m, int x1, int y1, int x2, int y2, char c) {
	// make (x1,y1) point the upper-left corner and (x2,y2) the lower-right corner for easier manipulation
	if (x1 > x2) std::swap(x1, x2);
	if (y1 > y2) std::swap(y1, y2);
	// Calculate dw and dh, the width and height difference between both points
	int dw = x2 - x1;
	int dh = y2 - y1;
	// Loops to replace each position on the map in a square of dw*dh dimensions with a certain character, starting from (x1,y1)
	// For each i between 0 and dh...
	for (int i = 0; i <= dh; i++)
		for (int j = 0; j <= dw; j++)
			if(m.map[(y1 + i) * m.w + x1 + j] != 'S' && m.map[(y1 + i) * m.w + x1 + j]!= 'X') 
				m.map[(y1 + i) * m.w + x1 + j] = c;
}

void set_point(maze &m, int x, int y, char c, bool &flag, int &cx, int &cy) {
	// if the inputted position is out of bounds, send error message
	if (x < 0 || x >= m.w || y < 0 || y >= m.h) error_msg("The starting point cannot be set on that position since it is out of bounds.");
	// else, if the map has an obstacle stored in that position, send error message
	else if (m.map[y * m.w + x] == '#') error_msg("The starting point cannot be set on that position since there is an obstacle.");
	// else, set the current point to the desired character
	else {
		/* flag is the variable that marks whether a certain kind of box is set on the maze class, cx and cy are the variables storing the coords of that certain kind of 
		box - all passed by reference. flag is used to pass either goal_set or start_set. if flag is not false (the desired kind of box exists on the map), set the box currently 
		storing either the goal or start to an empty space (in position (cx, cy), in order to replace it.*/
		if (flag) m.map[cy * m.w + cx] = '.';
		// set the point in the desired coordinates to the value passed in c, and update the coords of that kind of box to the new ones (set cx to x and cy to y).
		m.map[y * m.w + x] = c;
		cy = y; cx = x;
		// mark the flag in the maze class to true, to indicate that the kind of box exists on the map.
		flag = true;
	}
}

int build_map(maze &m, bool &map_loaded) {
	// flag that indicates if the loop of the map editor should end
	bool should_close_editor = 0;

	// if a map is not loaded/ does not exist on the program...
	if (!map_loaded) {
		// ask for dimensions of the map to be created
		std::cout << "Size of map is limitless. However, the larger the map is, the longer a search will take,\nand, depending on the size of the console window, the map will appaer distorted.\n";
		std::cout << "For that reason, the limit of the size of the map should be given by the size of the window.\n";
		std::cout << "Input the width of the map (how many columns) .\n";
		m.w = int_prompt("\t==> ", "Please, input only numeric values.");
		std::cout << "Input the height of the map (how many rows).\n";
		m.h = int_prompt("\t==> ", "Please, input only numeric values.");

		// if either the width or height are 0, send message error and return 0 (map not created/edited)
		if (m.w == 0 || m.h == 0) {
			error_msg("Both width and height of the map must be at least 1.");
			return 0;
		}
		// resize the string storing the map data to the correct size and fill it with '.' to indicate free spaces.
		m.map.resize(m.w * m.h);
		for (auto it = m.map.begin(); it != m.map.end(); ++it) *it = '.';
		// mark the map as existing on the program
		map_loaded = true;
	}
	// Show map editor title
	title2("Map Editor", conCYAN, conLIGHTCYAN);
	// Save console coords of where the map starts showing in order to be able to update it in further operations
	int ox, oy;
	wherexy(ox, oy);
	// show map and options menu
	m.show_layout_info();
	std::cout << "\t\tSelect an operation:\n";
	std::cout << "1) Set obstacle\n";
	std::cout << "2) Remove obstacle\n";
	std::cout << "3) Set/move start point\n";
	std::cout << "4) Set/move goal\n";
	std::cout << "5) Finish editing\n";
	// save console coords of where the prompt starts showing in order to clear from after finishing the operation to be performed
	int ax, ay;
	wherexy(ax, ay);
	int p1, p2, p3, p4;
	do {
		// read option
		switch (int op = int_prompt("\t\t==> ", "Please, input only numeric values.")) {
		case 1:	// Set obstable 
			p1 = int_prompt_bounds("\tSet upper-left corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, std::string("Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + ".").c_str());
			p2 = int_prompt_bounds("\tSet upper-left corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, std::string("Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + ".").c_str());
			p3 = int_prompt_bounds("\tSet lower-right corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, std::string("Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + ".").c_str());
			p4 = int_prompt_bounds("\tSet lower-right corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, std::string("Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + ".").c_str());
			replace_square(m,p1, p2, p3, p4, '#'); update_map(m, p1, p2, p3, p4, ox, oy); break;
		case 2: replace_square(m, // remove obstacle
			p1 = int_prompt_bounds("\tSet upper-left corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, std::string("Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + ".").c_str()),
			p2 = int_prompt_bounds("\tSet upper-left corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, std::string("Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + ".").c_str()),
			p3 = int_prompt_bounds("\tSet lower-right corner column ==> ", "Please, input only numeric values.", 0, m.w - 1, std::string("Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + ".").c_str()),
			p4 = int_prompt_bounds("\tSet lower-right corner row ==> ", "Please, input only numeric values.", 0, m.h - 1, std::string("Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + ".").c_str()),
			'.'); update_map(m, p1, p2, p3, p4, ox, oy); break;
		case 3: set_point(m, // set/move start point
			p1 = int_prompt_bounds("\tSet start point column ==> ", "Please, input only numeric values.", 0, m.w - 1, std::string("Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + ".").c_str()),
			p2 = int_prompt_bounds("\tSet start point row ==> ", "Please, input only numeric values.", 0, m.h - 1, std::string("Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + ".").c_str()),
			'S', m.start_set, m.sx, m.sy); update_map(m, 0, 0, m.w - 1, m.h - 1, ox, oy); break;
		case 4: set_point(m, // set/move goal point
			p1 = int_prompt_bounds("\tSet goal point column ==> ", "Please, input only numeric values.", 0, m.w - 1, std::string("Input values between " + std::to_string(0) + "and " + std::to_string(m.w - 1) + ".").c_str()),
			p2 = int_prompt_bounds("\tSet goal point row ==> ", "Please, input only numeric values.", 0, m.h - 1, std::string("Input values between " + std::to_string(0) + "and " + std::to_string(m.h - 1) + ".").c_str()),
			'X', m.goal_set, m.gx, m.gy); update_map(m, 0, 0, m.w - 1, m.h - 1, ox, oy); break;
		case 5: should_close_editor = true; break; // end map editor loop
		default: std::cout << "Option not recognized. Please, insert only numeric values between 1 and 5.\n"; break; // invalid option
		}
		// clear console from (ax, ay) on and move cursor to said position
		clear_from(ax, ay);
		gotoxy(ax, ay);
	} while (!should_close_editor);
	return 1; //map created/edited
}