#ifndef BLINKY_H
#define BLINKY_H

#include "ghost.hpp"

class Blinky: public Ghost
{
public:
    static Blinky& get_singleton();

private:
    Blinky();
    ~Blinky();
    Blinky(const Blinky&) = delete;
    Blinky& operator=(const Blinky&) = delete;

    virtual void set_state_first_level();
    virtual void set_state_second_level();

    virtual Position get_target_chasing();
    virtual Position get_target_scattering();
    virtual Position get_target_frightened();



};

#endif // BLINCKY_H
