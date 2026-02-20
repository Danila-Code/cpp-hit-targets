#include "quad_tree.h"

bool numberInBorder(int num, int min, int max) {
    return num >= min && num <= max;
}

int distanceSquare(Point p1, Point p2) {
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    return dx * dx + dy * dy;
}

//================= struct Circle methods =============================
bool Circle::hasPoint(Point point) const {
    return distanceSquare(center, point) <= radius * radius;
}

//================= struct Rectangle methods =============================
Rectangle::Rectangle(Point l_down, Point r_up) : left_down(l_down), right_up(r_up) {}

Point Rectangle::getMidPoint() const {
    return Point{left_down.x + getWidth(), left_down.y + getHeight()};
}

int Rectangle::getHeight() const {
    return right_up.y - left_down.y + 1;
}

int Rectangle::getWidth() const {
    return right_up.x - left_down.x + 1;
}

bool Rectangle::hasPoint(const Point& point) const {
    return numberInBorder(point.x, left_down.x, right_up.x) && numberInBorder(point.y, left_down.y, right_up.y);
}

bool Rectangle::intersectCircle(const Circle& circle) const {
    int closest_x = std::max(left_down.x, std::min(right_up.x, circle.center.x));
    int closest_y = std::max(left_down.y, std::min(right_up.y, circle.center.y));

    return circle.hasPoint(Point(closest_x, closest_y));
}

//================= class QuadTree::QuadNode methods =============================
QuadTree::QuadNode::QuadNode(Rectangle rect) : rect_{rect} {
    points_.reserve(NODE_COUNT);
}

bool QuadTree::QuadNode::insertPoint(Point point) {
    if (points_.size() < NODE_COUNT) {
        points_.push_back(point);
        return true;
    }

    if (!sub_nodes_) {
        subdivideNode();
    }

    for (auto& subnode : sub_nodes_.value()) {
        if (subnode.rect_.hasPoint(point)) {
            return subnode.insertPoint(point);
        }
    }
    return false;
}

size_t QuadTree::QuadNode::getCount(Circle circle) const {
    size_t points_count = 0;

    if (!rect_.intersectCircle(circle)) {
        return points_count;
    }

    for (const auto& p : points_) {
        if (circle.hasPoint(p)) {
            ++points_count;
        }
    }

    if (sub_nodes_.has_value()) {
        for (const auto& node : sub_nodes_.value()) {
            points_count += node.getCount(circle);
        }
    }
    return points_count;
};

void QuadTree::QuadNode::subdivideNode() {
    const auto rects = divideRect();
    std::vector<QuadNode> subnodes;
    subnodes.reserve(NODE_COUNT);
    for (size_t i = 0; i < rects.size(); ++i) {
        subnodes.push_back(QuadNode(rects[i]));
    }
    sub_nodes_ = subnodes;
}

std::vector<Rectangle> QuadTree::QuadNode::divideRect() const {
    Point mid_point = rect_.getMidPoint();

    return {{rect_.left_down, mid_point},
            {{mid_point.x + 1, rect_.left_down.y}, {rect_.right_up.x, mid_point.y}},
            {{rect_.left_down.x, mid_point.y + 1}, {mid_point.x, rect_.right_up.y}},
            {{mid_point.x + 1, mid_point.y + 1}, rect_.right_up}};
}

//================= class QuadTree methods =============================
QuadTree::QuadTree(Rectangle rect) : root_{std::make_unique<QuadTree::QuadNode>(rect)} {}

void QuadTree::insert(Point point) {
    root_->insertPoint(point);
}

size_t QuadTree::getCount(Circle circle) const {
    return root_->getCount(circle);
}
