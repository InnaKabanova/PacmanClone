#include <QDebug>
#include "inky.hpp"
#include "blinky.hpp"

// #define INKY_DEBUG

Inky& Inky::get_singleton()
{
    #ifdef INKY_DEBUG
    qDebug() << "'Inky::get_singleton'";
    #endif
    static Inky inky;
    return inky;
}

Inky::Inky(): Ghost(9, 9, "qrc:/QMLs/QMLs/Inky.qml", 3000, 13, 15, 13, 19)
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
    QTimer::singleShot(time_to_leave, this, SLOT(leave_the_house()));
}

void Inky::set_state_second_level()
{
    x = init_x;
    y = init_y;
    direction = GhostDirection::G_UP;
    speed = 270;
    move_timer.setInterval(speed);
    redraw();
    QTimer::singleShot(time_to_leave, this, SLOT(leave_the_house()));
}

const char* Inky::get_name()
{
    return "Inky";
}

Position Inky::get_target_chasing()
{
    // 1. Find coordinates of Blinky's position:
    unsigned int x_1 = Blinky::get_singleton().get_x();
    unsigned int y_1 = Blinky::get_singleton().get_y();

    // 2. Find coordinates of a position that is 2 tiles ahead of Pacman:
    unsigned int pacman_x = Pacman::get_singleton().get_x();
    unsigned int pacman_y = Pacman::get_singleton().get_y();
    unsigned int pacman_direction = Pacman::get_singleton().get_direction();

    unsigned int x_2;
    unsigned int y_2;

    if(pacman_direction == PacmanDirection::PAC_UP)
    {
        x_2 = pacman_x;
        y_2 = pacman_y - 2;
    }
    else if(pacman_direction == PacmanDirection::PAC_DOWN)
    {
        x_2 = pacman_x;
        y_2 = pacman_y + 2;
    }
    else if(pacman_direction == PacmanDirection::PAC_LEFT)
    {
        x_2 = pacman_x - 2;
        y_2 = pacman_y;
    }
    else if(pacman_direction == PacmanDirection::PAC_RIGHT)
    {
        x_2 = pacman_x + 2;
        y_2 = pacman_y;
    }

    // 3. Calculate coordinates of a target tile:

    Position target;
    target.x = x_1 + 2*(x_2 - x_1);
    target.y = y_1 + 2*(y_2 - y_1);

    if(RoomManager::get_singleton().is_valid(target))
        return target;
    else
        return {pacman_x, pacman_y};

}
