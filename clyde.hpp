#ifndef CLYDE_H
#define CLYDE_H

#include "ghost.hpp"

class Clyde: public Ghost
{
public:
    static Clyde& get_singleton();

private:
    Clyde();
    ~Clyde();
    Clyde(const Clyde&) = delete;
    Clyde& operator=(const Clyde&) = delete;

    virtual const char* get_name();
    virtual void set_state_first_level();
    virtual void set_state_second_level();
    virtual Position get_target_chasing();
};

#endif // CLYDE_H


