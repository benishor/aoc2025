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

struct machine {
	uint32_t state;
	uint32_t desired_state;
	std::vector<uint32_t> btn_masks;
	std::vector<int> joltages;

	void reset() {
		state = 0;
	}

	int find_minimum_combinations() {
		int max_iter = 1 << btn_masks.size();
		int result = 100000;
		for (int i = 1; i < max_iter; i++) {
			uint32_t current_state = 0;
			int current_presses = 0;
			for (int j = 0; j < btn_masks.size(); j++) {
				if ((i & (1 << j)) != 0) {
					current_state ^= btn_masks[j];
					current_presses++;
					if (current_presses > result) break;
					if (current_state == desired_state && current_presses < result) {
						result = current_presses;
					}
				}
			}
		}
		return result;
	}

	int find_minimum_combinations() {
		std::vector<int> keypresses;
		for (int i = 0; i < joltages.size(); i++) {
			std::vector<int> buttons_matching_counter;
			for (int j = 0; j < btn_masks.size(); j++) {
				if ((btn_masks[j] & (1 << i)) != 0) {
					for (int t = 0; t < joltages[i]; t++) {
						keypresses.push_back(j);
					}
				}
			}
		}

		do {
		} while (std::next_permutation(s.begin(), s.end()));

	}
};

std::ostream& operator << (std::ostream& out, const machine& m) {
	out << "{state: " << m.state << ", desired_state: " << m.desired_state;
	out << ", btn_masks: [";
	for (auto& b : m.btn_masks) {
		out << b << ",";
	}
	out << "]";
	out << ", joltages: [";
	for (auto& b : m.joltages) {
		out << b << ",";
	}
	out << "]}";
	return out;
}

int main() {
	std::fstream f{"input.txt"};

	uint64_t part1 = 0;
	uint64_t part2 = 0;

	auto start = std::chrono::high_resolution_clock::now();

	std::vector<machine> machines;

	for (std::string l; std::getline(f, l);) {
		std::stringstream ss(l);

		machine m;
		std::string token;
		while (ss >> token) {
			if (token[0] == '[') {
				m.desired_state = 0;
				for (int i = token.size() - 2; i > 0; i--) {
					m.desired_state <<= 1;
					m.desired_state |= token[i] == '#' ? 1 : 0;
				}
			} else if (token[0] == '(') {
				std::stringstream bs(token.substr(1));
				uint32_t btn_mask = 0;
				int btn_bit;
				char comma;
				while (bs >> btn_bit) {
					btn_mask |= 1 << btn_bit;
					bs >> comma;
				}
				m.btn_masks.push_back(btn_mask);
			} else if (token[0] == '{') {
				std::stringstream js(token.substr(1));
				char comma;
				int joltage;
				while (js >> joltage) {
					m.joltages.push_back(joltage);
					js >> comma;
				}
			}
		}
		machines.push_back(m);
	}

	for (machine& m : machines) {
		// std::cout << m << std::endl;
		m.reset();
		part1 += m.find_minimum_combinations();
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = end - start;

	std::cout << part1 << std::endl;
	std::cout << part2 << std::endl;

	auto us = std::chrono::duration_cast<std::chrono::microseconds>(duration);
    std::cout << "Time taken: " << us.count()/1000.0f << " ms" << std::endl;	

}