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

	setlocale(LC_ALL, "es-MX");
	set_text_color(conBROWN);
	std::cout << "\t\t//////////////////////////////////////////////////////////////////\n";
	std::cout << "\t\t//////// Welcome to the Maze Search Algorithms Simulator. ////////\n";
	std::cout << "\t\t//////////////////////////////////////////////////////////////////\n";
	std::cout << "\n";

	while (!program_should_exit) {
		set_text_color(conYELLOW);
		std::cout << "\n*****Operations*****\n\n";
		set_text_color(conWHITE);
		std::cout << "1) View current map\n";
		std::cout << "2) Test search algorithm\n";
		if(!map_loaded)	std::cout << "3) Build new map\n"; else std::cout << "3) Edit current map\n";
		std::cout << "4) Load map from file\n";
		std::cout << "5) Save map into file\n";
		std::cout << "6) Clear current map\n";
		std::cout << "7) View project/authors info\n";
		std::cout << "8) Clear screen\n";
		std::cout << "9) Exit\n";
		int op = int_prompt_bounds("\t\tSelect an option number ==> ", "Invalid input. Type only numeric values.", 1, 9, "Option not valid. Please, input only numeric values between 1 and 9.");
		std::cout << "------------------------------------------------------------\n";
		switch (op) {
			case 1: if (map_loaded) test_map.print_map(); else std::cout << "No map is loaded into the program.\n"; break;
			case 2: search_menu(test_map); break;
			case 3: if (!map_loaded && build_map(test_map)) map_loaded = true; else edit_map(test_map); break;
			case 4: if (test_map.load_map(string_prompt("\t\tInput the name of the file the map is stored in ==> "))) {
						 std::cout << "Map loaded successfully, with " << test_map.h << " rows and " << test_map.w << " columns.\n"; map_loaded = true; }
					else std::cout << "Map could not be loaded. Please verify the file exists.\n"; 
					break;
			case 5: if (test_map.save_map(string_prompt("Input the name of the file the map will be saved in ==> ")))
						 std::cout << "Map saved succesfully.";
					else std::cout << "Map could not be saved.\n"; 
					break;
			case 6: std::cout << "Are you sure you want to delete the current map? Y/N\n"; 
					if (y_n_prompt("\t\t==> ", "Input either Y/N.")) { 
						test_map.clear(); map_loaded = 0;
					} break;
			case 7: info(); break;
			case 8: CLEAR_SCREEN; break;
			case 9: program_should_exit = true; break;
		}
		std::cout << "\nPress Enter to continue.\n";
		std::cin.ignore();
		std::cout << "-----------------------------------------------------------------------------------------------\n";
	}
	std::cout << "Thanks for using our sim!\n";
	return 0;
}