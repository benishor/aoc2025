#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <fmt/ranges.h>
#include <ranges>
#include <set>



bool is_accessible(const std::vector<std::string>& grid, int x, int y) {
	int w = grid[0].size();
	int h = grid.size();

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
			if (grid[dy+y][dx+x] == '@')
				rolls_nearby++;
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


	int blocks_removed;
	do {
		blocks_removed = 0;
		auto new_grid = grid;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (grid[y][x] == '@' && is_accessible(grid, x, y)) {
					blocks_removed++;
					new_grid[y][x] = '.';
				}
			}
		}

		if (part1 == 0) {
			part1 = blocks_removed;
		}

		part2 += blocks_removed;
		grid = std::move(new_grid);
	} while (blocks_removed > 0);

	std::cout << part1 << std::endl;
	std::cout << part2 << std::endl;
}