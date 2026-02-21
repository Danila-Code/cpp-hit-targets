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
QuadTree::QuadNode::QuadNode() {
    points_.reserve(NODE_COUNT);
}

bool QuadTree::QuadNode::insertPoint(Point point) {
    if (points_.size() < NODE_COUNT) {
        points_.push_back(point);
        return true;
    }

    if (!subnodes_) {
        subdivideNode();
    }

    for (int i = 0; i < NODE_COUNT; ++i) {
        if (subnodes_[i].rect_.hasPoint(point)) {
            return subnodes_[i].insertPoint(point);
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

    if (subnodes_) {
        for (int i = 0; i < NODE_COUNT; ++i) {
            points_count += subnodes_[i].getCount(circle);
        }
    }
    return points_count;
}

const QuadTree::QuadNode* const QuadTree::QuadNode::getSubnodes() const {
    return subnodes_.get();
}

void QuadTree::QuadNode::setRect(Rectangle rect) {
    rect_ = rect;
}

Rectangle QuadTree::QuadNode::getRect() const {
    return rect_;
}

const std::vector<Point>& QuadTree::QuadNode::getPoints() const {
    return points_;
}

void QuadTree::QuadNode::subdivideNode() {
    const auto rects = divideRect();

    subnodes_ = std::make_unique<QuadNode[]>(NODE_COUNT);
    for (size_t i = 0; i < NODE_COUNT; ++i) {
        subnodes_[i].setRect(rects[i]);
    }
}

std::vector<Rectangle> QuadTree::QuadNode::divideRect() const {
    Point mid_point = rect_.getMidPoint();

    return {{rect_.left_down, mid_point},
            {{mid_point.x + 1, rect_.left_down.y}, {rect_.right_up.x, mid_point.y}},
            {{rect_.left_down.x, mid_point.y + 1}, {mid_point.x, rect_.right_up.y}},
            {{mid_point.x + 1, mid_point.y + 1}, rect_.right_up}};
}

//================= class QuadTree methods =============================
QuadTree::QuadTree(Rectangle rect) : root_{std::make_unique<QuadTree::QuadNode>()} {
    root_->setRect(rect);
}

void QuadTree::insert(Point point) {
    root_->insertPoint(point);
}

size_t QuadTree::getCount(Circle circle) const {
    return root_->getCount(circle);
}

const QuadTree::QuadNode* const QuadTree::getRoot() const {
    return root_.get();
}

const std::vector<Point> QuadTree::getPoints() const {
    std::vector<Point> all_points;

    getPointsRec(root_.get(), all_points);

    return all_points;
}

void QuadTree::getPointsRec(const QuadNode* const node, std::vector<Point>& all_points) const {
    if (!node) {
        return;
    }

    auto points = node->getPoints();

    all_points.insert(all_points.end(), points.begin(), points.end());

    auto subnodes = node->getSubnodes();
    if (!subnodes) {
        return;
    }

    for (int i = 0; i < NODE_COUNT; ++i) {
        getPointsRec(&subnodes[i], all_points);
    }
}
