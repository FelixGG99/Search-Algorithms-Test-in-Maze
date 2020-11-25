#pragma once

#include <queue>
#include <array>
#include <vector>
#include "map_operations.h"
#include "prompts.h"

const int n_dir = 8;
int states_visited = 0;
int m_ox, m_oy;
int dx[8]			= {  0 ,  0 , -1 ,  1 ,  1 ,  1 , -1 , -1};
int dy[8]			= { -1 ,  1 ,  0 ,  0 ,  1 , -1 ,  1 , -1};
int cost[8]			= { 10 , 10 , 10 , 10 , 14,  14 , 14 , 14};

struct search_state {
	search_state(int ny, int nx, int ns) : x{ nx }, y{ ny }, steps{ ns } {}
	int x, y, steps;
};

void bfs(maze &m);
int  dfs_unopt(maze& m, int x, int y, int steps, int lx, int ly);
int  dfs_opt(maze& m, int x, int y, int steps, int lx, int ly);
int  hill_climbing(maze& m, int x, int y, int steps, int lx, int ly);
void best_first_search(maze& m);
void a_star(maze& m);

int manhattan(int x1, int y1, int x2, int y2) {
	return std::abs(x1 - x2) + std::abs(y2 - y1);
}

void search_menu(maze &m) {
	bool should_end_sim = false;
	std::cout << "\n";
	int scr_px, scr_py;
	wherexy(scr_px, scr_py);
	title2(std::string("Search Algorithms Testing"),conLIGHTCYAN, conCYAN);
	m.show_layout_info();
	std::cout << "\t\tSelect a search algorithm to test:\n";
	std::cout << "1) Breadth First Search\n";
	std::cout << "2) Depth First Search (un-optimized)\n";
	std::cout << "3) Depth First Search (optimized)\n";
	std::cout << "4) Hill Climbing\n";
	std::cout << "5) Best First Search\n";
	std::cout << "6) A*\n";
	std::cout << "7) End Simulations\n";

	int ox, oy;
	wherexy(ox, oy);

	do {
		int op = int_prompt("\t\t==> ", "Please, input only numeric values.");

		if (op > 0 && op < 7) {
			int sim_scr_px, sim_scr_py;
			wherexy(sim_scr_px, sim_scr_py);
			std::cout << "Map Simulation:\n\n";
			wherexy(m_ox, m_oy);
			m.print_map();
			scroll_up_to(sim_scr_px, sim_scr_py);
		}

		states_visited = 0;

		switch (op) {
		case 1: bfs(m); break;
		case 2: dfs_unopt(m, m.sx, m.sy, 0, m.sx, m.sy); break;
		case 3: dfs_opt(m, m.sx, m.sy, 0, m.sx, m.sy); break;
		case 4: hill_climbing(m,m.sx,m.sy,0,m.sx, m.sy); break;
		case 5: best_first_search(m); break;
		case 6: a_star(m); break;
		case 7: should_end_sim = true; break;
		default: error_msg("Only options 1 to 7 are allowed."); break;
		}
		if(op != 7) wait_key();
		clear_from(ox, oy);
		gotoxy(ox, oy);
		scroll_up_to(scr_px, scr_py);
	} while (!should_end_sim);
}

void set_bfs_path(maze& m, int x, int y, char** marks, int ox, int oy) {
	do {
		int dir = marks[y][x] - '0';
		x -= dx[dir]; y -= dy[dir];
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
		bool goal_found = false;

		/*std::cout << "Following the principle of a BFS, this search uses a queue: the algorithm takes the\n state at the front of the queue and processes it. For each state visited, new child\n states are generated and pushed to the end of the queue, as long as such state has\n" +
			"not been visited before.In this specific case, states are defined only as a certain\n position(x, y), since we are only interested in reaching the goal, if possible, and \n without any special conditions.However, this algorithm ALWAYS provides the shortest\n"+
			"(or one of the shortest solutions, if there is a tie) path to the goal, under the\n"+
			"assumption that, since states are processed in order of generation, the first time\n"+
			"the goal state is encountered, it is with the least steps required.";
		*/

		char** marks = new char* [m.h];
		for (int i = 0; i < m.h; i++)
			marks[i] = new char [m.w]();
		std::queue<search_state>states;
		marks[m.sy][m.sx] = true;
		states.push(search_state(m.sy, m.sx, 0));

		// lx,ly shouldnt be ox,oy
		int lx = m.sy;
		int ly = m.sx;

		while (!states.empty()) {
			search_state current = states.front();
			states.pop();
			states_visited++;
			if ((current.x == m.gx) && (current.y == m.gy)) {
				gotoxy((short) m_ox +lx, (short)m_oy + ly);
				set_text_color(conDARKGRAY);
				std::cout << "*";
				gotoxy(m_ox, m_oy);
				set_bfs_path(m,current.x, current.y, marks, m_ox, m_oy);
				set_text_color(conWHITE);
				// Move cursor past the map
				gotoxy(0, m_oy + m.h + 2);
				sleep_(1000);
				std::cout << "Goal Reached!!\n";
				std::cout << "The path the simulation took that first arrived to the goal is marked in blue.\n";
				std::cout << "Starting Position: (" << m.sx << "," << m.sy << ")\n";
				std::cout << "Last Position: (" << current.x << "," << current.y << ")\n";
				std::cout << "Path length (moves taken): " << current.steps <<"\n";
				std::cout << "States visited during search: " << states_visited << "\n";
				goal_found = true; break;
			}
			else for (int i = 0; i < n_dir; i++) {
				gotoxy((short) m_ox + lx, (short) m_oy + ly);
				set_text_color(conDARKGRAY);
				if (m.map[ly * m.w + lx] == 'S') std::cout << "S";
				else std::cout << "*";
				gotoxy((short)(m_ox + current.x), (short)(m_oy + current.y));
				set_text_color(conLIGHTBLUE);
				std::cout << "V";
				lx = current.x; ly = current.y;
				Sleep(25);
				search_state child = current;
				child.x += dx[i];
				child.y += dy[i];
				if (child.x >= 0 && child.x < m.w && child.y >= 0 && child.y < m.h && m.map[child.y * m.w + child.x] != '#' && !marks[child.y][child.x]) {
					marks[child.y][child.x] = i + '0';
					child.steps++;
					states.push(child);
				}
			}
		}
		if (!goal_found) {
			gotoxy((short)0, (short)(m_oy + m.h + 2));
			std::cout << "The goal could not be reached.\n";
			std::cout << "States visited during search : " << states_visited << "\n";
		}
		delete[] marks;
		marks = nullptr;
		set_text_color(conWHITE);
	}
}

int dfs_unopt(maze& m, int x, int y, int steps, int lx, int ly) {

	static char **marks = nullptr;

	if (!marks) {
		marks = new char* [m.h];
		for (int i = 0; i < m.h; i++)
			marks[i] = new char[m.w]();
		marks[m.sy][m.sx] = true;
		int result = dfs_unopt(m, x, y, steps, x, y);
		delete[] marks;
		marks = nullptr;
		if (!result) {
			gotoxy((short)0, (short)(m_oy + m.h + 2));
			std::cout << "The goal position could not be reached.\n";
			std::cout << "States visited during search: " << states_visited << "\n";
		}
		return result;
	}
	else {
		states_visited++;

		if (m.map[y * m.w + x] == 'X') {
			gotoxy((short)m_ox + lx, (short)m_oy + ly);
			set_text_color(conLIGHTBLUE);
			std::cout << "*";
			set_text_color(conWHITE);

			// Move cursor past the map
			gotoxy((short)0, (short)(m_oy + m.h + 2));
			std::cout << "Goal Reached!!\n";
			std::cout << "The path the simulation took that first arrived to the goal is marked in blue.\n";
			std::cout << "Starting Position: (" << m.sx << "," << m.sy << ")\n";
			std::cout << "Last Position: (" << x << "," << y << ")\n";
			std::cout << "Path length (moves taken): " << steps << "\n";
			std::cout << "States visited during search: " << states_visited << "\n";
			return 1;
		}
		else {
			marks[y][x] = true;
			set_text_color(conLIGHTBLUE);
			gotoxy((short)(m_ox + lx), (short)(m_oy + ly));
			if (lx == m.sx && ly == m.sy) std::cout << 'S';
			else std::cout << '*';
			gotoxy((short)(m_ox + x), (short)(m_oy + y));
			set_text_color(conBLUE);
			std::cout << "V";
			set_text_color(conWHITE);
			sleep_(100);
			for (int i = 0; i < n_dir; i++) {
				int nx = x + dx[i];
				int ny = y + dy[i];
				if (marks && ny >= 0 && ny < m.h && nx >= 0 && nx < m.w && !marks[y + dy[i]][x + dx[i]] && m.map[(y + dy[i]) * m.w + x + dx[i]] != '#')
					if (dfs_unopt(m, nx, ny, steps + 1, x, y))
						return 1;
			}
			gotoxy((short)(m_ox + x), (short)(m_oy + y));
			if (x == m.sx && y == m.sy) std::cout << "S";
			else std::cout << ".";
			sleep_(100);
			marks[y][x] = false;
			return 0;
		}
	}
}

int dfs_opt(maze& m, int x, int y, int steps, int lx, int ly) {

	static char** marks = nullptr;

	if (!marks) {
		marks = new char* [m.h];
		for (int i = 0; i < m.h; i++)
			marks[i] = new char[m.w]();
		marks[m.sy][m.sx] = true;
		int result = dfs_opt(m, x, y, steps, x, y);
		delete[] marks;
		marks = nullptr;
		if (!result) {
			gotoxy((short)0, (short)(m_oy + m.h + 2)); 
			std::cout << "The goal position could not be reached.\n";
			std::cout << "States visited during search: " << states_visited << "\n";
		}
		return result;
	}
	else {
		states_visited++;

		if (m.map[y * m.w + x] == 'X') {
			gotoxy((short)m_ox + lx, (short)m_oy + ly);
			set_text_color(conLIGHTBLUE);
			std::cout << "*";
			set_text_color(conWHITE);

			// Move cursor past the map
			gotoxy((short)0, (short)(m_oy + m.h + 2));
			std::cout << "Goal Reached!!\n";
			std::cout << "The path the simulation took that first arrived to the goal is marked in blue.\n";
			std::cout << "Starting Position: (" << m.sx << "," << m.sy << ")\n";
			std::cout << "Last Position: (" << x << "," << y << ")\n";
			std::cout << "Path length (moves taken): " << steps << "\n";
			std::cout << "States visited during search: " << states_visited << "\n";
			return 1;
		}
		else {
			marks[y][x] = true;
			set_text_color(conLIGHTBLUE);
			gotoxy((short)(m_ox + lx), (short)(m_oy + ly));
			if (lx == m.sx && ly == m.sy) std::cout << 'S';
			else std::cout << '*';
			gotoxy((short)(m_ox + x), (short)(m_oy + y));
			set_text_color(conBLUE);
			std::cout << "V";
			set_text_color(conWHITE);
			sleep_(100);
			for (int i = 0; i < n_dir; i++) {
				int nx = x + dx[i];
				int ny = y + dy[i];
				if (marks && ny >= 0 && ny < m.h && nx >= 0 && nx < m.w && !marks[y + dy[i]][x + dx[i]] && m.map[(y + dy[i]) * m.w + x + dx[i]] != '#')
					if (dfs_opt(m, nx, ny, steps + 1, x, y))
						return 1;
			}
			gotoxy((short)(m_ox + x), (short)(m_oy + y));
			set_text_color(conDARKGRAY);
			if (x == m.sx && y == m.sy) std::cout << "S";
			else std::cout << "*";
			set_text_color(conWHITE);
			sleep_(100);
			return 0;
		}
	}
}

int hill_climbing(maze &m, int x, int y, int steps, int lx, int ly) {

	static char** marks = nullptr;

	if (!marks) {
		marks = new char* [m.h];
		for (int i = 0; i < m.h; i++)
			marks[i] = new char[m.w]();
		marks[m.sy][m.sx] = true;
		int result = hill_climbing(m, x, y, steps, x, y);
		delete[] marks;
		marks = nullptr;
		if (!result) {
			gotoxy((short)0, (short)(m_oy + m.h + 2));
			std::cout << "The goal position could not be reached.\n";
			std::cout << "States visited during search: " << states_visited << "\n";
		}
		return result;
	}
	else {
		
		states_visited++;
		if (x == m.gx && y == m.gy) {
			gotoxy((short)m_ox + lx, (short)m_oy + ly);
			set_text_color(conLIGHTBLUE);
			std::cout << "*";
			set_text_color(conWHITE);

			// Move cursor past the map
			gotoxy((short)0, (short)(m_oy + m.h + 2));
			std::cout << "Goal Reached!!\n";
			std::cout << "The path the simulation took that first arrived to the goal is marked in blue.\n";
			std::cout << "Starting Position: (" << m.sx << "," << m.sy << ")\n";
			std::cout << "Last Position: (" << x << "," << y << ")\n";
			std::cout << "Path length (moves taken): " << steps << "\n";
			std::cout << "States visited during search: " << states_visited << "\n";
			return 1;
		}
		else {
			marks[y][x] = true;
			set_text_color(conLIGHTBLUE);
			gotoxy((short)(m_ox + lx), (short)(m_oy + ly));
			if (lx == m.sx && ly == m.sy) std::cout << 'S';
			else std::cout << '*';
			gotoxy((short)(m_ox + x), (short)(m_oy + y));
			set_text_color(conBLUE);
			std::cout << "V";
			set_text_color(conWHITE);
			sleep_(100);
			int best_h = NOT_VALID_INT2;
			int move_i = -1;
			for (int i = 0; i < n_dir; i++) {
				int nx = x + dx[i];
				int ny = y + dy[i];
				if (marks && ny >= 0 && ny < m.h && nx >= 0 && nx < m.w && !marks[ny][nx] && m.map[ny * m.w + nx] != '#')
					if (manhattan(nx, ny, m.gx, m.gy) < best_h) {
						best_h = manhattan(nx, ny, m.gx, m.gy);
						move_i = i;
					}
			}
			if (move_i == -1 || !hill_climbing(m, x + dx[move_i], y + dy[move_i], steps + 1, x, y)) {
				gotoxy((short)(m_ox + x), (short)(m_oy + y));
				set_text_color(conDARKGRAY);
				if (x == m.sx && y == m.sy) std::cout << "S";
				else std::cout << "*";
				set_text_color(conWHITE);
				return 0;
			}
		}
	}
}

void best_first_search(maze& m) {

	struct compare {
		bool operator()(const std::pair<search_state, int> &a, const std::pair<search_state, int> &b) {
			return a.second < b.second;
		}
	};
	std::priority_queue<std::pair<search_state, int>, std::vector<std::pair<search_state, int>>, compare>pq;

	bool goal_found = false;
	char** marks = new char* [m.h];
	for (int i = 0; i < m.h; i++)
		marks[i] = new char[m.w]();
	marks[m.sy][m.sx] = true;
	
	pq.push(std::make_pair(search_state(m.sx, m.sx, 0),0));

	int lx = m.sx;
	int ly = m.sy;

	while (!pq.empty()) {
		states_visited++;
		std::pair<search_state,int> current = pq.top();
		pq.pop();
		if (current.first.x == m.gx && current.first.y == m.gy) {
			gotoxy((short)m_ox + lx, (short)m_oy + ly);
			set_text_color(conDARKGRAY);
			std::cout << "*";
			gotoxy(m_ox, m_oy);
			set_bfs_path(m, current.first.x, current.first.y, marks, m_ox, m_oy);
			set_text_color(conWHITE);
			// Move cursor past the map
			gotoxy(0, m_oy + m.h + 2);
			sleep_(1000);
			std::cout << "Goal Reached!!\n";
			std::cout << "The path the simulation took that first arrived to the goal is marked in blue.\n";
			std::cout << "Starting Position: (" << m.sx << "," << m.sy << ")\n";
			std::cout << "Last Position: (" << current.first.x << "," << current.first.y << ")\n";
			std::cout << "Path length (moves taken): " << current.first.steps << "\n";
			std::cout << "States visited during search: " << states_visited << "\n";
			goal_found = true; break;
		}
		else for (int i = 0; i < n_dir; i++) {
			gotoxy((short)m_ox + lx, (short)m_oy + ly);
			set_text_color(conDARKGRAY);
			if (m.map[ly * m.w + lx] == 'S') std::cout << "S";
			else std::cout << "*";
			gotoxy((short)(m_ox + current.first.x), (short)(m_oy + current.first.y));
			set_text_color(conLIGHTBLUE);
			std::cout << "V";
			lx = current.first.x; ly = current.first.y;
			Sleep(25);
			std::pair<search_state, int> child = current;
			child.first.x += dx[i];
			child.first.y += dy[i];
			if (child.first.x >= 0 && child.first.x < m.w && child.first.y >= 0 && child.first.y < m.h && m.map[child.first.y * m.w + child.first.x] != '#' && !marks[child.first.y][child.first.x]) {
				marks[child.first.y][child.first.x] = i + '0';
				child.first.steps++;
				child.second = manhattan(child.first.x, child.first.y, m.gx, m.gy);
				pq.push(child);
			}
		}
	}
	if (!goal_found) {
		gotoxy((short)0, (short)(m_oy + m.h + 2));
		std::cout << "The goal could not be reached.\n";
		std::cout << "States visited during search : " << states_visited << "\n";
	}
	delete[] marks;
	marks = nullptr;
	set_text_color(conWHITE);
}

void a_star(maze& m) {

	struct compare {
		bool operator()(const std::pair<search_state, int>& a, const std::pair<search_state, int>& b) {
			return a.second < b.second;
		}
	};
	std::priority_queue<std::pair<search_state, int>, std::vector<std::pair<search_state, int>>, compare>pq;

	bool goal_found = false;
	char** marks = new char* [m.h];
	for (int i = 0; i < m.h; i++)
		marks[i] = new char[m.w]();
	marks[m.sy][m.sx] = true;

	pq.push(std::make_pair(search_state(m.sx, m.sx, 0), 0));

	int lx = m.sx;
	int ly = m.sy;

	while (!pq.empty()) {
		states_visited++;
		std::pair<search_state, int> current = pq.top();
		pq.pop();
		if (current.first.x == m.gx && current.first.y == m.gy) {
			gotoxy((short)m_ox + lx, (short)m_oy + ly);
			set_text_color(conDARKGRAY);
			std::cout << "*";
			gotoxy(m_ox, m_oy);
			set_bfs_path(m, current.first.x, current.first.y, marks, m_ox, m_oy);
			set_text_color(conWHITE);
			// Move cursor past the map
			gotoxy(0, m_oy + m.h + 2);
			sleep_(1000);
			std::cout << "Goal Reached!!\n";
			std::cout << "The path the simulation took that first arrived to the goal is marked in blue.\n";
			std::cout << "Starting Position: (" << m.sx << "," << m.sy << ")\n";
			std::cout << "Last Position: (" << current.first.x << "," << current.first.y << ")\n";
			std::cout << "Path length (moves taken): " << current.first.steps << "\n";
			std::cout << "States visited during search: " << states_visited << "\n";
			goal_found = true; break;
		}
		else for (int i = 0; i < n_dir; i++) {
			gotoxy((short)m_ox + lx, (short)m_oy + ly);
			set_text_color(conDARKGRAY);
			if (m.map[ly * m.w + lx] == 'S') std::cout << "S";
			else std::cout << "*";
			gotoxy((short)(m_ox + current.first.x), (short)(m_oy + current.first.y));
			set_text_color(conLIGHTBLUE);
			std::cout << "V";
			lx = current.first.x; ly = current.first.y;
			Sleep(25);
			std::pair<search_state, int> child = current;
			child.first.x += dx[i];
			child.first.y += dy[i];
			if (child.first.x >= 0 && child.first.x < m.w && child.first.y >= 0 && child.first.y < m.h && m.map[child.first.y * m.w + child.first.x] != '#' && !marks[child.first.y][child.first.x]) {
				marks[child.first.y][child.first.x] = i + '0';
				child.first.steps++;
				child.second = cost[i] + manhattan(child.first.x, child.first.y, m.gx, m.gy);
				pq.push(child);
			}
		}
	}
	if (!goal_found) {
		gotoxy((short)0, (short)(m_oy + m.h + 2));
		std::cout << "The goal could not be reached.\n";
		std::cout << "States visited during search : " << states_visited << "\n";
	}
	delete[] marks;
	marks = nullptr;
	set_text_color(conWHITE);
}