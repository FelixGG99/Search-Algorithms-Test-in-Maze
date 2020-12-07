#pragma once
#ifndef MAZE_MAP_H	//header guards
#define MAZE_MAP_H

/*
	File:			map_operations.h
	Description:	Header where the map class (named maze) is defined, as well as the clear, save, load, search and print operations.
	Authors:		Name							ID:			Mat.:
					Félix Garduza Gutiérrez			3429399		24500597
					Axel Adrían Flores				3431762		24500628
					Ignacio Augusto Rodríguez Díaz	3424369		24500586
					Cindy Berenice Castro Aguilar	3376177		24400254
	Last updated:	06/12/2020
	Version:		1.0
*/

#include <iostream>		// cin, cout
#include <fstream>		// ifstream, ofstream

#include "prompts.h"	// Prompts, console manipulation, formatting, colors

class maze {			// maze class definition
public:

	// class members
	bool start_set, goal_set;	// bools to mark if the start point and goal point, respectively, exist in the map
	int w, h, sx, sy, gx, gy;	// width, height, start point and goal point coords
	std::string map;			// map stored in a string class

	maze() { w = h = sx = sy = gx = gy = 0; start_set = goal_set = false; }	// initializer
	~maze() {}	// destructor

	// procedures, all defined later in this file
	void				print_map();								// print map on the screen. (line 31)
	void				show_layout_info();							// print map and show info about its current layout. (line 46)
	void				clear();									// clears the map and sets dimensions, coords to 0 and marks to false. (line 55)
	int					load_map(const std::string &filename);		// loads map into the class from a specific file. (line 64)
	int					save_map(const std::string &filename);		// saves map into a specific file. (line 101)
	std::pair<int, int>	search_point(char c);						// searches for the first position in the map that stores a certain character,
																	// used for locating start/goal point if unknown to the class. (line 134)
};

void maze::print_map() {
	// For each row i included in the range [0,h]...
	for (int i = 0; i < h; i++) {
		// For each column j included in the range [0,w]...
		for (int j = 0; j < w; j++) {
			// set color according to the type of character stored and print it on the screen
			set_char_color(map[i * w + j]);
			std::cout << map[i * w + j];
		}
		// return color to white
		set_text_color(conWHITE);
		std::cout << "\n";
	}
}

void maze::show_layout_info() {
	// print map in the screen
	print_map();
	// print info about the map
	std::cout << "\n\n";
	std::cout << "Numeration of each position begins in the upper-left corner on the map, in (0,0),\nand ends in the lower-right corner, in (w-1, h-1). The notation used is (column, row).\n";
	std::cout << "~ Width/columns (w): " << w << "\n~ Height/rows (h): " << h << "\n\n";
}

void maze::clear() {
	// set coords and dimensions to 0
	w = h = sx = sy = gx = gy = 0;
	// set booleans to false
	goal_set = start_set = false;
	// clear the string storing the map contents
	map.clear();
}

int maze::load_map(const std::string& filename) {
	// try opening the file "filename" in reading mode and storing the pointer to it in map_file
	std::ifstream map_file(filename);
	// if map_file stores a pointer (filename was opened)...
	if (map_file.is_open()) {
		// read width and height from file
		map_file >> w >> h;
		// resize string class storing the map to a size of (w*h)
		map.resize(w * h);
		// For each row i included in the range [0,h]...
		for (int i = 0; i < h; i++) {
			// For each column j included in the range [0,w]...
			for (int j = 0; j < w; j++) {
				// read the character corresponding to the coord (j,i) where j is the column and i is the row
				// store it in map[i*w+j], which is analog to map[i][j] which can't used because of string class limitations
				map_file >> map[i * w + j];
				// if map[i * w + j] == 'S', save the start point coords and mark it as found
				if (map[i * w + j] == 'S') {
					sx = j; sy = i;
					start_set = true;
				}
				// if map[i * w + j] == 'X', save the goal point coords and mark it as found
				else if (map[i * w + j] == 'X') {
					gx = j; gy = i;
					goal_set = true;
				}
			}
			map_file.ignore(1, '\n'); // ignore the next end of line to prevent it from altering further inputs
		}
		// close map and return 1 (loading successful)
		map_file.close();
		return 1;
	}
	// close map and return 0 (loading failed)
	else return 0;
}

int maze::save_map(const std::string& filename) {
	// try opening the file "filename" and storing the pointer to it in map_file to verify if the file already exists
	std::ifstream f(filename);
	// if map_file stores a pointer (filename exists and was opened)...
	if (f.good()) { 
		// ask if user wants to overwrite. if not, return 0 (saving failed/not done).
		warn_msg("This file already exists. Do you want to overwrite it? Y/N");
		if (!y_n_prompt("==> ", "Invalid answer. Please, input either Y or N.")) return 0;
	}
	if (f.is_open()) f.close();	// close file in reading mode

	// open file in writing mode 
	std::ofstream output(filename, std::ios_base::trunc | std::ios_base::out);
	// if file was opened...
	if (output.is_open()) {
		// print width and height on the map
		output << w << " " << h << "\n";
		// For each row i included in the range [0,h]...
		for (int i = 0; i < h; i++) {
			// For each column j included in the range [0,w]...
			for (int j = 0; j < w; j++)
				// print the character contained in the current row and column
				output << map[i * w + j];
			output << "\n";
		}
		// close file and return 1 (saving successful)
		output.close();
		return 1;
	}
	// else, return 0 (saving failed).
	else return 0;
}

std::pair<int, int> maze::search_point(char c) {
	// For each row i included in the range [0,h]...
	for (int i = 0; i < h; i++)
		// For each column j included in the range [0,w]...
		for (int j = 0; j < w; j++)
			// if map in current position (row i and column j), return pair of (i,j), coords of the char asked to be found.
			if (map[i * w + j] == c) return std::make_pair(i, j); 
	// if done visiting each position on the map, return a pair (-1,-1), invalid coords, denoting the query failed.
	return std::make_pair(-1, -1);
}

#endif // !MAZE_MAP_H
