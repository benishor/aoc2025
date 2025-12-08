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
#include <sstream>
#include <regex>


std::map<int, uint64_t> cache;

uint64_t compute_timelines(int source_x, int source_y, int width, const std::vector<std::vector<int>>& splitters) {
	if (source_y >= splitters.size())
		return 1;
	if (source_x < 0)
		return 1;
	if (source_x >= width)
		return 1;

	for (auto& x : splitters[source_y]) {
		if (x == source_x) {
			//  std::cout << "Splitting at x: " << x << ", y: " << source_y << std::endl;
			int key = 1000*source_y + source_x;
			if (!cache.contains(key)) {
				auto value = compute_timelines(source_x - 1, source_y + 1, width, splitters) +
						compute_timelines(source_x + 1, source_y + 1, width, splitters);
				cache[key] = value;
			}
			return cache[key];
		}
	}
	return compute_timelines(source_x, source_y + 1, width, splitters);
}

int main() {
	std::fstream f{"input.txt"};

	uint64_t part1 = 0;
	uint64_t part2 = 0;


	std::vector<std::string> lines;
	std::vector<std::vector<int>> splitters;

	int line_nr = 0;
	int source_x = 0;
	int width = 0;
	for (std::string l; std::getline(f, l);) {
		if (line_nr == 0) {
			source_x = l.find("S");
			width = l.size();
		} else {
			std::vector<int> row;
			for (int i = 0; i < l.size(); i++) {
				if (l[i] == '^') {
					row.push_back(i);
				}
			}
			splitters.push_back(row);
		}
		line_nr++;
	}

	auto start = std::chrono::high_resolution_clock::now();

	// std::cout << "start_x = " << source_x  << ", width: " << width << std::endl;
	// for (auto& kv : splitters) {
	// 	std::cout << "\t" << kv.first << ": ";
	// 	for (auto& x : kv.second)
	// 		std::cout << x << ", ";
	// 	std::cout << std::endl;
	// }
	std::vector<bool> beams(width, false);
	beams[source_x] = true;
	for (auto& row : splitters) {
		std::vector<bool> new_beams(width, false);
		new_beams = beams;
		for (auto& x : row) {
			if (beams[x]) {
				new_beams[x] = false;
				auto left = x - 1;
				auto right = x + 1;
				if (left >= 0)
					new_beams[left] = true;
				if (right < width)
					new_beams[right] = true;
				part1++;
			}
		}
		beams = std::move(new_beams);
		// std::copy(beams,new_beams);
	}

	auto source_y = 0;
	part2 = compute_timelines(source_x, source_y, width, splitters);



	auto end = std::chrono::high_resolution_clock::now();
	auto duration = end - start;

	std::cout << part1 << std::endl;
	std::cout << part2 << std::endl;

	auto us = std::chrono::duration_cast<std::chrono::microseconds>(duration);
    std::cout << "Time taken: " << us.count()/1000.0f << " ms" << std::endl;	

}