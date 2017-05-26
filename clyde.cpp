#include <QDebug>
#include "clyde.hpp"

// #define CLYDE_DEBUG

using std::vector;

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
    QTimer::singleShot(time_to_leave, this, SLOT(leave_the_house()));
}

void Clyde::set_state_second_level()
{
    x = init_x;
    y = init_y;
    direction = GhostDirection::G_UP;
    speed = 270;
    move_timer.setInterval(speed);
    redraw();
    QTimer::singleShot(time_to_leave, this, SLOT(leave_the_house()));
}

const char* Clyde::get_name()
{
    return "Clyde";
}

vector<unsigned int> random_x = {4, 14, 8, 4, 14, 4, 14, 8, 4, 14};
vector<unsigned int> random_y = {1, 19, 3, 13, 1, 19, 3, 13, 1, 19};

Position Clyde::get_target_chasing()
{   
    // Whenever Clyde needs to determine his target tile, he first calculates his
    // distance from Pac-Man.
    unsigned int pacman_x = Pacman::get_singleton().get_x();
    unsigned int pacman_y = Pacman::get_singleton().get_y();

    Position curr_position(x, y);
    Position destination(pacman_x, pacman_y);

    int distance = (int)(RoomManager::get_singleton().calculate_h(curr_position, destination));

    if(distance >= 8)
        // If he is farther than 8 tiles away, his targeting is identical to Blinky’s:
        return destination;
    else
    {
        // Soon as his distance to Pacman becomes less than 8 tiles, Clyde’s target is
        // some another tile:
        srand(time(NULL));
        return {random_x[rand() % 10], random_y[rand() % 10]};
    }
}

