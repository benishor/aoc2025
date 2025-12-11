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


std::map<std::string, uint64_t> cache;

uint64_t count_graph_paths(std::map<std::string, std::vector<std::string>>& graph, std::string from, std::string to) {
	if(cache.count(from) > 0) {
		return cache[from];
	}

	if (graph.count(from) == 0)
		return 0;

	uint64_t result = 0;
	for (auto& dest : graph[from]) {
		if (dest == to) {
			return 1;
		}
		result += count_graph_paths(graph, dest, to);
	}
	cache[from] = result;

	return result;
}

struct cache_item {
	uint64_t count;
	uint8_t flags;
};


uint64_t count_graph_paths2(std::map<std::string, std::vector<std::string>>& graph, std::string from, std::string to, uint8_t flags, std::vector<std::string> visited_nodes) {
	if (graph.count(from) == 0)
		return 0;

	if (cache.count(from) > 0 && cache[from] == 0) {
		return 0;
	}

	if (std::find(visited_nodes.begin(), visited_nodes.end(), from) != visited_nodes.end()) {
		std::cout << "loop detected!" << std::endl;
		exit(0);
	}
	visited_nodes.push_back(from);

	uint8_t new_flags = flags;	
	if (from == "dac") {
		new_flags |= 1;
		std::cout << "Found dac" << std::endl;
	}
	if (from == "fft") {
		new_flags |= 2;
		std::cout << "Found fft" << std::endl;
	}

	// if (new_flags > 0)
	// 	std::cout << "Found flags " << (int)new_flags << std::endl;

	uint64_t result = 0;
	for (auto& dest : graph[from]) {
		if (dest == to) {
			if (new_flags > 0)
				// std::cout << "Found path with flags" << int(new_flags) << std::endl;

			return new_flags == 3 ? 1 : 0;
		}
		auto tmp = count_graph_paths2(graph, dest, to, new_flags, visited_nodes);
		result += tmp;
	}


	// if (result > 0) {
	// 	std::cout << "found something greater than 0" << std::endl;
	// }

	return result;
}

int main() {
	std::fstream f{"input.txt"};

	uint64_t part1 = 0;
	uint64_t part2 = 0;

	auto start = std::chrono::high_resolution_clock::now();

	std::map<std::string, std::vector<std::string>> graph;
	for (std::string l; std::getline(f, l);) {
		auto pos = l.find(":");
		auto key = l.substr(0, pos);

		std::vector<std::string> outputs;
		std::stringstream ss(l.substr(pos+2));
		std::string token;
		while (ss >> token) {
			outputs.push_back(token);
		}
		graph[key] = outputs;
	}

	part1 = count_graph_paths(graph, "you", "out");
	part2 = 1;
	cache.clear();
	part2 *= count_graph_paths(graph, "svr", "fft");
	cache.clear();
	part2 *= count_graph_paths(graph, "fft", "dac");
	cache.clear();
	part2 *= count_graph_paths(graph, "dac", "out");
	// part2 = count_graph_paths2(graph, "svr", "out", 0, {});
	// part2 = count_graph_paths(graph, "dac", "out");
	// part2 = count_graph_paths(graph, "fft", "out");
	// part2 = count_graph_paths2(graph, "svr", "out", 0, {});


	auto end = std::chrono::high_resolution_clock::now();
	auto duration = end - start;

	std::cout << part1 << std::endl;
	std::cout << part2 << std::endl;

	auto us = std::chrono::duration_cast<std::chrono::microseconds>(duration);
    std::cout << "Time taken: " << us.count()/1000.0f << " ms" << std::endl;	

}