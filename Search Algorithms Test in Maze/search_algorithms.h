#pragma once

/*
	File:			search_algorithms.h
	Description:	Header where all search algorithms functions are defined, as well as the search state structure.
	Authors:		Name							ID:			Mat.:
					Félix Garduza Gutiérrez			3429399		24500597
					Axel Adrían Flores				3431762		24500628
					Ignacio Augusto Rodríguez Díaz	3424369		24500586
					Cindy Berenice Castro Aguilar	3376177		24400254
	Last updated:	06/12/2020
	Version:		1.0
*/

#include <queue>			// queue and priority queue class
#include <array>			// array class
#include <vector>			// vector class
#include "map_operations.h"	// maze class definition		
#include "prompts.h"		// prompts and log messaging


const int n_dir = 4;		// Number of movements
int states_visited = 0;		// Counter of how many states have been visited during the current search
int m_ox, m_oy;				// Storage for console coords used for cleaning and moving the cursor

// Movements and real distances (used in A*)
int dx[4]	= { 1 , -1 ,  0 ,  0 };
int dy[4]	= { 0 ,  0 ,  1 , -1 };
int real_distance[4] = { 10, 10 , 10 , 10 };

// Search state structure
struct search_state {
	search_state(int ny, int nx, int ns) : x{ nx }, y{ ny }, steps{ ns } {}
	int x, y, steps;
};

void bfs(maze &m);														// line 129
int  dfs(maze& m, int x, int y, int steps, int lx, int ly);				// line 244
int  hill_climbing(maze& m, int x, int y, int steps, int lx, int ly);	// line 311
void best_first_search(maze& m);										// line 383
void a_star(maze& m);													// line 456

// Returns manhattan distance between p1 and p2
int manhattan(int x1, int y1, int x2, int y2) {
	return std::abs(x1 - x2) + std::abs(y2 - y1);
}

void search_menu(maze& m) {
	// indicates if search algorithm testing loop should end
	bool should_end_sim = false;
	std::cout << "\n";
	// save console coords to scroll back to once an operation ends
	int scr_px, scr_py;
	wherexy(scr_px, scr_py);

	// display title, current map and options menu
	title2(std::string("Search Algorithms Testing"),conLIGHTCYAN, conCYAN);
	m.show_layout_info();
	std::cout << "\t\tSelect a search algorithm to test:\n";
	std::cout << "1) Breadth First Search\n";
	std::cout << "2) Depth First Search\n";
	std::cout << "3) Hill Climbing\n";
	std::cout << "4) Best First Search\n";
	std::cout << "5) A*\n";
	std::cout << "6) End Simulations\n";

	// save console coords of where the prompt will start showing to clear from after the desired operation finishes
	int PromptStartCoordsX, PromptStartCoordsY;
	wherexy(PromptStartCoordsX, PromptStartCoordsY);

	// repeat while should_end_sim == false
	do {
		// read op
		int op = int_prompt("\t\t==> ", "Please, input only numeric values.");

		// if op valid...
		if (op > 0 && op < 6) {
			// save coords to scroll up to
			int sim_scr_px, sim_scr_py;
			wherexy(sim_scr_px, sim_scr_py);
			// display sim title
			std::cout << "Map Simulation:\n\n";
			// save coords of where simulations will start showing from
			wherexy(m_ox, m_oy);
			m.print_map();
			// scroll up to (sim_scr_px, sim_scr_py), to position console in a way that only the simulation is showing
			scroll_up_to(sim_scr_px, sim_scr_py);
		}

		// counts how many states were visited during the current search
		states_visited = 0;

		// calls the function corresponding to the selected algorithm
		switch (op) {
			case 1: bfs(m); break;
			case 2: dfs(m, m.sx, m.sy, 0, m.sx, m.sy); break;
			case 3: hill_climbing(m,m.sx,m.sy,0,m.sx, m.sy); break;
			case 4: best_first_search(m); break;
			case 5: a_star(m); break;
			case 6: should_end_sim = true; break;
			default: error_msg("Only options 1 to 6 are allowed."); break;
		}
		// if option selected is not exit, make program wait to allow user to view results
		if(op != 6) wait_key();

		// clear console from (ox, oy) and move cursor to said coords
		clear_from(PromptStartCoordsX, PromptStartCoordsY);
		gotoxy(PromptStartCoordsX, PromptStartCoordsY);

		// scroll console up to (scr_px, scr_py), where the option menu was printed
		scroll_up_to(scr_px, scr_py);
	} while (!should_end_sim);
}

// Marks the path followed during the search in the map after reaching the goal in bfs(), best_first_search() and a_star()
void show_path_marks(maze& m, int x, int y, char** marks, int ox, int oy) {
	do { // repeat while the current position is not the start point
		/*
			marks[][] keeps track not only of every position that has been visited - in the case of bfs(), best_first_search() and a_star(),
			it stores a char between '0' and the character corresponding to n_dir, indicating that, in order to reach a said position [y][x],
			the movement corresponding to adding dx[i] to x and dy[i] to y was used. such index i is calculated by substracting the value corresponding
			to '0' in decimal (check ASCII table) to the value stored in marks[y][x], since, when marked, the char used for marking was determined as the character
			whose value corresponded to '0' + the index of the movement used.
		*/
		// get the index of the kind of movement (dx[i], dy[i]) used to arrive to [y][x]. 
		int dir = marks[y][x] - '0';
		// move to the position previous to the current one moving in an inverse way to the one used to reach it.
		x -= dx[dir]; y -= dy[dir];
		// set color to blue and print it on the map
		set_text_color(conBLUE);
		gotoxy((short)(ox + x), (short)(oy+ y));
		std::cout << "*";
	}while (m.map[y * m.w + x] != 'S');
	// set start point char to blue too
	set_text_color(conBLUE);
	gotoxy(ox + m.sx, oy + m.sy);
	std::cout << "S";
	set_text_color(conWHITE);
}

void bfs(maze &m) {
	// Make sure both goal and start points exist in order to be able to perform a search
	if (!m.goal_set || !m.start_set) std::cout << "Start and/or goal points not set. Please, use the map editor\nto set these points to be able to perform a search.\n";
	else {
		// flag that indicates whenever the goal state has been reached
		bool goal_found = false;

		// create the state marking matrix 
		char** marks = new char* [m.h];
		for (int i = 0; i < m.h; i++)
			marks[i] = new char [m.w]();
		std::queue<search_state>states;

		// set the starting position mark as true and push it into the bfs queue
		marks[m.sy][m.sx] = true;
		states.push(search_state(m.sy, m.sx, 0));

		// save last x, last y as starting x, starting y
		int lx = m.sy;
		int ly = m.sx;

		// while there are states to be processed still in the queue...
		while (!states.empty()) {
			// retrieve the state at the front of the queue and remove it
			search_state current = states.front();
			states.pop();
			// increase states_visited
			states_visited++;
			// if current position is goal position
			if ((current.x == m.gx) && (current.y == m.gy)) {
				// move the cursor to the console coord corresponding to the last position on the simulated map
				gotoxy((short) m_ox +lx, (short)m_oy + ly);
				// replace it with a gray '*' to indicate it has been visited
				set_text_color(conDARKGRAY);
				std::cout << "*";
				// mark the followed path on the map
				show_path_marks(m,current.x, current.y, marks, m_ox, m_oy);
				set_text_color(conWHITE);
				// Move cursor past the map
				gotoxy(0, m_oy + m.h + 2);
				// Show results of the successful search
				std::cout << "Goal Reached!!\n";
				std::cout << "The path the simulation took that first arrived to the goal is marked in blue.\n";
				std::cout << "Starting Position: (" << m.sx << "," << m.sy << ")\n";
				std::cout << "Last Position: (" << current.x << "," << current.y << ")\n";
				std::cout << "Path length (moves taken): " << current.steps <<"\n";
				std::cout << "States visited during search: " << states_visited << "\n";
				// mark goal_found as true and end the while cycle
				goal_found = true; break;
			}
			// else, for each possible movement to perform as the next step
			else for (int i = 0; i < n_dir; i++) {

				// move the cursor to the console coord corresponding to the last position on the simulated map
				gotoxy((short)m_ox + lx, (short)m_oy + ly);
				// replace it with a gray '*' to indicate it has been visited
				set_text_color(conDARKGRAY);
				if (m.map[ly * m.w + lx] == 'S') std::cout << "S";
				else std::cout << "*";
				// move cursor to console coords where the current position is represented on the simulated map
				gotoxy((short)(m_ox + current.x), (short)(m_oy + current.y));
				// print blue 'V' to indicate the current position
				set_text_color(conLIGHTBLUE);
				std::cout << "V";
				// save last position as current position
				lx = current.x; ly = current.y;
				// Sleep program to allow update visualization
				Sleep(15);
				// create a copy of the current state and move its x and y coords using the dx[i] and dy[i] movement
				search_state child = current;
				child.x += dx[i];
				child.y += dy[i];

				// if the child state position is within map bounds, is not an obstacle and has not been visited...
				if (child.x >= 0 && child.x < m.w && child.y >= 0 && child.y < m.h && m.map[child.y * m.w + child.x] != '#' && !marks[child.y][child.x]) {
					// mark the position with a character representing the index value of the movement used to arrive to that position, increase steps and push into queue
					marks[child.y][child.x] = i + '0';
					child.steps++;
					states.push(child);
				}
			}
		}
		// if goal_found == false by the time the while loop ends (the goal was never found after visiting all reachable states)...
		if (!goal_found) {
			// move cursor past the simulation map and display failed search message.
			gotoxy((short)0, (short)(m_oy + m.h + 2));
			std::cout << "The goal could not be reached.\n";
			std::cout << "States visited during search : " << states_visited << "\n";
		}
		// de-allocate marks memory
		delete[] marks;
		marks = nullptr;
		set_text_color(conWHITE);
	}
}

// Depth-First Search recursive function. A call to this function returns 1 if it (or one of the further calls derived from it) found the goal, or 0 if the goal was never found.
int dfs(maze& m, int x, int y, int steps, int lx, int ly) {

	// declare state marking matrix pointer as static to keep it between function calls
	// set it to nullptr at declaration
	static char** marks = nullptr;

	// FIRST CALL OF DFS(): state marking matrix creation, search result catching, reporting in casce of failed search

	// if marks is null, then it must have been just declared
	if (!marks) {
		// allocate memory for the state marking matrix according to current map and assign it to marks
		marks = new char* [m.h];
		for (int i = 0; i < m.h; i++)
			marks[i] = new char[m.w]();
		// set starting position as true
		marks[m.sy][m.sx] = true;
		// call the dfs from the same position, but since marks is not null anymore, this "if" statement will not pass. Store the result returned in a variable.
		int result = dfs(m, x, y, steps, x, y);
		// de-allocate matrix memory since it will not be used anymore
		delete[] marks;
		marks = nullptr;
		// if the result returned from further calls to dfs() was 0, send failed search message.
		if (!result) {
			gotoxy((short)0, (short)(m_oy + m.h + 2)); 
			std::cout << "The goal position could not be reached.\n";
			std::cout << "States visited during search: " << states_visited << "\n";
		}
		return result;
	}
	// else, marks is pointing to something, that means it is not the first call of the search
	else {
		// increase how many states visited
		states_visited++;
		// if current position is the goal...
		if (m.map[y * m.w + x] == 'X') {
			// mark the last position with blue to indicate if takes part in the path followed that led to the goal
			gotoxy((short)m_ox + lx, (short)m_oy + ly);
			set_text_color(conLIGHTBLUE);
			std::cout << "*";
			set_text_color(conWHITE);

			// Move cursor past the simulation map
			gotoxy((short)0, (short)(m_oy + m.h + 2));

			// Show success message and report results
			std::cout << "Goal Reached!!\n";
			std::cout << "The path the simulation took that first arrived to the goal is marked in blue.\n";
			std::cout << "Starting Position: (" << m.sx << "," << m.sy << ")\n";
			std::cout << "Last Position: (" << x << "," << y << ")\n";
			std::cout << "Path length (moves taken): " << steps << "\n";
			std::cout << "States visited during search: " << states_visited << "\n";
			// return 1 to previous dfs() calls, indicating the goal was found and, therefore, a successful search
			return 1;
		}
		else {
			// mark the current position as visited
			marks[y][x] = true;
			set_text_color(conLIGHTBLUE);
			// move the cursor to the console coords corresponding to the previous position on the simulation map and print a light blue character to indicate it is part of the
			// path currently being followed and, possibly, of the path leading to the goal
			gotoxy((short)(m_ox + lx), (short)(m_oy + ly));
			if (lx == m.sx && ly == m.sy) std::cout << 'S';
			else std::cout << '*';
			// print blue 'V' to indicate the current position
			gotoxy((short)(m_ox + x), (short)(m_oy + y));
			set_text_color(conBLUE);
			std::cout << "V";
			set_text_color(conWHITE);
			// sleep the program to allow map update visualization
			sleep_(100);

			/*FOR LOOP OF MOVEMENTS: In this for loop, the "child states" of the current state are generated adding dx[i] to x and dy[i] to y - that is,
			the position of the child state after making certain movements parting from the current state.*/
			// for each possible movement to make the next step...
			for (int i = 0; i < n_dir; i++) {
				// save in nx, ny the new coords on the map after making the move i
				int nx = x + dx[i];
				int ny = y + dy[i];
				// if the new position is valid, has not been visited and is not an obstacle...
				if (marks && ny >= 0 && ny < m.h && nx >= 0 && nx < m.w && !marks[y + dy[i]][x + dx[i]] && m.map[(y + dy[i]) * m.w + x + dx[i]] != '#')
					// call dfs() recursively with the new position and number of steps increased in one, and if it returns 1 (succesful search)...
					if (dfs(m, nx, ny, steps + 1, x, y))
						// ...return 1 as well, indicating the goal was found in a further call to dfs() and the search was successful
						return 1;
			}
			
			/*END OF THE FOR LOOP: if the loop ends normally, that is, a return statement was never made because a dfs() call never returned 1 because
			* the goal was never found, then the current call must make the necessary arrangements for the case in which the search was unsuccessful and
			* return 0 recursively to make previous calls aware of this.*/
			// move the cursor to the console coords corresponding to the previous position on the simulation map and print a light gray character to indicate it was visited
			// (but discarded from the "active" path).
			gotoxy((short)(m_ox + x), (short)(m_oy + y));
			set_text_color(conDARKGRAY);
			if (x == m.sx && y == m.sy) std::cout << "S";
			else std::cout << "*";
			set_text_color(conWHITE);
			// sleep program to allow map update visualization
			sleep_(100);
			// return 0 recursively to let previous calls to dfs() know the search was unsuccessful
			return 0;
		}
	}
}

int hill_climbing(maze &m, int x, int y, int steps, int lx, int ly) {

	// declare state marking matrix pointer as static to keep it between function calls
	// set it to nullptr at declaration
	static char** marks = nullptr;

	// FIRST CALL OF DFS(): state marking matrix creation, search result catching, reporting in casce of failed search

	// if marks is null, then it must have been just declared
	if (!marks) {
		// allocate memory for the state marking matrix according to current map and assign it to marks
		marks = new char* [m.h];
		for (int i = 0; i < m.h; i++)
			marks[i] = new char[m.w]();
		// set starting position as true
		marks[m.sy][m.sx] = true;
		// call the dfs from the same position, but since marks is not null anymore, this "if" statement will not pass. Store the result returned in a variable.
		int result = hill_climbing(m, x, y, steps, x, y);
		// de-allocate matrix memory since it will not be used anymore
		delete[] marks;
		marks = nullptr;
		// if the result returned from further calls to dfs() was 0, send failed search message.
		if (!result) {
			gotoxy((short)0, (short)(m_oy + m.h + 2));
			std::cout << "The goal position could not be reached.\n";
			std::cout << "States visited during search: " << states_visited << "\n";
		}
		return result;
	}
	// else, marks is pointing to something, that means it is not the first call of the search
	else {
		// increase how many states visited
		states_visited++;
		// if current position is the goal...
		if (m.map[y * m.w + x] == 'X') {
			// mark the last position with blue to indicate if takes part in the path followed that led to the goal
			gotoxy((short)m_ox + lx, (short)m_oy + ly);
			set_text_color(conLIGHTBLUE);
			std::cout << "*";
			set_text_color(conWHITE);

			// Move cursor past the simulation map
			gotoxy((short)0, (short)(m_oy + m.h + 2));

			// Show success message and report results
			std::cout << "Goal Reached!!\n";
			std::cout << "The path the simulation took that first arrived to the goal is marked in blue.\n";
			std::cout << "Starting Position: (" << m.sx << "," << m.sy << ")\n";
			std::cout << "Last Position: (" << x << "," << y << ")\n";
			std::cout << "Path length (moves taken): " << steps << "\n";
			std::cout << "States visited during search: " << states_visited << "\n";
			// return 1 to previous dfs() calls, indicating the goal was found and, therefore, a successful search
			return 1;
		}
		else {
			// mark the current position as visited
			marks[y][x] = true;
			set_text_color(conLIGHTBLUE);
			// move the cursor to the console coords corresponding to the previous position on the simulation map and print a light blue character to indicate it is part of the
			// path currently being followed and, possibly, of the path leading to the goal
			gotoxy((short)(m_ox + lx), (short)(m_oy + ly));
			if (lx == m.sx && ly == m.sy) std::cout << 'S';
			else std::cout << '*';
			// print blue 'V' to indicate the current position
			gotoxy((short)(m_ox + x), (short)(m_oy + y));
			set_text_color(conBLUE);
			std::cout << "V";
			set_text_color(conWHITE);
			// sleep the program to allow map update visualization
			sleep_(100);

			/*HILL CLIMBING ALGORITHM: Of all the possible child states in the search, go to the one with the best heuristic. In this case, the
			heuristic is the manhattan distance from a certain position to the goal, and the best one is the lesser manhattan distance.*/

			// declare best_h, a variable that stores the best heuristic so far, and move_i, the index of the move that generates the state with the best
			// heuristic, both with placeholder values. If by the end of the search move_i == -1, no valid movement could be made.
			int best_h = NOT_VALID_INT2;
			int move_i = -1;
			// for each possible movement...
			for (int i = 0; i < n_dir; i++) {
				// save in nx, ny the new coords on the map after making the move i
				int nx = x + dx[i];
				int ny = y + dy[i];
				// if the new position is valid, has not been visited and is not an obstacle...
				if (marks && ny >= 0 && ny < m.h && nx >= 0 && nx < m.w && !marks[y + dy[i]][x + dx[i]] && m.map[(y + dy[i]) * m.w + x + dx[i]] != '#')
					// if the manhattan distance from the child position to the goal is better than the best one so far...
					if (manhattan(nx, ny, m.gx, m.gy) < best_h) {
						// replace the best one and the move
						best_h = manhattan(nx, ny, m.gx, m.gy);
						move_i = i;
					}
			}
			// variable to store the result of the call to hill_climbing() with the child state using moving to the position with the best heuristic.
			// it is set to 0 by defualt to account on the case move_i == -1, in which the search is considered to be failed.
			int success = 0;
			// if move_i != -1, that is, a valid move could be made (and is the one with the best heuristic), call hill_climbing() with the best child state
			if(move_i != -1)
				success = hill_climbing(m, x + dx[move_i], y + dy[move_i], steps + 1, x, y);
			// if success == false, meaning that the search did not reach the goal state...
			if (!success) {
				// Mark the current position on the map with a gray char to indicate it has been visited but it is not part of the "active" path
				gotoxy((short)(m_ox + x), (short)(m_oy + y));
				set_text_color(conDARKGRAY);
				if (x == m.sx && y == m.sy) std::cout << "S";
				else std::cout << "*";
				set_text_color(conWHITE);
			}
			// return the result of the search to let previous calls know if it failed or succeeded.
			return success;
		}
	}
}

void best_first_search(maze& m) {

	/*BEST FIRST SEARCH: Heuristic search that makes sure the goal is reached (if it is reachable) by searching all states, but always choosing the one with
	the best "calculated distance" as the next to process. In this case, the best "calculated distace" is the one with the lesser manhattan distance to the goal (an heuristic
	estimate). A priority queue is used and the state with the best calculated distance is always at the top - such is state is always picked as the next to process.*/

	// compare function to make the priority_queue sort by the heuristic, represented by the .second of the pair, in ascending order (the minumum value is always at the top)
	struct compare {
		bool operator()(const std::pair<search_state, int> &a, const std::pair<search_state, int> &b) {
			return a.second > b.second;
		}
	};
	// priority queue to store every state of the search and the heuristic corresponding to it in an std::pair structure
	// .first of the pair: state_search storing x, y, steps
	// .second of the pair: heuristic (manhattan distance) to the goal
	std::priority_queue<std::pair<search_state, int>, std::vector<std::pair<search_state, int>>, compare>pq;

	// create the state marking matrix 
	bool goal_found = false;
	char** marks = new char* [m.h];
	for (int i = 0; i < m.h; i++)
		marks[i] = new char[m.w]();
	// set the starting position mark as true and push it into the bfs queue
	marks[m.sy][m.sx] = true;
	pq.push(std::make_pair(search_state(m.sx, m.sx, 0),0));

	// save last x, last y as starting x, starting y
	int lx = m.sx;
	int ly = m.sy;

	// while there are states to be processed still in the queue...
	while (!pq.empty()) {
		// retrieve the state at the front of the queue and remove it
		states_visited++;
		std::pair<search_state,int> current = pq.top();
		pq.pop();


		// if current position is goal position
		if ((current.first.x == m.gx) && (current.first.y == m.gy)) {
			// move the cursor to the console coord corresponding to the last position on the simulated map
			gotoxy((short)m_ox + lx, (short)m_oy + ly);
			// replace it with a gray '*' to indicate it has been visited
			set_text_color(conDARKGRAY);
			std::cout << "*";
			// mark the followed path on the map
			show_path_marks(m, current.first.x, current.first.y, marks, m_ox, m_oy);
			set_text_color(conWHITE);
			// Move cursor past the map
			gotoxy(0, m_oy + m.h + 2);
			// Show results of the successful search
			std::cout << "Goal Reached!!\n";
			std::cout << "The path the simulation took that first arrived to the goal is marked in blue.\n";
			std::cout << "Starting Position: (" << m.sx << "," << m.sy << ")\n";
			std::cout << "Last Position: (" << current.first.x << "," << current.first.y << ")\n";
			std::cout << "Path length (moves taken): " << current.first.steps << "\n";
			std::cout << "States visited during search: " << states_visited << "\n";
			// mark goal_found as true and end the while cycle
			goal_found = true; break;
		}
		// else, for each possible movement to perform as the next step
		else for (int i = 0; i < n_dir; i++) {
			// move the cursor to the console coord corresponding to the last position on the simulated map
			gotoxy((short)m_ox + lx, (short)m_oy + ly);
			// replace it with a gray '*' to indicate it has been visited
			set_text_color(conDARKGRAY);
			if (m.map[ly * m.w + lx] == 'S') std::cout << "S";
			else std::cout << "*";
			// move cursor to console coords where the current position is represented on the simulated map
			gotoxy((short)(m_ox + current.first.x), (short)(m_oy + current.first.y));
			// print blue 'V' to indicate the current position
			set_text_color(conLIGHTBLUE);
			std::cout << "V";
			// save last position as current position
			lx = current.first.x; ly = current.first.y;
			// Sleep program to allow update visualization
			Sleep(15);
			
			// create a child state variable as a copy of the current state and modify its position with the move i (x + dx[i], y + dy[i])
			std::pair<search_state, int> child = current;
			child.first.x += dx[i];
			child.first.y += dy[i];

			// if the child state position is within map bounds, is not an obstacle and has not been visited...
			if (child.first.x >= 0 && child.first.x < m.w && child.first.y >= 0 && child.first.y < m.h && m.map[child.first.y * m.w + child.first.x] != '#' && !marks[child.first.y][child.first.x]) {
				// mark the position with a character representing the index value of the movement used to arrive to that position
				marks[child.first.y][child.first.x] = i + '0';
				// increase steps
				child.first.steps++;
				// set state heuristic value as heuristic distance (manhattan distance from child distance to goal)
				child.second = manhattan(child.first.x, child.first.y, m.gx, m.gy);
				// push into priority queue
				pq.push(child);
			}
		}
	}
	// if goal_found == false by the time the while loop ends (the goal was never found after visiting all reachable states)...
	if (!goal_found) {
		// move cursor past the simulation map and display failed search message.
		gotoxy((short)0, (short)(m_oy + m.h + 2));
		std::cout << "The goal could not be reached.\n";
		std::cout << "States visited during search : " << states_visited << "\n";
	}
	// de-allocate marks memory
	delete[] marks;
	marks = nullptr;
	set_text_color(conWHITE);
}

void a_star(maze& m) {

	/* A*: Heuristic search that makes sure the goal is reached (if it is reachable) by searching all states, but always choosing the one with
	the best "calculated distance" as the next to process. It is more precise than the best first search since it also takes into account a real distance.
	In this case, the calculated distance uses both the best heuristic distance, which is the one with the lesser manhattan distance from the child position to the goal,
	and the real distance, which is the actual distance from the current position to the child position. A priority queue is used and the state with the best calculated 
	distance is always at the top - such is state is always picked as the next to process.*/

	// compare function to make the priority_queue sort by the heuristic, represented by the .second of the pair, in ascending order (the minumum value is always at the top)
	struct compare {
		bool operator()(const std::pair<search_state, int>& a, const std::pair<search_state, int>& b) {
			return a.second > b.second;
		}
	};
	// priority queue to store every state of the search and the heuristic corresponding to it in an std::pair structure
	// .first of the pair: state_search storing x, y, steps
	// .second of the pair: heuristic (manhattan distance) to the goal
	std::priority_queue<std::pair<search_state, int>, std::vector<std::pair<search_state, int>>, compare>pq;

	// create the state marking matrix 
	bool goal_found = false;
	char** marks = new char* [m.h];
	for (int i = 0; i < m.h; i++)
		marks[i] = new char[m.w]();
	// set the starting position mark as true and push it into the bfs queue
	marks[m.sy][m.sx] = true;
	pq.push(std::make_pair(search_state(m.sx, m.sx, 0), 0));

	// save last x, last y as starting x, starting y
	int lx = m.sx;
	int ly = m.sy;

	// while there are states to be processed still in the queue...
	while (!pq.empty()) {
		// retrieve the state at the front of the queue and remove it
		states_visited++;
		std::pair<search_state, int> current = pq.top();
		pq.pop();


		// if current position is goal position
		if ((current.first.x == m.gx) && (current.first.y == m.gy)) {
			// move the cursor to the console coord corresponding to the last position on the simulated map
			gotoxy((short)m_ox + lx, (short)m_oy + ly);
			// replace it with a gray '*' to indicate it has been visited
			set_text_color(conDARKGRAY);
			std::cout << "*";
			// mark the followed path on the map
			show_path_marks(m, current.first.x, current.first.y, marks, m_ox, m_oy);
			set_text_color(conWHITE);
			// Move cursor past the map
			gotoxy(0, m_oy + m.h + 2);
			// Show results of the successful search
			std::cout << "Goal Reached!!\n";
			std::cout << "The path the simulation took that first arrived to the goal is marked in blue.\n";
			std::cout << "Starting Position: (" << m.sx << "," << m.sy << ")\n";
			std::cout << "Last Position: (" << current.first.x << "," << current.first.y << ")\n";
			std::cout << "Path length (moves taken): " << current.first.steps << "\n";
			std::cout << "States visited during search: " << states_visited << "\n";
			// mark goal_found as true and end the while cycle
			goal_found = true; break;
		}
		// else, for each possible movement to perform as the next step
		else for (int i = 0; i < n_dir; i++) {
			// move the cursor to the console coord corresponding to the last position on the simulated map
			gotoxy((short)m_ox + lx, (short)m_oy + ly);
			// replace it with a gray '*' to indicate it has been visited
			set_text_color(conDARKGRAY);
			if (m.map[ly * m.w + lx] == 'S') std::cout << "S";
			else std::cout << "*";
			// move cursor to console coords where the current position is represented on the simulated map
			gotoxy((short)(m_ox + current.first.x), (short)(m_oy + current.first.y));
			// print blue 'V' to indicate the current position
			set_text_color(conLIGHTBLUE);
			std::cout << "V";
			// save last position as current position
			lx = current.first.x; ly = current.first.y;
			// Sleep program to allow update visualization
			Sleep(15);

			// create a child state variable as a copy of the current state and modify its position with the move i (x + dx[i], y + dy[i])
			std::pair<search_state, int> child = current;
			child.first.x += dx[i];
			child.first.y += dy[i];

			// if the child state position is within map bounds, is not an obstacle and has not been visited...
			if (child.first.x >= 0 && child.first.x < m.w && child.first.y >= 0 && child.first.y < m.h && m.map[child.first.y * m.w + child.first.x] != '#' && !marks[child.first.y][child.first.x]) {
				// mark the position with a character representing the index value of the movement used to arrive to that position
				marks[child.first.y][child.first.x] = i + '0';
				// increase steps
				child.first.steps++;
				// set state heuristic value as real distance (from current position to child position) +  heuristic distance (manhattan distance from child distance to goal)
				child.second = real_distance[i] + manhattan(child.first.x, child.first.y, m.gx, m.gy);
				// push into priority queue
				pq.push(child);
			}
		}
	}
	// if goal_found == false by the time the while loop ends (the goal was never found after visiting all reachable states)...
	if (!goal_found) {
		// move cursor past the simulation map and display failed search message.
		gotoxy((short)0, (short)(m_oy + m.h + 2));
		std::cout << "The goal could not be reached.\n";
		std::cout << "States visited during search : " << states_visited << "\n";
	}
	// de-allocate marks memory
	delete[] marks;
	marks = nullptr;
	set_text_color(conWHITE);
}