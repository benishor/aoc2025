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

struct junction {
	int64_t x, y, z;
};

std::ostream& operator << (std::ostream& out, const junction& a) {
	out << "{x: " << a.x << ", y: " << a.y << ", z: " << a.z << "}";
	return out;
}


uint64_t distance(const junction& a, const junction& b) {
	int64_t dx = b.x - a.x;
	int64_t dy = b.y - a.y;
	int64_t dz = b.z - a.z;
	// std::cout << "distance between " << a << " and " << b << std::endl;
	// std::cout << "dx: " << dx << ", dy: " << dy << ", dz: " << dz << std::endl;
	if (dx == 0 && dy == 0 && dz == 0) {
		return 0;
	}
	uint64_t result = dx*dx + dy*dy + dz*dz;
	// std::cout << "result is " << result << std::endl;
	return result;
}

// returns the number of junctions in the affected circuit size
int connect(uint64_t from, uint64_t to, std::vector<std::vector<int>>& circuits) {
	// std::cout << "connecting " << from << " and " << to << std::endl;
	int from_circuit = -1;
	int to_circuit = -1;
	for (int i = 0; i < circuits.size(); i++) {
		if (std::find(circuits[i].begin(), circuits[i].end(), from) != circuits[i].end()) {
			from_circuit = i;
		}
		if (std::find(circuits[i].begin(), circuits[i].end(), to) != circuits[i].end()) {
			to_circuit = i;
		}
		if (from_circuit > -1 && to_circuit > -1)
			break;
	}

	// do nothing if already connected
	if (from_circuit == to_circuit && from_circuit != -1) {
		return circuits[from_circuit].size();
	}

	// new circuit
	if (from_circuit == -1 && to_circuit == -1) {
		auto circuit = std::vector<int>();
		circuit.push_back(from);
		circuit.push_back(to);
		circuits.push_back(circuit);
		return 2;
	}

	// if any of them is not part of a circuit, add them to the existing one
	if (from_circuit == -1) {
		circuits[to_circuit].push_back(from);
		return circuits[to_circuit].size();
	}

	if (to_circuit == -1) {
		circuits[from_circuit].push_back(to);
		return circuits[from_circuit].size();
	}

	// merge the two circuits. add all in one and delete the other
	for (auto& x : circuits[to_circuit]) {
		circuits[from_circuit].push_back(x);
	}
	circuits.erase(circuits.begin() + to_circuit);
	return circuits[from_circuit].size();
}

int main() {
	std::fstream f{"input.txt"};

	uint64_t part1 = 0;
	uint64_t part2 = 0;

	auto start = std::chrono::high_resolution_clock::now();
	std::vector<junction> junctions;
	std::map<uint64_t, uint64_t> distances; // key is : from*1024+to
	std::map<uint64_t, uint64_t> reverse_distances;

	for (std::string l; std::getline(f, l);) {
		std::stringstream ss(l);
		int a, b, c;
		char comma;
		ss >> a >> comma >> b >> comma >> c;
		junctions.push_back({a, b, c});
	}


	for (uint64_t i = 0; i < (junctions.size() - 1); i++) {
		for (uint64_t j = i+1; j < junctions.size(); j++) {
			auto d = distance(junctions[i], junctions[j]);
			uint64_t key1 = i * 1024 + j;
			uint64_t key2 = j * 1024 + i;
			distances[key1] = d;
			distances[key2] = d;
			// std::cout << "key1 is: " << key1 << ", key2: " << key2  << ", d: " << d << std::endl;
			reverse_distances[d] = key1;
		}
	}

	std::vector<std::vector<int>> circuits;

	int i = 0;
	for (auto& rd : reverse_distances) {
		uint64_t from = rd.second >> (uint64_t)10;
		uint64_t to = rd.second & (uint64_t)1023;
		if (connect(from, to, circuits) == junctions.size() ) {
			// stop, we have one circuit.
			part2 = junctions[from].x * junctions[to].x;
			break;
		}
		i++;

		// part1
		if (i == 1000) {
			std::set<int> circuit_sizes;
			for (auto& c : circuits) {
				circuit_sizes.insert(c.size());
			}
			auto itt = circuit_sizes.rbegin();
			auto a = *itt++;
			auto b = *itt++;
			auto c = *itt++;
			part1 = a*b*c;
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = end - start;

	std::cout << part1 << std::endl;
	std::cout << part2 << std::endl;

	auto us = std::chrono::duration_cast<std::chrono::microseconds>(duration);
    std::cout << "Time taken: " << us.count()/1000.0f << " ms" << std::endl;	

}