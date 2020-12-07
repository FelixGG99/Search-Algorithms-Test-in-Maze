
/*
	File:			main.cpp
	Description:	Entry point, where the main loop is defined, and the options menu shows.
	Authors:		Name							ID:			Mat.:
					Félix Garduza Gutiérrez			3429399		24500597
					Axel Adrían Flores				3431762		24500628
					Ignacio Augusto Rodríguez Díaz	3424369		24500586
					Cindy Berenice Castro Aguilar	3376177		24400254
	Last updated:	06/12/2020
	Version:		1.0
*/

#include <iostream>				// cin, cout
#include <locale.h>				// setlocale

#include "search_algorithms.h"	// Search algorithms testing section
#include "map_operations.h"		// Maze class definition, save, load, etc
#include "map_editor.h"			// Map editor/creator section
#include "prompts.h"			// Prompts, console manipulation, formatting, colors

bool program_should_exit;		// Boolean to mark when the loop should end
bool map_loaded;				// Boolean to mark whether a map is currently loaded in the program or not

maze test_map;					// Maze instance where map, goal and start points, dimensions are stored

int main() {
	// Mexico locale
	setlocale(LC_ALL, "es-MX");

	// Set point to scroll back to after the operation is finished
	int scroll_point_x, scroll_point_y;
	wherexy(scroll_point_x, scroll_point_y);

	// Title and Main Menu
	title2(std::string("Welcome to the Maze Search Algorithms Simulator."), conCYAN, conBLUE);
	set_text_color(conCYAN);
	std::cout << "\n*****Operations*****\n\n";
	set_text_color(conWHITE);
	std::cout << "1) View current map\n";
	std::cout << "2) Test search algorithm\n";
	std::cout << "3) Build/edit map\n"; 
	std::cout << "4) Load map from file\n";
	std::cout << "5) Save map into file\n";
	std::cout << "6) Clear current map\n";
	std::cout << "7) Help\n";
	std::cout << "8) View project/authors info\n";
	std::cout << "9) Exit\n";

	// Set console coords to clear screen from after reading input and return cursor to position
	int ox, oy;
	wherexy(ox, oy);

	// Main loop: Reads input and calls to its corresponding function while program_should_exit!=true
	while (!program_should_exit) {
		// Read option from user
		int op = int_prompt_bounds("\t\tSelect an option number ==> ", "Invalid input. Type only numeric values.", 1, 9, "\n>Option not valid. Please, input only numeric values between 1 and 9.");
		switch (op) {
			// Show current map
			case 1: if (map_loaded) test_map.print_map(); else info_msg("No map was loaded into the program."); break;
			// Show algorithms testing menu
			case 2: if (map_loaded) search_menu(test_map); else info_msg("There isn't any map loaded in the program to test on."); break;
			// Show map editor
			case 3: build_map(test_map, map_loaded); break;
			// Load map
			case 4: if (map_loaded) {	warn_msg("There is already a map loaded into the program. Do you wish to overwrite it? Y/N?");
										if (!y_n_prompt("\t\t==> ", "Input either Y/N.")) break; }
					std::cout << "Input the name of the file the map is stored in.\n\n";
					if (test_map.load_map(string_prompt("\t\t==> "))) {
						succ_msg("Map loaded successfully!"); info_msg(std::string(std::to_string(test_map.h) + " rows and " + std::to_string(test_map.w) + " columns were loaded.").c_str());
						map_loaded = true;
					}
					else error_msg("Map could not be loaded. Please verify the file exists.");
					break;
			// Save current map
			case 5: if (map_loaded && test_map.save_map(string_prompt("Input the name of the file the map will be saved in ==> ")))
						succ_msg("Map saved succesfully.");
					else if (!map_loaded) info_msg("There is no map to save into an external file.");
					else error_msg("Map could not be saved."); 
					break;
			// Erase current map
			case 6: if (map_loaded) {
						warn_msg("Are you sure you want to delete the current map? Y/N");
						if (y_n_prompt("\t\t==> ", "Input either Y/N.")) { test_map.clear(); map_loaded = 0; }
					} else info_msg("There isn't any map loaded in the program to delete."); break;
			// Show help 
			case 7: help(); break;
			// Show info
			case 8: info(); break;
			// Close program
			case 9: program_should_exit = true; break;
		}
		// Wait until user presses key in order to allow them to view the results of the operation. 
		wait_key();

		// Return cursor to (ox,oy), scroll console view to scroll_point and clear everything from (ox,oy) onwards.
		gotoxy(ox, oy);
		clear_from(ox, oy);
		scroll_up_to(scroll_point_x, scroll_point_y);
	}
	std::cout << "Thanks for using our sim!\n";
	return 0;
}