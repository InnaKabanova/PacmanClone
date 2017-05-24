#include <QDebug>
#include "inky.hpp"

#define INKY_DEBUG

Inky& Inky::get_singleton()
{
    #ifdef INKY_DEBUG
    qDebug() << "'Inky::get_singleton'";
    #endif
    static Inky inky;
    return inky;
}

Inky::Inky(): Ghost(9, 9, "qrc:/QMLs/QMLs/Inky.qml", 5000, 13, 15, 13, 19)
{
    #ifdef INKY_DEBUG
    qDebug() << "'Inky' constructor";
    #endif
}

Inky::~Inky()
{
    #ifdef INKY_DEBUG
    qDebug() << "'Inky' destructor";
    #endif
}

void Inky::set_state_first_level()
{
    x = init_x;
    y = init_y;
    direction = GhostDirection::G_UP;
    speed = 250;
    move_timer.setInterval(speed);
    redraw();
}

void Inky::set_state_second_level()
{
    x = init_x;
    y = init_y;
    direction = GhostDirection::G_UP;
    speed = 270;
    move_timer.setInterval(speed);
    redraw();
}

const char* Inky::get_name()
{
    return "Inky";
}

Position Inky::get_target_chasing()
{
    // TO DO

    unsigned int pacman_x = Pacman::get_singleton().get_x();
    unsigned int pacman_y = Pacman::get_singleton().get_y();

    Position destination(pacman_x, pacman_y);
    return destination;
}
