#include <format>
#include <fstream>
#include <iostream>
#include <string>

#include "quad_tree.h"

const int width = 99;
const int height = 99;

std::ostream& operator<<(std::ostream& out, Point point) {
    out << "(" << point.x << ", " << point.y << ")";
    return out;
}

Point getPoint(const std::string& str) {
    size_t delim_pos = str.find(',');
    return Point{atoi(str.substr(0, delim_pos).c_str()), atoi(str.substr(delim_pos + 1).c_str())};
}

struct HitPoint {
    Point hit_point;
    size_t targets_count;
};

HitPoint getHitPointMaxTargets(const QuadTree& q_tree, int radius) {
    Point launch_point{0, 0};
    size_t max_targets_count = 0;

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            size_t targets_count = q_tree.getCount(Circle({x, y}, radius));

            if (targets_count > max_targets_count) {
                max_targets_count = targets_count;
                launch_point.x = x;
                launch_point.y = y;
            }
        }
    }
    return {launch_point, max_targets_count};
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::string err = std::format("You need to enter: {} <file with coordinates> <radius of damage>\n", argv[0]);
        std::cerr << err;
        return 1;
    }

    const std::string file_name(argv[1]);
    const int radius = atoi(argv[2]);

    std::ifstream file(file_name);
    if (!file) {
        std::cerr << "Can not open file " << file_name;
        return 1;
    }

    Rectangle rect(Point{0, 0}, Point{width, height});
    QuadTree q_tree(rect);

    std::string line;
    while (getline(file, line)) {
        q_tree.insert(getPoint(line));
    }

    HitPoint launch_point = getHitPointMaxTargets(q_tree, radius);

    std::cout << "Launch coordinates: " << launch_point.hit_point << std::endl;
    std::cout << "Targets count: " << launch_point.targets_count << std::endl;
    return 0;
}