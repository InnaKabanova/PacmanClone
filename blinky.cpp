#include <QDebug>
#include <cstdlib>
#include "blinky.hpp"

// #define BLINKY_DEBUG

using std::stack;

Blinky::Blinky(): Ghost(9, 7, "qrc:/QMLs/QMLs/Blinky.qml", 0, 16, 1, 16, 5)
{
    #ifdef BLINKY_DEBUG
    qDebug() << "'Blinky' constructor";
    #endif
}

Blinky& Blinky::get_singleton()
{
    #ifdef BLINKY_DEBUG
    qDebug() << "'Blinky::get_singleton'";
    #endif

    static Blinky blinky;
    return blinky;
}

Blinky::~Blinky()
{
    #ifdef BLINKY_DEBUG
    qDebug() << "'Blinky' destructor";
    #endif
}

void Blinky::set_state_first_level()
{
    x = init_x;
    y = init_y;
    direction = GhostDirection::G_UP;
    speed = 250;
    move_timer.setInterval(speed);
    redraw();
}

void Blinky::set_state_second_level()
{
    x = init_x;
    y = init_y;
    direction = GhostDirection::G_UP;
    speed = 270;
    move_timer.setInterval(speed);
    redraw();
}

const char* Blinky::get_name()
{
    return "Blinky";
}

Position Blinky::get_target_chasing()
{
    unsigned int pacman_x = Pacman::get_singleton().get_x();
    unsigned int pacman_y = Pacman::get_singleton().get_y();

    Position destination(pacman_x, pacman_y);
    return destination;
}







