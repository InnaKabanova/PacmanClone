#ifndef CLYDE_H
#define CLYDE_H

#include "ghost.hpp"

class Clyde: public Ghost
{
public:
    static Clyde& get_singleton();
    void move();

private:
    Clyde();
    ~Clyde();
    Clyde(const Clyde&) = delete;
    Clyde& operator=(const Clyde&) = delete;
};

#endif // CLYDE_H
