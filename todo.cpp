#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <ncurses.h>

using json = nlohmann::json;

namespace todo {
struct todo_entry {
	bool completed;
	std::string desc;
};


void print_elements(std::map<std::string, todo_entry> &todo) {
	for (auto [name, entry] : todo) {
		std::string output = "";

		if (entry.completed) {
			attron(A_DIM);
		}

		output += name;

		printw("%s\n", output.c_str());

		attroff(A_DIM);
;
	}
	
}

void load_data(std::map<std::string, todo_entry> &todo) {
	std::ifstream f("todo.json");
	json data = json::parse(f);

	for (auto element : data) {
		todo_entry entry;
		entry.completed = element["completed"];
		entry.desc = element["desc"];
		todo[element["name"]] = entry;
	}
}

void save_data(std::map<std::string, todo_entry> &todo) {
	json data;

	std::size_t index = 0;
	for(auto [name, entry] : todo) {
		data[index]["completed"] = entry.completed;
		data[index]["name"] = name;
		data[index]["desc"] = entry.desc;
		++index;
	}
	
	std::ofstream f("todo.json");
	f << data;
}
}

int main() {
	initscr();
	noecho();
	
	std::map<std::string, todo::todo_entry> todo;
	todo::load_data(todo);

	bool done = false;

	while(!done) {
		todo::print_elements(todo);

		int ch = getch();
		switch(ch) {
			case 'a': {
				char buffer[4096] = { 0 };
				std::string name;
				todo::todo_entry entry;
				entry.completed = false;

				echo();
				printw("\nInsert entry name: ");
				scanw("%4095s", buffer);
				name = buffer;

				printw("Insert entry description: ");
				scanw("%4095s", buffer);
				entry.desc= buffer;
				noecho();
				
				todo[name] = entry;
				}
				break;
			case 'd': {
				char buffer[4096] = { 0 };

				echo();
				printw("\nInsert entry name: ");
				scanw("%4095s", buffer);
				noecho();

				todo.erase(buffer);
				}
				break;
			case 's': {
				char buffer[4096] = { 0 };

				echo();
				printw("\nInsert entry name: ");
				scanw("%4095s", buffer);

				noecho();
				
				todo[buffer].completed = true;
				}
				break;
			case 'c':{
				char confirm;
				echo();
				printw("\nDo you want to clear the list (Y/N): ");
				scanw("%c", &confirm);
				noecho();

				if (confirm == 'y' || confirm == 'Y') {
					todo.clear();
				}

				}
				break;
			case 'w':
				todo::save_data(todo);
				break;
			case 'q':
				done = true;
				break;

		}

		clear();
		move(0, 0);

	}

	endwin();

	return 0;
}
