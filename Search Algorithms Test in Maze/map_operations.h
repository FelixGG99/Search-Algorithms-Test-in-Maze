#pragma once
#ifndef MAZE_MAP_H
#define MAZE_MAP_H

#include <iostream>
#include <fstream>

#include "prompts.h"

class maze {
public:

	bool start_set, goal_set;
	int w, h, sx, sy, gx, gy;
	std::string map;

	maze() { w = h = sx = sy = gx = gy = 0; start_set = goal_set = false; }
	~maze() {}

	void				print_map();
	void				print_map_ruler();
	void				show_layout_info();
	void				clear();
	int					load_map(const std::string &filename);
	int					save_map(const std::string &filename);
	std::pair<int, int>	search_point(char c);
};

void set_char_color(char c) {

	switch (c) {
	case '#': set_text_color(conBROWN); break;
	case 'S': set_text_color(conYELLOW); break;
	case 'X': set_text_color(conLIGHTGREEN); break;
	case '.': set_text_color(conWHITE); break;
	}
}

void maze::print_map() {
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			set_char_color(map[i * w + j]);
			std::cout << map[i * w + j];
		}
		set_text_color(conWHITE);
		std::cout << "\n";
	}
}

void maze::print_map_ruler() {
	std::cout << "\t";
	for (int i = 0; i < w; i++) std::cout << i << "  ";
	std::cout << "\n\n";
	for (int i = 0; i < h; i++) {
		std::cout << i << "\t";
		for (int j = 0; j < w; j++) {
			std::cout << map[i * w + j] << "  ";
		}
		std::cout << "\n";
	}
}

void maze::show_layout_info() {
	print_map();
	std::cout << "\n\n";
	std::cout << "Numeration of each position begins in the upper-left corner on the map, in (0,0),\nand ends in the lower-right corner, in (w-1, h-1). The notation used is (column, row).\n";
	std::cout << "~ Width/columns (w): " << w << "\n~ Height/rows (h): " << h << "\n\n";
}

void maze::clear() {
	w = h = sx = sy = gx = gy = 0;
	goal_set = start_set = false;
	map.clear();
}

int maze::load_map(const std::string& filename) {
	std::ifstream map_file(filename);
	if (map_file.is_open()) {
		map_file >> w >> h;
		map.resize(w * h);
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				map_file >> map[i * w + j];
				if (map[i * w + j] == 'S') {
					sx = j; sy = i;
					start_set = true;
				}
				else if (map[i * w + j] == 'X') {
					gx = j; gy = i;
					goal_set = true;
				}
			}
			map_file.ignore(1, '\n');
		}
		map_file.close();
		return 1;
	}
	else return 0;
}

int maze::save_map(const std::string& filename) {
	std::ifstream f(filename);
	if (f.good()) {
		std::cout << "This file already exists. Do you want to overwrite it? Y/N\n";
		if (!y_n_prompt("==> ", "Invalid answer. Please, input either Y or N.")) return 0;
	}
	if (f.is_open()) f.close();

	std::ofstream output(filename, std::ios_base::trunc | std::ios_base::out);
	if (output.is_open()) {
		output << w << " " << h << "\n";
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++)
				output << map[i * w + j];
			output << "\n";
		}
		output.close();
		return 1;
	}
	else return 0;
}

std::pair<int, int> maze::search_point(char c) {
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			if (map[i * w + j] == c) return std::make_pair(i, j);
	return std::make_pair(-1, -1);
}

#endif // !MAZE_MAP_H
