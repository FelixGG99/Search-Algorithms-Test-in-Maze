#pragma once

#include <queue>
#include <array>
#include "map_operations.h"
#include "prompts.h"

int states_visited = 0;
int bfs_dx[4]		= {  0 ,  0 , -1 ,  1 };
int bfs_dy[4]		= { -1 ,  1 ,  0 ,  0 };
int marks_dir[4]	= { 'D', 'U', 'R', 'L' };

struct bfs_state {
	bfs_state(int ny, int nx, int ns) : x{ nx }, y{ ny }, steps{ ns } {}
	int x, y, steps;
};

void bfs(maze &m);

void search_menu(maze &m) {
	bool should_end_sim = false;
	do {
		std::cout << "\n";
		std::cout << "\t\t////////////////////////////\n";
		std::cout << "\t\t// SEARCH ALGORITHMS TEST //\n";
		std::cout << "\t\t////////////////////////////\n";
		m.show_layout_info();
		std::cout << "\t\tSelect a search algorithm to test:\n";
		std::cout << "1) Breadth First Search\n";
		std::cout << "2) Depth First Search\n";
		std::cout << "3) Hill Climbing\n";
		std::cout << "4) Best First Search\n";
		std::cout << "5) A*\n";
		std::cout << "6) End Simulations\n";

		switch (int op = int_prompt("\t\t==> ", "Please, input only numeric values.")) {
		case 1: bfs(m); break;
		case 2: break;
		case 3: break;
		case 4: break;
		case 5: break;
		case 6: should_end_sim = true; break;
		default: break;
		}
		std::cout << "Press Enter to continue.\n";
		std::cout << "------------------------------------------------------------\n";
		std::cin.ignore();
	} while (!should_end_sim);
}

void set_bfs_path(maze& m, int x, int y, char** marks, int ox, int oy) {
	do {
		switch (marks[y][x]) {
		case 'D': x -= bfs_dx[0]; y -= bfs_dy[0]; break;
		case 'U': x -= bfs_dx[1]; y -= bfs_dy[1]; break;
		case 'R': x -= bfs_dx[2]; y -= bfs_dy[2]; break;
		case 'L': x -= bfs_dx[3]; y -= bfs_dy[3]; break;
		}
		set_text_color(conBLUE);
		gotoxy((short)(ox + x), (short)(oy+ y));
		std::cout << "*";
	}while (m.map[y * m.w + x] != 'S');
	set_text_color(conBLUE);
	gotoxy(ox + m.sx, oy + m.sy);
	std::cout << "S";
	set_text_color(conWHITE);
}

void bfs(maze &m) {
	if (!m.goal_set || !m.start_set) std::cout << "Start and/or goal points not set. Please, use the map editor\nto set these points to be able to perform a search.\n";
	else {
		states_visited = 0;
		bool goal_found = false;
		char** marks = new char* [m.h];
		for (int i = 0; i < m.h; i++)
			marks[i] = new char [m.w]();
		std::queue<bfs_state>states;
		marks[m.sy][m.sx] = true;
		states.push(bfs_state(m.sy, m.sx, 0));

		std::cout << "Map Simulation:\n\n";
		int ox, oy;
		wherexy(ox, oy);
		int lx = ox;
		int ly = oy;
		int fy = oy + m.h + 2;
		m.print_map();

		while (!states.empty()) {
			bfs_state current = states.front();
			states.pop();
			states_visited++;
			if ((current.x == m.gx) && (current.y == m.gy)) {
				gotoxy((short)lx, (short)ly);
				set_text_color(conDARKGRAY);
				std::cout << "*";
				gotoxy(ox, oy);
				set_bfs_path(m,current.x, current.y, marks, ox, oy);
				set_text_color(conWHITE);
				gotoxy(0, fy);
				Sleep(1000);
				std::cout << "Goal Reached!!\n";
				std::cout << "The path the simulation took that first arrived to the goal is marked in blue.\n";
				std::cout << "Starting Position: (" << m.sx << "," << m.sy << ")\n";
				std::cout << "Last Position: (" << current.x << "," << current.y << ")\n";
				std::cout << "Path length (moves taken): " << current.steps <<"\n";
				std::cout << "States visited during search: " << states_visited << "\n";
				goal_found = true; break;
			}
			else for (int i = 0; i < 4; i++) {
				gotoxy((short)lx, (short)ly);
				set_text_color(conDARKGRAY);
				std::cout << "*";
				gotoxy((short)(ox + current.x), (short)(oy + current.y));
				set_text_color(conLIGHTBLUE);
				std::cout << "V";
				lx = ox + current.x; ly = oy + current.y;
				Sleep(50);
				bfs_state child = current;
				child.x += bfs_dx[i];
				child.y += bfs_dy[i];
				if (child.x >= 0 && child.x < m.w && child.y >= 0 && child.y < m.h && m.map[child.y * m.w + child.x] != '#' && !marks[child.y][child.x]) {
					marks[child.y][child.x] = marks_dir[i];
					child.steps++;
					states.push(child);
				}
			}
		}
		if (!goal_found) {
			std::cout << "The goal could not be reached.\n";
			std::cout << "States visited during search : " << states_visited << "\n";
		}
		set_text_color(conWHITE);
	}
}