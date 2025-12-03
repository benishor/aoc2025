#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <unordered_map>

int main() {
	std::fstream f{"input.txt"};

	int dial = 50;
	int part1 = 0;
	int part2 = 0;

	for (std::string l; std::getline(f, l); ) {
		auto old_dial = dial;
		auto times = std::stoi(l.substr(1));

		if (l[0] == 'R') {
			dial += (times % 100);
			if (dial > 99) {
				dial -= 100;
				part2++;
			}
			part1 += (dial == 0);
			part2 += times / 100;
			// part2 += (((times % 100) + old_dial) > 99);
		} else {
			dial -= times % 100;
			if (dial < 0) {
				dial += 100;
				part2++;
			}
			part1 += (dial == 0);
			part2 += times / 100;
		}
	}

	std::cout << part1 << std::endl;
	std::cout << part2 << std::endl;
}