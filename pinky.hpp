#ifndef PINKY_H
#define PINKY_H

#include "ghost.hpp"

class Pinky: public Ghost
{
public:
    static Pinky& get_singleton();
    void move();

private:
    Pinky();
    ~Pinky();
    Pinky(const Pinky&) = delete;
    Pinky& operator=(const Pinky&) = delete;
};

#endif // PINKY_H
