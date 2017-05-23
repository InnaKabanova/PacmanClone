#ifndef POSITION_H
#define POSITION_H

// Struct to hold (x,y) pairs which form ghost's
// movement trajectory
struct Position
{
    int x;
    int y;

    Position* parent;

    double g; // how far this node is located from a starting node
    double h; // (heuristic) how far this node is located from a destination node
    double f; // (g + h)

    Position(){}

    Position(unsigned int use_x, unsigned int use_y):
        x(use_x),
        y(use_y),
        parent(nullptr),
        g(0),
        h(0),
        f(0)
    {}

    Position(const Position& another)
    {
        x = another.x;
        y = another.y;
        parent = another.parent;
        g = another.g;
        h = another.h;
        f = another.f;
    }

    Position& operator=(const Position& another)
    {
        x = another.x;
        y = another.y;
        parent = another.parent;
        g = another.g;
        h = another.h;
        f = another.f;

        return *this;
    }
};

#endif // POSITION_H
