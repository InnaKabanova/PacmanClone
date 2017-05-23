#include <QDebug>
#include "blinky.hpp"

// #define BLINKY_DEBUG
#define BLINKY_TARGET_DEBUG

using std::stack;
using std::swap;

Blinky::Blinky(): Ghost(9, 7, "qrc:/QMLs/QMLs/Blinky.qml", 0)
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
    speed = 1000;
    move_timer.setInterval(speed);
    redraw();
}

void Blinky::set_state_second_level()
{
    x = init_x;
    y = init_y;
    direction = GhostDirection::G_UP;
    speed = 1000;
    move_timer.setInterval(speed);
    redraw();
}

Position Blinky::get_target_chasing()
{
    unsigned int pacman_x = Pacman::get_singleton().get_x();
    unsigned int pacman_y = Pacman::get_singleton().get_y();
    Position destination(pacman_x, pacman_y);

    #ifdef BLINKY_TARGET_DEBUG
    qDebug() << "'Blinky::get_target_chasing'. Target: " << destination.x << destination.y;
    #endif

    return destination;
}

Position Blinky::get_target_scattering()
{
    #ifdef BLINKY_TARGET_DEBUG
    qDebug() << "'Blinky::get_target_scattering'. TO DO";
    #endif

    Position destination(0, 0);
    return destination;

}

Position Blinky::get_target_frightened()
{
    #ifdef BLINKY_TARGET_DEBUG
    qDebug() << "'Blinky::get_target_frightened'. TO DO";
    #endif

    Position destination(0, 0);
    return destination;
}






