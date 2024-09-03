#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Point
{
    float x, y, z;
};

struct Node
{
    Point point;
    Node *left;
    Node *right;
};

Node *buildKDTree(vector<Point> &points, int depth = 0)
{
    if (points.empty())
        return nullptr;

    int axis = depth % 3;

    auto compared = [axis](const Point &a, const Point &b)
    {
        return axis == 0 ? a.x < b.x : (axis == 1 ? a.y < b.y : a.z < b.z);
    };

    sort(points.begin(), points.end(), compared);

    int medianIndex = points.size() / 2;
    Node *node = new Node{points[medianIndex], nullptr, nullptr};

    vector<Point> leftPoints(points.begin(), points.begin() + medianIndex);
    vector<Point> rightPoints(points.begin() + medianIndex + 1, points.end());

    node->left = buildKDTree(leftPoints, depth + 1);
    node->right = buildKDTree(rightPoints, depth + 1);

    return node;
}


void raycast(Node *node, const Point &rayStart, const Point &rayDirection, int depth = 0) {
    if (!node) return;

    cout << "Checking point (" << node->point.x << ", " << node->point.y << ", " << node->point.z << ")" << endl;

    int axis = depth % 3;
    float rayStartCoord = axis == 0 ? rayStart.x : (axis == 1 ? rayStart.y : rayStart.z);
    float nodeCoord = axis == 0 ? node->point.x : (axis == 1 ? node->point.y : node->point.z);
    float rayDirCoord = axis == 0 ? rayDirection.x : (axis == 1 ? rayDirection.y : rayDirection.z);

    if (rayDirCoord < 0) {
        raycast(node->left, rayStart, rayDirection, depth + 1);
        if (rayStartCoord > nodeCoord) {
            raycast(node->right, rayStart, rayDirection, depth + 1);
        }
    } else {
        raycast(node->right, rayStart, rayDirection, depth + 1);
        if (rayStartCoord <= nodeCoord) {
            raycast(node->left, rayStart, rayDirection, depth + 1);
        }
    }
}

int main()
{
    vector<Point> points = {{2, 3, 1}, {5, 4, 2}, {9, 6, 3}, {4, 7, 5}, {8, 1, 7}, {7, 2, 6}};

    Node *root = buildKDTree(points);
    Point rayStart = {0,0,0};
    Point rayDir;

    cout << "Enter in your ray's direction (x,y,z) !numbers 0-9 only!" << endl;
    cin >> rayDir.x >> rayDir.y >> rayDir.z;

    raycast(root, rayStart, rayDir);

    return 0;
}