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


uint64_t area_from_corners(int64_t x1, int64_t y1, int64_t x2, int64_t y2) {
	uint64_t w = std::abs(x2 - x1) + 1;
	uint64_t h = std::abs(y2 - y1) + 1;
	return w*h;
}

int64_t cross_product(int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t x3, int64_t y3) {
	std::cout << "cross-product of (" << x1 << ", " << y1 << "), (" << x2 << ", " << y2 << "), (" << x3 << ", " << y3 << ")" << std::endl;
    return (x2 - x1) * (y3 - y1) - 
           (y2 - y1) * (x3 - x1);
}

bool is_ccw(int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t x3, int64_t y3) {
	return (y3 - y1) * (x2 - x1) > (y2 - y1) * (x3 - x1);
}

bool segments_intersect(
	int64_t x1, int64_t y1, 
	int64_t x2, int64_t y2, 
	int64_t x3, int64_t y3, 
	int64_t x4, int64_t y4) {
	return	((is_ccw(x1, y1, x3, y3, x4, y4) != 
			 is_ccw(x2, y2, x3, y3, x4, y4)) 
			&&
			(is_ccw(x1, y1, x2, y2, x3, y3) != 
			 is_ccw(x1, y1, x2, y2, x4, y4)));
}

bool on_segment(int64_t x1, int64_t y1, 
				int64_t x2, int64_t y2, 
				int64_t x3, int64_t y3) {
	return ((x2 <= std::max(x1, x3)) &&
			(x2 >= std::min(x1, x3)) &&
			(y2 <= std::max(y1, y3)) &&
			(y2 >= std::min(y1, y3)));
}

struct bbox {
	int min_x;
	int min_y;
	int max_x;
	int max_y;

	bool contains(int x, int y) {
		return (x >= min_x && x <= max_x && y >= min_y && y <= max_y);
	}

	bool contains_strict(int x, int y) {
		return (x > min_x && x < max_x && y > min_y && y < max_y);
	}
};


// function to find orientation of ordered triplet (p, q, r)
// 0 --> p, q and r are collinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(
	int64_t x1, int64_t y1, 
	int64_t x2, int64_t y2, 
	int64_t x3, int64_t y3) {
    int val = (y2 - y1) * (x3 - x2) -
              (x2 - x1) * (y3 - y2);

    // collinear
    if (val == 0) return 0;

    // clock or counterclock wise
    // 1 for clockwise, 2 for counterclockwise
    return (val > 0 && x2 != x3) ? 1 : 2;
}


bool segments_intersect2(
	int64_t x1, int64_t y1, 
	int64_t x2, int64_t y2, 
	int64_t x3, int64_t y3, 
	int64_t x4, int64_t y4) {

	int o1 = orientation(x1, y1, x2, y2, x3, y3);
	int o2 = orientation(x1, y1, x2, y2, x4, y4);
	int o3 = orientation(x3, y3, x4, y4, x1, y1);
	int o4 = orientation(x3, y3, x4, y4, x1, y1);

	// general case
    if (o1 != o2 && o3 != o4)
        return true;

	 // special cases
    // p1, q1 and p2 are collinear and p2 lies on segment p1q1
    if (o1 == 0 && on_segment(x1, y1, x3, y3, x2, y2))
    	return true;

    // p1, q1 and q2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && on_segment(x1, y1, x4, y4, x2, y2))
    	return true;


    // p2, q2 and p1 are collinear and p1 lies on segment p2q2
    if (o3 == 0 && on_segment(x3, y3, x1, y1, x4, y4))
    	return true;

    // p2, q2 and q1 are collinear and q1 lies on segment p2q2 
    if (o4 == 0 && on_segment(x3, y3, x2, y2, x4, y4))
    	return true;

    return false;
}

bool is_point_on_straight_edge(
	int64_t x, int64_t y, 
	int64_t x1, int64_t y1, 
	int64_t x2, int64_t y2) {


	if (x1 == x2) {
		return (x == x1 && y >= std::min(y1, y2) && y <= std::max(y1, y2));
		// vertical
	} else {
		return (y == y1 && x >= std::min(x1, x2) && x <= std::max(x1, x2));
	}
}

int edge_for_point(const std::vector<std::pair<int64_t, int64_t>>& locations, int64_t x, int64_t y) {
	int result = -1;
	for (int i = 0; i < locations.size(); i++) {
		int j = (i + 1) % locations.size();
		if (is_point_on_straight_edge(x, y, locations[i].first, locations[i].second, locations[j].first, locations[j].second)) {
			return i;
		}
		// if (on_segment(locations[i].first, locations[i].second, x, y, locations[j].first, locations[j].second))
		// 	return i;
	}
	return result;
}


std::map<std::pair<int64_t, int64_t>, bool> point_cache;

std::vector<bbox> valid_rectangles;


bool point_inside_polygon(const std::vector<std::pair<int64_t, int64_t>>& locations, int64_t x1, int64_t y1, const bbox& box) {
	if (x1 < box.min_x || x1 > box.max_x || y1 < box.min_y || y1 > box.max_y) {
		return false;
	}	

	auto p = std::pair<int64_t, int64_t>({x1, y1});
	if (point_cache.count(p) > 0) {
		return point_cache[p];
	}

	for (auto& b : valid_rectangles) {
		if (b.contains(x1, y1)) {
			point_cache[p] = true;
			return true;
		}
	}


	// is on an edge?
	if (edge_for_point(locations, x1, y1) != -1) {
		point_cache[p] = true;
		return true;
	}

	// check if surounded by edges in all parts or gets out of the bbox

	// go left
	// if (!point_inside_polygon(locations, x1 - 1, y1, box)) {
	// 	point_cache[p] = false;
	// 	return false;
	// }

	// if (!point_inside_polygon(locations, x1 + 1, y1, box)) {
	// 	point_cache[p] = false;
	// 	return false;
	// }

	// if (!point_inside_polygon(locations, x1, y1-1, box)) {
	// 	point_cache[p] = false;
	// 	return false;
	// }

	// if (!point_inside_polygon(locations, x1, y1 + 1, box)) {
	// 	point_cache[p] = false;
	// 	return false;
	// }

	bool edge_found = false;
	int x_left = 0;
	int x_right = 0;
	int y_up = 0;
	int y_down = 0;
	for (int x = x1-1; x >= box.min_x; x--) {
		if (edge_for_point(locations, x, y1) != -1) {
			edge_found = true;
			x_left = x;
			break;
		}
	}
	if (!edge_found) {
		point_cache[p] = false;
		return false;
	}

	// go right
	edge_found = false;
	for (int x = x1+1; x <= box.max_x; x++) {
		if (edge_for_point(locations, x, y1) != -1) {
			edge_found = true;
			x_right = x;
			break;
		}
	}
	if (!edge_found) {
		point_cache[p] = false;
		return false;
	}

	// go up
	edge_found = false;
	for (int y = y1-1; y >= box.min_y; y--) {
		if (edge_for_point(locations, x1, y) != -1) {
			edge_found = true;
			y_up = y;
			break;
		}
	}
	if (!edge_found) {
		point_cache[p] = false;
		return false;
	}

	// go down
	edge_found = false;
	for (int y = y1+1; y <= box.max_y; y++) {
		if (edge_for_point(locations, x1, y) != -1) {
			edge_found = true;
			y_down = y;
			break;
		}
	}
	if (!edge_found) {
		point_cache[p] = false;
		return false;
	}


	point_cache[p] = true;

	// save our traces in the cache
	for (int x = x_left; x <= x_right; x++) {
		point_cache[{x, y1}] = true;
	}

	for (int y = y_up; y <= y_down; y++) {
		point_cache[{x1, y}] = true;
	}

	return true;


	// int64_t x2 = 10000000; // check how many edges intersect a horizontal line
	// int64_t y2 = y1;


	// bool has_cw = false;
	// bool has_ccw = false;
	// for (int i = 0; i < locations.size(); i++) {
	// 	auto next_i = (i + 1) % locations.size();

	// 	auto o = orientation(x1, y1, 
	// 			locations[i].first, locations[i].second,
	// 			locations[next_i].first, locations[next_i].second);
	// 	// std::cout << "orientation with " << locations[i].first << "," << locations[i].second << " and " << locations[next_i].first << "," << locations[next_i].second << " is" << o << std::endl;
	// 	switch (o) {
	// 	case 1:
	// 			has_cw = true;
	// 			break;
	// 	case 2:
	// 			has_ccw = true;
	// 			break;
	// 	}

	// 	if (has_cw && has_ccw)
	// 		return false;
	// }

	// return true;

	// std::cout << "x1: " << x1 << ", y1: " << y1 << std::endl;
	// int64_t x2 = 10000000; // check how many edges intersect a horizontal line
	// int64_t y2 = y1;

	// int intersections = 0;
	// for (int i = 0; i < locations.size(); i++) {
	// 	auto next_i = (i + 1) % locations.size();

	// 	if (locations[i].first == x1 && locations[i].second == y1)
	// 		return true;

	// 	if (locations[next_i].first == x1 && locations[next_i].second == y1)
	// 		return true;

	// 	if (segments_intersect2(
	// 			x1, y1,
	// 			x2, y2,
	// 			locations[i].first, locations[i].second,
	// 			locations[next_i].first, locations[next_i].second)) {
	// 		std::cout << "intersection with: (" << locations[i].first << ", " << locations[i].second << ") - (" << locations[next_i].first << "," << locations[next_i].second << ")" << std::endl;
	// 		intersections++;
	// 	}
	// }

	// auto result = ((intersections % 2) == 0);
	// std::cout << "intersections: " << intersections << ". contained: " << result << std::endl;
	// return result;


	// int neg_times = 0;
	// int pos_times = 0;
	// int zero_times = 0;

	// for (int i = 0; i < locations.size(); i++) {
	// 	auto next_i = (i + 1) % locations.size();
	// 	if (locations[i].first == x1 && locations[i].second == y1)
	// 		return true;
	// 	if (locations[next_i].first == x1 && locations[next_i].second == y1)
	// 		return true;

	// 	auto cp = cross_product(
	// 		x1, y1, 
	// 		locations[i].first,
	// 		locations[i].second,
	// 		locations[next_i].first,
	// 		locations[next_i].second
	// 	);

	// 	std::cout << "cp(" << i << ", " << next_i << "): " << cp << std::endl;


	// 	if (cp == 0)
	// 		zero_times++;
		
	// 	if (cp < 0)
	// 		neg_times++;

	// 	if (cp > 0)
	// 		pos_times++;

	// }
	// std::cout << "neg_times: " << neg_times << ", pos_times: " << pos_times << ", zero_times: " << zero_times << std::endl;

	// if ((neg_times & 1) == 1)
	// 	return false;

	// return true;
}



bool is_valid_rectangle(const std::vector<std::pair<int64_t, int64_t>>& locations, int a, int b) {
	int64_t x1 = locations[a].first;
	int64_t y1 = locations[a].second;

	int64_t x2 = locations[b].first;
	int64_t y2 = locations[b].second;

	// a one line rectangle
	if (x1 == x2 || y1 == y2)
		return true;


	// check perimeter inside polygon
	int64_t from_y = std::min(y1, y2);
	int64_t from_x = std::min(x1, x2);
	int64_t to_x = std::max(x1, x2);
	int64_t to_y = std::max(y1, y2);

	// [9:14 PM, 12/9/2025] Bogdan Mocanu: Efectiv sunt doar 3 cazuri de verificat la un area vs un edge
	// [9:16 PM, 12/9/2025] Bogdan Mocanu: 1) edge trece transversal stanga-dreapta prin area (y trebuie sa fie strict intre y-ii ariei)
	// 									2) edge trece transversal sus-jos prin area (x trebuie sa fie strict intre x-ii ariei)
	// 									3) edge are un capat inauntrul ariei, iarasi verificare stricta

	// Cuvantul strict de mai sus (comparare cu > <, nu >=, <=) rezolva cea mai mare problema: edge-urile care trec pe bordura ariei

	bbox bb{from_x, from_y, to_x, to_y};
	for (int i = 0; i < locations.size(); i++) {
		int j = (i + 1) % locations.size();

		bool is_horizontal = locations[i].second == locations[j].second;
		if (is_horizontal 
			&& locations[i].second > from_y 
			&& locations[i].second < to_y 
			&& locations[i].first <= to_x 
			&& locations[j].first >= from_x) {
			return false;
		}

		if (!is_horizontal 
			&& locations[i].first > from_x 
			&& locations[i].first < to_x 
			&& locations[i].second < to_y 
			&& locations[j].second > from_y) {
			return false;
		}

		if (bb.contains_strict(locations[i].first, locations[i].second) ||
			bb.contains_strict(locations[j].first, locations[j].second)) {
			return false;
		}
	}


	return true;
}

int main() {
	std::fstream f{"input.txt"};

	uint64_t part1 = 0;
	uint64_t part2 = 0;

	auto start = std::chrono::high_resolution_clock::now();

	std::vector<std::pair<int64_t, int64_t>> locations;
	for (std::string l; std::getline(f, l);) {
		std::stringstream ss(l);
		int a, b;
		char comma;
		ss >> a >> comma >> b;
		locations.push_back({a, b});
	}

	std::map<uint64_t, std::pair<int, int>> areas;
	for (int i = 0; i < (locations.size() - 1); i++) {
		for (int j = i+1; j < locations.size(); j++) {
			uint64_t area = area_from_corners(
					locations[i].first,
					locations[i].second,
					locations[j].first,
					locations[j].second
				);
			areas[area] = {i, j};
		}
	}

	auto it = areas.rbegin();
	part1 = (*it).first;

	while (it != areas.rend()) {
		auto p = (*it).second;
		// std::cout << (*it).first << std::endl;
		if (is_valid_rectangle(locations, p.first, p.second)) {
			part2 = (*it).first;
			break;
		} 
		it++;
	}


	auto end = std::chrono::high_resolution_clock::now();
	auto duration = end - start;

	std::cout << part1 << std::endl;
	std::cout << part2 << std::endl;

	auto us = std::chrono::duration_cast<std::chrono::microseconds>(duration);
    std::cout << "Time taken: " << us.count()/1000.0f << " ms" << std::endl;	

}