#include <gtest/gtest.h>

#include "quad_tree.h"

TEST(TestQuadTree, EmptyQuadTree) {
    Rectangle rect{{0, 0}, {99, 99}};
    QuadTree q_tree(rect);
    EXPECT_EQ(q_tree.getCount(Circle({70, 70}, 5)), 0);
}

TEST(TestQuadTree, EqualTargetsPoints) {
    std::vector<Point> points = {{5, 5}, {5, 5}, {5, 5}};
    Rectangle rect{{0, 0}, {99, 99}};

    QuadTree q_tree(rect);

    // заполнение дерева точками
    for (const auto& p : points) {
        q_tree.insert(p);
    }
    EXPECT_EQ(q_tree.getCount(Circle({6, 6}, 5)), points.size());
}

TEST(TestQuadTree, NoTargetsFound) {
    std::vector<Point> points = {{5, 5}, {5, 6}, {16, 2}, {17, 6}, {40, 40}, {43, 35}, {45, 45}};
    Rectangle rect{{0, 0}, {99, 99}};

    QuadTree q_tree(rect);

    // заполнение дерева точками
    for (const auto& p : points) {
        q_tree.insert(p);
    }
    EXPECT_EQ(q_tree.getCount(Circle({70, 70}, 5)), 0);
}

TEST(TestQuadTree, FindTargetsOnEdge) {
    std::vector<Point> points = {{0, 0}, {0, 99}, {99, 0}, {99, 99}};
    Rectangle rect{{0, 0}, {99, 99}};

    QuadTree q_tree(rect);
    // заполнение дерева точками
    for (const auto& p : points) {
        q_tree.insert(p);
    }
    // проверка результата
    EXPECT_EQ(q_tree.getCount(Circle({1, 1}, 5)), 1);
    EXPECT_EQ(q_tree.getCount(Circle({1, 98}, 5)), 1);
    EXPECT_EQ(q_tree.getCount(Circle({98, 1}, 5)), 1);
    EXPECT_EQ(q_tree.getCount(Circle({98, 98}, 5)), 1);
}

TEST(TestQuadTree, FindTargetsOnTheMiddle) {
    std::vector<Point> points = {{50, 50}, {50, 50}};
    Rectangle rect{{0, 0}, {99, 99}};

    QuadTree q_tree(rect);
    // заполнение дерева точками
    for (const auto& p : points) {
        q_tree.insert(p);
    }
    // проверка результата
    EXPECT_EQ(q_tree.getCount(Circle({49, 49}, 5)), 2);
}

TEST(TestQuadTree, FindTragets) {
    std::vector<Point> points = {{5, 5}, {5, 6}, {16, 2}, {17, 6}, {40, 40}, {43, 35}, {45, 45}};
    Rectangle rect{{0, 0}, {99, 99}};

    QuadTree q_tree(rect);
    // заполнение дерева точками
    for (const auto& p : points) {
        q_tree.insert(p);
    }
    // проверка результата
    EXPECT_EQ(q_tree.getCount(Circle({5, 5}, 5)), 2);
    EXPECT_EQ(q_tree.getCount(Circle({15, 4}, 5)), 2);
    EXPECT_EQ(q_tree.getCount(Circle({35, 35}, 10)), 2);
    EXPECT_EQ(q_tree.getCount(Circle({35, 35}, 15)), 3);
}
