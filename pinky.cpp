#include <QDebug>
#include "pinky.hpp"

// #define PINKY_DEBUG

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
    QTimer::singleShot(time_to_leave, this, SLOT(leave_the_house()));
}

void Pinky::set_state_second_level()
{
    x = init_x;
    y = init_y;
    direction = GhostDirection::G_UP;
    speed = 270;
    move_timer.setInterval(speed);
    redraw();
    QTimer::singleShot(time_to_leave, this, SLOT(leave_the_house()));
}

const char* Pinky::get_name()
{
    return "Pinky";
}

Position Pinky::get_target_chasing()
{
    // Pinky’s target tile in 'Chase' mode is determined by looking at Pacman’s current position
    // and orientation, and selecting the location four tiles straight ahead of Pacman:

    unsigned int pacman_x = Pacman::get_singleton().get_x();
    unsigned int pacman_y = Pacman::get_singleton().get_y();
    unsigned int pacman_direction = Pacman::get_singleton().get_direction();

    Position destination(pacman_x, pacman_y);

    if(pacman_direction == PacmanDirection::PAC_UP)
        destination.y = y - 4;
    else if(pacman_direction == PacmanDirection::PAC_DOWN)
        destination.y = y + 4;
    else if(pacman_direction == PacmanDirection::PAC_LEFT)
        destination.x = x - 4;
    else if(pacman_direction == PacmanDirection::PAC_RIGHT)
        destination.x = x + 4;

    if(RoomManager::get_singleton().is_valid(destination))
        return destination;
    else
        return {pacman_x, pacman_y};
}
