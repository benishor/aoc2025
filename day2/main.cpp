#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <fmt/ranges.h>
#include <ranges>


bool is_invalid_id(uint64_t id) {
	auto nr = std::to_string(id);
	auto s = nr.size();
	if ((s & 1) == 1) return false;
	for (auto i = 0; i < s/2; i++) {
		if (nr[i] != nr[s/2+i])
			return false;
	}
	return true;
}

bool is_invalid_id2(uint64_t id) {
	auto nr = std::to_string(id);
	// fmt::print("nr: {}\n", nr);
	auto s = nr.size();
	// fmt::print("s: {}\n", s);
	for (int part_size = s/2; part_size > 0; part_size--) {
		// fmt::print("part_size: {}\n", part_size);

		if ((s % part_size) != 0)
			continue;
		int pieces = s / part_size;
		// fmt::print("pieces: {}\n", pieces);

		bool invalid = true;
		for (int i = 0; i < part_size; i++) {
			for (int p = 1; p < pieces; p++) {
				if (nr[i] != nr[i+p*part_size]) {
					invalid = false;
					goto next_part;
				}
			}
		}
		next_part:
		if (invalid)
			return true;

	}

	return false;
}

void add_invalid_ids(uint64_t from, uint64_t to, uint64_t& sum) {
	for (uint64_t i = from; i <= to; i++) {
		if (is_invalid_id(i)) {
			sum += i;
		}
	}
}

void add_invalid_ids2(uint64_t from, uint64_t to, uint64_t& sum) {
	for (uint64_t i = from; i <= to; i++) {
		if (is_invalid_id2(i)) {
			sum += i;
		}
	}
}

int main() {
	std::fstream f{"input.txt"};


	uint64_t part1 = 0;
	uint64_t part2 = 0;

	std::string l;
	std::getline(f, l);
	const std::string& delim = ",";
	std::vector<std::string> line;

	for (const auto& word : std::views::split(l, delim)) {
		auto range = std::string(word.begin(), word.end());
		auto sep = range.find('-');
		auto from = std::stoul(range.substr(0, sep));
		auto to = std::stoul(range.substr(sep+1));
		add_invalid_ids(from, to, part1);
		add_invalid_ids2(from, to, part2);
    }

	std::cout << part1 << std::endl;
	std::cout << part2 << std::endl;
}