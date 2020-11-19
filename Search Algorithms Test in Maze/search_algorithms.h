#pragma once

#include <queue>
#include <array>
#include "map_operations.h"
#include "prompts.h"

int states_visited = 0;
int dx[4]		= {  0 ,  0 , -1 ,  1 };
int dy[4]		= { -1 ,  1 ,  0 ,  0 };
int marks_dir[4]	= { 'D', 'U', 'R', 'L' };

struct bfs_state {
	bfs_state(int ny, int nx, int ns) : x{ nx }, y{ ny }, steps{ ns } {}
	int x, y, steps;
};

void bfs(maze &m, int ox, int oy);
int  dfs(maze& m, int x, int y, int steps, char** marks, int lx, int ly);

void search_menu(maze &m) {
	bool should_end_sim = false;
	std::cout << "\n";
	set_text_color(conYELLOW);
	std::cout << "\t\t////////////////////////////\n";
	std::cout << "\t\t// SEARCH ALGORITHMS TEST //\n";
	std::cout << "\t\t////////////////////////////\n";
	set_text_color(conWHITE);
	m.show_layout_info();
	std::cout << "\t\tSelect a search algorithm to test:\n";
	std::cout << "1) Breadth First Search\n";
	std::cout << "2) Depth First Search\n";
	std::cout << "3) Hill Climbing\n";
	std::cout << "4) Best First Search\n";
	std::cout << "5) A*\n";
	std::cout << "6) End Simulations\n";

	int ox, oy;
	wherexy(ox, oy);

	do {
		switch (int op = int_prompt("\t\t==> ", "Please, input only numeric values.")) {
		case 1: bfs(m, ox, oy); break;
		case 2: dfs(m, m.sx, m.sy, 0, nullptr, m.sx, m.sy); wait_key(); break;
		case 3: break;
		case 4: break;
		case 5: break;
		case 6: should_end_sim = true; break;
		default: break;
		}
		clear_from(ox, oy);
		gotoxy(ox, oy);
	} while (!should_end_sim);
}

void set_bfs_path(maze& m, int x, int y, char** marks, int ox, int oy) {
	do {
		switch (marks[y][x]) {
		case 'D': x -= dx[0]; y -= dy[0]; break;
		case 'U': x -= dx[1]; y -= dy[1]; break;
		case 'R': x -= dx[2]; y -= dy[2]; break;
		case 'L': x -= dx[3]; y -= dy[3]; break;
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

void bfs(maze &m, int ox, int oy) {
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
		m.print_map();
		// lx,ly shouldnt be ox,oy
		int lx = ox;
		int ly = oy;

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
				// Move cursor past the map
				gotoxy(0, oy + m.h + 2);
				sleep_(1000);
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
				child.x += dx[i];
				child.y += dy[i];
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
		wait_key();
	}
}

int dfs(maze& m, int x, int y, int steps, char** marks, int lx, int ly) {

	static int ox, oy;

	if (!marks) {
		std::cout << "Map Simulation:\n\n";
		wherexy(ox, oy);
		m.print_map();
		
		char** marks = new char* [m.h];
		for (int i = 0; i < m.h; i++)
			marks[i] = new char[m.w]();
		marks[m.sy][m.sx] = true;
		lx = ox;
		ly = oy;
	}

	states_visited++;

	if (m.map[y * m.w + x] == 'X') {
		gotoxy((short)lx, (short)ly);
		set_text_color(conLIGHTBLUE);
		std::cout << "*";
		set_text_color(conWHITE);
		
		// Move cursor past the map
		gotoxy(0, oy + m.h + 2);
		sleep_(1000);
		std::cout << "Goal Reached!!\n";
		std::cout << "The path the simulation took that first arrived to the goal is marked in blue.\n";
		std::cout << "Starting Position: (" << m.sx << "," << m.sy << ")\n";
		std::cout << "Last Position: (" << x << "," << y << ")\n";
		std::cout << "Path length (moves taken): " << steps << "\n";
		std::cout << "States visited during search: " << states_visited << "\n";
		return 1;
	}
	else{
		set_text_color(conLIGHTBLUE);
		gotoxy(ox + lx, oy + ly);
		if (lx == m.sx && ly == m.sy) std::cout << 'S';
		else std::cout << '*';
		gotoxy(ox + x, oy + y);
		set_text_color(conBLUE);
		std::cout << "V";
		set_text_color(conWHITE);
		sleep_(20);
		for (int i = 0; i < 4; i++)
			if (marks && !marks[y + dy[i]][x + dx[i]] && y + dy[i] >= 0 && y + dy[i] < m.h && x + dx[i] >= 0 && x + dx[i] < m.w && m.map[(y+dy[i])*m.w+x+dx[i]]!='#' 
				&& dfs(m, x + dx[i], y + dy[i], steps + 1, marks, x, y))
					return 1;
		gotoxy(ox + y, oy + y);
		std::cout << ".";
		sleep_(20);
		return 0;
	}
}