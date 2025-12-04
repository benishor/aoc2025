#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <fmt/ranges.h>
#include <ranges>
#include <set>
#include <chrono>



inline bool is_accessible(const std::vector<std::string>& grid, int w, int h, int x, int y) {
	int rolls_nearby = 0;
	for (int dy = -1; dy <= 1; dy++) {
		for (int dx = -1; dx <= 1; dx++) {
			if (dy == 0 && dx == 0)
				continue;
			if (((dx + x) <  0) ||
				((dx + x) >=  w) ||
				((dy + y) <  0) ||
				((dy + y) >=  h) )
				continue;
			rolls_nearby += (grid[dy+y][dx+x] == '@');
		}
	}

	return rolls_nearby < 4;
}

int main() {
	std::fstream f{"input.txt"};

	std::vector<std::string> grid;
	for (std::string l; std::getline(f, l);) {
		grid.push_back(l);
	}

	auto width = grid[0].size();
	auto height = grid.size();

	uint64_t part1 = 0;
	uint64_t part2 = 0;



	auto start = std::chrono::high_resolution_clock::now();
	int blocks_removed;
	auto new_grid = grid;
	bool keep_running;
	for (;;) {
		blocks_removed = 0;
		keep_running = false;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (grid[y][x] == '@' && is_accessible(grid, width, height, x, y)) {
					blocks_removed++;
					part2++;
					keep_running = true;
					new_grid[y][x] = '.';
				}
			}
		}

		if (part1 == 0) {
			part1 = blocks_removed;
		}
		
		if (keep_running) {
			grid = new_grid;
		} else {
			break;
		}
	};

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = end - start;

	std::cout << part1 << std::endl;
	std::cout << part2 << std::endl;

	auto us = std::chrono::duration_cast<std::chrono::microseconds>(duration);
    std::cout << "Time taken: " << us.count()/1000.0f << " ms" << std::endl;	

}