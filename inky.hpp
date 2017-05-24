#ifndef INKY_H
#define INKY_H

#include "ghost.hpp"

class Inky: public Ghost
{
public:
    static Inky& get_singleton();

private:
    Inky();
    ~Inky();
    Inky(const Inky&) = delete;
    Inky& operator=(const Inky&) = delete;

    virtual const char* get_name();
    virtual void set_state_first_level();
    virtual void set_state_second_level();
    virtual Position get_target_chasing();
};

#endif // INKY_H


