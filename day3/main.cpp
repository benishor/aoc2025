#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <fmt/ranges.h>
#include <ranges>
#include <set>


uint64_t get_max_joltage(const std::string& l, int digits) {

	uint64_t joltage = 0;
	int last_digit_pos = 0;

	for (auto d = 0; d < digits; d++) {
		int cur_digit_max = 0;
		int cur_digit_max_pos = 0;

		for (auto i = last_digit_pos; i < (l.size() - digits + d + 1); i++) {
			if ((l[i] - '0') > cur_digit_max) {
				cur_digit_max = (l[i] - '0');
				cur_digit_max_pos = i;
			}
		}

		joltage *= 10;
		joltage += cur_digit_max;
		last_digit_pos = cur_digit_max_pos + 1;
	}
	return joltage;

}
int main() {
	std::fstream f{"input.txt"};

	uint64_t part1 = 0;
	uint64_t part2 = 0;

	for (std::string l; std::getline(f, l);) {
		part1 += get_max_joltage(l, 2);
		part2 += get_max_joltage(l, 12);
	}

	std::cout << part1 << std::endl;
	std::cout << part2 << std::endl;
}