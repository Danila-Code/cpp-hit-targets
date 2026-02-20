#pragma once

#include <memory>
#include <optional>
#include <vector>

const int NODE_COUNT = 4;

struct Point {
    Point(int x_coord, int y_coord) : x(x_coord), y(y_coord) {}

    int x;
    int y;
};

struct Circle {
    Circle(Point point, int r) : center(point), radius(r) {}

    bool hasPoint(Point point) const;

    Point center;
    int radius;
};

struct Rectangle {
    Rectangle(Point l_down, Point r_up);
    Point getMidPoint() const;

    int getHeight() const;
    int getWidth() const;

    bool hasPoint(const Point& point) const;
    bool intersectCircle(const Circle& circle) const;

    Point left_down;
    Point right_up;
};

class QuadTree {
public:
    explicit QuadTree(Rectangle rect);
    ~QuadTree() {

    };

    void insert(Point point);

    size_t getCount(Circle circle) const;

private:
    class QuadNode {
    public:
        explicit QuadNode(Rectangle rect);

        bool insertPoint(Point point);

        size_t getCount(Circle circle) const;

    private:
        void subdivideNode();

        std::vector<Rectangle> divideRect() const;

    private:
        Rectangle rect_;
        std::vector<Point> points_;

        // four nodes that divide node rect_ on four rectangle areas
        // order of sub_nodes_: left down, right down, left up, right up
        std::optional<std::vector<QuadNode>> sub_nodes_;
    };

private:
    std::unique_ptr<QuadNode> root_;
};
