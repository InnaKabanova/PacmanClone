#include <QDebug>
#include "pinky.hpp"

#define PINKY_DEBUG

Pinky& Pinky::get_singleton()
{
    #ifdef PINKY_DEBUG
    qDebug() << "'Pinky::get_singleton'";
    #endif
    static Pinky pinky;
    return pinky;
}

Pinky::Pinky(): Ghost(8, 9, "qrc:/QMLs/QMLs/Pinky.qml", 0, 2, 1, 2, 3)
{
    #ifdef PINKY_DEBUG
    qDebug() << "'Pinky' constructor";
    #endif
}

Pinky::~Pinky()
{
    #ifdef PINKY_DEBUG
    qDebug() << "'Pinky' destructor";
    #endif
}

void Pinky::set_state_first_level()
{
    x = init_x;
    y = init_y;
    direction = GhostDirection::G_UP;
    speed = 250;
    move_timer.setInterval(speed);
    redraw();
}

void Pinky::set_state_second_level()
{
    x = init_x;
    y = init_y;
    direction = GhostDirection::G_UP;
    speed = 270;
    move_timer.setInterval(speed);
    redraw();
}

const char* Pinky::get_name()
{
    return "Pinky";
}

Position Pinky::get_target_chasing()
{
    // TO DO

    unsigned int pacman_x = Pacman::get_singleton().get_x();
    unsigned int pacman_y = Pacman::get_singleton().get_y();

    Position destination(pacman_x, pacman_y);
    return destination;
}
