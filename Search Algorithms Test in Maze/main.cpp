#include <iostream>
#include <locale.h>

#include "search_algorithms.h"
#include "map_operations.h"
#include "map_editor.h"
#include "prompts.h"

bool program_should_exit;
bool map_loaded;

maze test_map;

int main() {
	// Mexico locale
	setlocale(LC_ALL, "es-MX");

	// Set point to scroll back to
	int scroll_point_x, scroll_point_y;
	wherexy(scroll_point_x, scroll_point_y);

	// Title and Main Menu
	set_text_color(conBROWN);
	title2(std::string("Welcome to the Maze Search Algorithms Simulator."), conLIGHTMAGENTA, conBROWN);
	set_text_color(conYELLOW);
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

	// Set point to clear screen from after reading input
	int ox, oy;
	wherexy(ox, oy);

	// Main loop: Reads input and calls to its corresponding function
	while (!program_should_exit) {
		int op = int_prompt_bounds("\t\tSelect an option number ==> ", "Invalid input. Type only numeric values.", 1, 9, "\n>Option not valid. Please, input only numeric values between 1 and 9.");
		switch (op) {
			// Show current map
			case 1: if (map_loaded) test_map.print_map(); else std::cout << "\n>No map was loaded into the program.\n"; break;
			// Show algorithms testing menu
			case 2: if (map_loaded) search_menu(test_map); else std::cout << "\nThere isn't any map loaded in the program to test on.\n"; break;
			// Show map editor
			case 3: build_map(test_map, map_loaded); break;
			// Load map
			case 4: if (map_loaded) {	std::cout << "There is already a map loaded into the program. Do you wish to overwrite it? Y/N?\n";
										if (!y_n_prompt("\t\t==> ", "Input either Y/N.")) break; }
					std::cout << "Input the name of the file the map is stored in.\n\n";
					if (test_map.load_map(string_prompt("\t\t==> "))) {
						succ_msg("Map loaded successfully!"); info_msg(std::string(std::to_string(test_map.h) + " rows and " + std::to_string(test_map.w) + " columns were loaded.").c_str());
						map_loaded = true;
					}
					else std::cout << "\n>Map could not be loaded. Please verify the file exists.\n";
					break;
			// Save current map
			case 5: if (map_loaded && test_map.save_map(string_prompt("Input the name of the file the map will be saved in ==> ")))
						std::cout << "\n>Map saved succesfully.";
					else if (!map_loaded) std::cout << "There is no map to save into an external file.\n";
					else std::cout << "\n>Map could not be saved.\n"; 
					break;
			// Erase current map
			case 6: if (map_loaded) {
						std::cout << "\n>Are you sure you want to delete the current map? Y/N\n";
						if (y_n_prompt("\t\t==> ", "Input either Y/N.")) { test_map.clear(); map_loaded = 0; }
					} else std::cout << "\nThere isn't any map loaded in the program to delete.\n"; break;
			// Show help 
			case 7: help(); break;
			// Show info
			case 8: info(); break;
			// Close program
			case 9: program_should_exit = true; break;
		}
		wait_key();
		gotoxy(ox, oy);
		clear_from(ox, oy);
		scroll_up_to(scroll_point_x, scroll_point_y);
	}
	std::cout << "Thanks for using our sim!\n";
	return 0;
}