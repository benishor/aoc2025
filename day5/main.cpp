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


bool exists_in_ranges(uint64_t value, const std::vector<std::pair<uint64_t, uint64_t>>& ranges) {
	for (auto& r : ranges) {
		if (value >= r.first && value <= r.second) {
			return true;
		}
	}
	return false;
}

void compact_ranges(std::vector<std::pair<uint64_t, uint64_t>>& ranges) {
	bool overlapped = false;
	auto new_ranges = std::vector<std::pair<uint64_t, uint64_t>>();

	do {
		// new_ranges.clear();
		overlapped = false;

		for (auto& rr : ranges) {
			auto from = rr.first;
			auto to = rr.second;


			bool merged = false;

			auto it = new_ranges.begin();
			while (it != new_ranges.end()) {
				if (from <= (*it).first && to >= (*it).second) {
					// the new one encompasses this one
					(*it).first = from;
					(*it).second = to;
					merged = true;
					overlapped = true;
					break;
				} else if (from >= (*it).first && to <= (*it).second) {
					// the new one is fully contained 
					merged = true;
					overlapped = true;
					break;
				} else if (from < (*it).first && to >= (*it).first && to <= (*it).second) {
					// new one overlaps at left
					(*it).first = from;
					merged = true;
					overlapped = true;
					break;
				} else if (from >= (*it).first && from <= (*it).second && to > (*it).second) {
					// new one overlaps at right
					(*it).second = to;
					merged = true;
					overlapped = true;
					break;
				}
				it++;
			}

			if (!merged) {
				new_ranges.push_back({from, to});
			}
		}
		ranges = std::move(new_ranges);

	} while (overlapped);
}

int main() {
	std::fstream f{"input.txt"};

	uint64_t part1 = 0;
	uint64_t part2 = 0;

	auto start = std::chrono::high_resolution_clock::now();

	std::vector<std::pair<uint64_t, uint64_t>> ranges;
	for (std::string l; std::getline(f, l);) {
		if (l == "") {
			compact_ranges(ranges);
			continue;
		}

		size_t pos;
		if ((pos = l.find('-')) != std::string::npos) {
			ranges.push_back({
				std::stoul(l.substr(0, pos)), 
				std::stoul(l.substr(pos + 1))
			});
		} else {
			auto value = std::stoul(l);
			part1 += exists_in_ranges(value, ranges);
		}
	}

	for (auto& r : ranges) {
		part2 += r.second - r.first + 1;
	}


	// -----------------------------------------------------
	// -----------------------------------------------------


	auto end = std::chrono::high_resolution_clock::now();
	auto duration = end - start;

	std::cout << part1 << std::endl;
	std::cout << part2 << std::endl;

	auto us = std::chrono::duration_cast<std::chrono::microseconds>(duration);
    std::cout << "Time taken: " << us.count()/1000.0f << " ms" << std::endl;	

}