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


void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}
void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}
void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

int main() {
	std::fstream f{"input.txt"};

	uint64_t part1 = 0;
	uint64_t part2 = 0;


	std::vector<size_t> col_max_size(1000, 0);

	std::vector<std::vector<std::string>> data;

	std::vector<std::string> lines;
	std::vector<std::string> operators;
	std::string token;
	for (std::string l; std::getline(f, l);) {
		if (std::isdigit(l[0]) || std::isspace(l[0])) {
			lines.push_back(l);
			int column = 0;
		    for (std::stringstream ss(l); ss >> token;) {
		    	col_max_size[column] = std::max(col_max_size[column], token.size());
		    	column++;
		    }
		} else {
		    for (std::stringstream ss(l); ss >> token;) {
		    	operators.push_back(token);
		    }
		}
	}

	for (auto& l : lines) {
		std::vector<std::string> row;
		int start = 0;
		for (int i = 0; i < operators.size(); i++) {
			auto len = col_max_size[i];
			row.push_back(l.substr(start, len));
			start += len + 1;
		}
		data.push_back(row);
	}

	// for (auto& row : data) {
	// 	for (auto& col : row) {
	// 		std::cout << "[" << col << "] ";
	// 	}
	// 	std::cout << std::endl;
	// }

	auto start = std::chrono::high_resolution_clock::now();


	for (int i = 0; i < operators.size(); i++) {
    	uint64_t result_part1 = 0;
    	uint64_t result_part2 = 0;

		auto op = operators[i];

		if (op == "*") {
			result_part1 = 1;
			for (auto& row : data) {
				result_part1 *= std::stoul(row[i]);
			}

			result_part2 = 1;
			for (int j = 0; j < col_max_size[i]; j++) {
				std::stringstream nr;
				for (auto& row : data) {
					nr << row[i][j];
				}
				result_part2 *= std::stoul(nr.str());
			}

		} else {

			for (auto& row : data) {
				result_part1 += std::stoul(row[i]);
			}

			for (int j = 0; j < col_max_size[i]; j++) {
				std::stringstream nr;
				for (auto& row : data) {
					nr << row[i][j];
				}
				result_part2 += std::stoul(nr.str());
			}
		}

		part1 += result_part1;
		part2 += result_part2;
	}

	// for (auto& l : lines) {
	// 	std::cout << "[" << l << "] " << std::endl;
	// }

	// std::cout << "operators: ";
	// for (auto& op : operators) {
	// 	std::cout << "[" << op << "] ";
	// }
	// std::cout << std::endl;

	// std::cout << "col sizes: ";
	// for (auto& c : col_max_size) {
	// 	std::cout << "[" << c << "] ";
	// }
	// std::cout << std::endl;

	// 		std::vector<std::string> row;
	// 		bool can_read_delimiter = false;
	// 		int column = 0;
	// 		std::string token;
	// 		for (int i = 0; i < l.size(); i++) {
	// 			auto c = l[i];
	// 			if (can_read_delimiter && c == ' ' && i < (l.size() - 1)  && l[i+1] != ' ') {
	// 				row.push_back(token);
	// 				token.clear();
	// 				column++;
	// 				can_read_delimiter = false;
	// 				continue;
	// 			}
	// 			if (std::isdigit(c)) {
	// 				can_read_delimiter = true;
	// 			}

	// 			token += c;
	// 		}
	// 		row.push_back(token);
	// 		data.push_back(row);
 
	// 	} else {

	// 		for (auto& row : data) {
	// 			for (auto& t : row) {
	// 				std::cout << "[" << t << "] ";
	// 			}
	// 			std::cout << std::endl;
	// 		}

	// 		std::string token;
	// 		int column = 0;
	// 	    for (std::stringstream ss(l); ss >> token; column++) {

	// 	    	uint64_t result_part1 = 0;
	// 	    	uint64_t result_part2 = 0;

	// 	    	if (token == "*") {
	// 	    		result_part1 = 1;
	// 	    		for (auto& row : data) {
	// 	    			result_part1 *= std::stoul(row[column]);
	// 	    		}

	// 	    		result_part2 = 1;
	// 	    		// for (int i = 0; data[0][column].size(); i++) {
	// 		    	// 	std::stringstream nr;
	// 		    	// 	for (auto& row : data) {
	// 		    	// 		if (std::isdigit(row[column][i])) {
	// 		    	// 			nr << row[column][i];
	// 		    	// 		} else {
	// 		    	// 			nr << "0";
	// 		    	// 		}
	// 		    	// 	}
	// 		    	// 	std::cout << "nr is: " << nr.str() << std::endl;
	// 		    	// 	result_part2 *= std::stoul(nr.str());
	// 	    		// }

	// 	    	} else {
	// 	    		for (auto& row : data) {
	// 	    			result_part1 += std::stoul(row[column]);
	// 	    		}

	// 	    		// for (int i = 0; data[0][column].size(); i++) {
	// 		    	// 	std::stringstream nr;
	// 		    	// 	for (auto& row : data) {
	// 		    	// 		if (std::isdigit(row[column][i])) {
	// 		    	// 			nr << row[column][i];
	// 		    	// 		} else {
	// 		    	// 			nr << "0";
	// 		    	// 		}
	// 		    	// 	}
	// 		    	// 	std::cout << "nr is: " << nr.str() << std::endl;
	// 		    	// 	result_part2 += std::stoul(nr.str());
	// 	    		// }
	// 	    	}
	// 	    	part1 += result_part1;
	// 	    	part2 += result_part2;
	// 	    }
	// 	}
	// }

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = end - start;

	std::cout << part1 << std::endl;
	std::cout << part2 << std::endl;

	auto us = std::chrono::duration_cast<std::chrono::microseconds>(duration);
    std::cout << "Time taken: " << us.count()/1000.0f << " ms" << std::endl;	

}