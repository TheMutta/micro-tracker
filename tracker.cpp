#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <ncurses.h>

using json = nlohmann::json;
/*
void todo() {
	for (auto entry : data) {
		std::string output;

		if (entry["completed"]) {
			output += "✔";
		} else {
			output += "✘";
		}
	
		std::cout << output << std::endl;
	}
}*/


void print_elements(std::map<std::string, std::size_t> &food, std::size_t &total) {
	for (auto [name, calories] : food) {
		printw("%s\t%lukcal\n", name.c_str(), calories);
		total += calories;
	}
}

void print_total(std::size_t total) {
	const std::size_t aim= 1800;
	printw("Total:\t%lukcal\n", total);
	printw("Aim:\t%lukcal\n", aim);
}

void load_data(std::map<std::string, std::size_t> &food) {
	std::ifstream f("food.json");
	json data = json::parse(f);

	for(auto element : data) {
		food[element["name"]] = element["calories"];
	}
}

void save_data(std::map<std::string, std::size_t> &food) {
	json data;

	std::size_t index = 0;
	for(auto [name, calories] : food) {
		data[index]["name"] = name;
		data[index]["calories"] = calories;
		++index;
	}
	
	std::ofstream f("food.json");
	f << data;
}

int main() {
	initscr();
	noecho();
	
	std::map<std::string, std::size_t> food;
	load_data(food);

	bool done = false;

	while(!done) {
		std::size_t totalCalories = 0;
		print_elements(food, totalCalories);
		print_total(totalCalories);

		int ch = getch();
		switch(ch) {
			case 'a': {
				char name[11] = { 0 };
				std::size_t calories;

				echo();
				printw("\nInsert food name: ");
				scanw("%10s", name);

				printw("Insert food calories: ");
				scanw("%lu", &calories);

				noecho();
				
				food[std::string(name)] = calories;
				}
				break;
			case 'c':{
				char confirm;
				echo();
				printw("\nDo you want to clear the list (Y/N): ");
				scanw("%c", &confirm);
				noecho();

				if (confirm == 'y' || confirm == 'Y') {
					food.clear();
				}

				}
				break;
			case 'w':
				save_data(food);
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
