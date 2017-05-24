#include <QDebug>
#include "clyde.hpp"

#define CLYDE_DEBUG

Clyde& Clyde::get_singleton()
{
    #ifdef CLYDE_DEBUG
    qDebug() << "'Clyde::get_singleton'";
    #endif
    static Clyde clyde;
    return clyde;
}

Clyde::Clyde(): Ghost(10, 9, "qrc:/QMLs/QMLs/Clyde.qml", 10000, 5, 15, 5, 19)
{
    #ifdef CLYDE_DEBUG
    qDebug() << "'Clyde' constructor";
    #endif
}

Clyde::~Clyde()
{
    #ifdef CLYDE_DEBUG
    qDebug() << "'Clyde' destructor";
    #endif
}

void Clyde::set_state_first_level()
{
    x = init_x;
    y = init_y;
    direction = GhostDirection::G_UP;
    speed = 250;
    move_timer.setInterval(speed);
    redraw();
}

void Clyde::set_state_second_level()
{
    x = init_x;
    y = init_y;
    direction = GhostDirection::G_UP;
    speed = 270;
    move_timer.setInterval(speed);
    redraw();
}

const char* Clyde::get_name()
{
    return "Clyde";
}

Position Clyde::get_target_chasing()
{
    // TO DO

    unsigned int pacman_x = Pacman::get_singleton().get_x();
    unsigned int pacman_y = Pacman::get_singleton().get_y();

    Position destination(pacman_x, pacman_y);
    return destination;
}

