#ifndef INKY_H
#define INKY_H

#include "ghost.hpp"

class Inky: public Ghost
{
public:
    static Inky& get_singleton();
    void move();

private:
    Inky();
    ~Inky();
    Inky(const Inky&) = delete;
    Inky& operator=(const Inky&) = delete;
};

#endif // INKY_H
