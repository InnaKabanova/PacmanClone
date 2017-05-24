#ifndef PINKY_H
#define PINKY_H

#include "ghost.hpp"

class Pinky: public Ghost
{
public:
    static Pinky& get_singleton();

private:
    Pinky();
    ~Pinky();
    Pinky(const Pinky&) = delete;
    Pinky& operator=(const Pinky&) = delete;

    virtual const char* get_name();
    virtual void set_state_first_level();
    virtual void set_state_second_level();
    virtual Position get_target_chasing();
};

#endif // PINKY_H

