#include "ghost.hpp"
#include "globalconfig.hpp"
#include <QDebug>
#include <QQuickWindow>
#include <QQmlComponent>

//#define GHOST_DEBUG
//#define MOVEMENT_DEBUG
#define TARGET_DEBUG

using std::shared_ptr;
using std::make_unique;
using std::stack;

Ghost::Ghost(unsigned int use_init_x, unsigned int use_init_y, const char* use_qml_path, 
             unsigned int use_time_to_leave,
             unsigned int use_s_x_1, unsigned int use_s_y_1,
             unsigned int use_s_x_2, unsigned int use_s_y_2):
             engine(nullptr),
             init_x(use_init_x),
             init_y(use_init_y),
             qml_path(use_qml_path),
             time_to_leave(use_time_to_leave),
             speed(0),
             quickitem(nullptr),
             parent(make_unique<QObject>(nullptr)),
             x(init_x),
             y(init_y),
             direction(GhostDirection::G_UP),
             curr_mode(Mode::SCATTER),
             scatter_x_1(use_s_x_1),
             scatter_y_1(use_s_y_1),
             scatter_x_2(use_s_x_2),
             scatter_y_2(use_s_y_2)
{
    connect(&move_timer, SIGNAL(timeout()), this, SLOT(move()));

    #ifdef GHOST_DEBUG
    qDebug() << "'Ghost' constructor";
    #endif
}

Ghost::~Ghost()
{
    #ifdef GHOST_DEBUG
    qDebug() << "'Ghost' constructor";
    #endif
}

void Ghost::set_engine(shared_ptr<QQmlApplicationEngine>& use_engine)
{
    engine = use_engine;

    // Dynamically creating a visual (QML) representation of this ghost:
    QQuickWindow* window = qobject_cast<QQuickWindow*>(engine->rootObjects().at(0));
    QQuickItem* root = window->contentItem();
    QQmlComponent ghost_comp(engine.get(), QUrl(qml_path));

    quickitem = qobject_cast<QQuickItem*>(ghost_comp.create());
    QQmlApplicationEngine::setObjectOwnership(quickitem, QQmlApplicationEngine::CppOwnership);
    quickitem->setParentItem(root);
    quickitem->setParent(parent.get());
}

void Ghost::set_state_first_level()
{
    // This method must be redefined in the derived class
    return;
}

void Ghost::set_state_second_level()
{
    // This method must be redefined in the derived class
    return;
}

const char* Ghost::get_name()
{
    // This method must be redefined in the derived class
    return "Ghost";
}

void Ghost::show()
{
    quickitem->setProperty("visible", true);
    move_timer.start();
}

void Ghost::hide()
{
    move_timer.stop();
    delete_trajectory();
    quickitem->setProperty("visible", false);
}

void Ghost::delete_trajectory()
{
    // Clear the trajectory queue:
    stack<Position> empty;
    swap(curr_trajectory, empty);

    #ifdef MOVEMENT_DEBUG
    qDebug() << "'GHOST::delete_trajectory'";
    #endif
}

void Ghost::set_frightened()
{
    curr_mode = Mode::FRIGHTENED;
    direction = GhostDirection::G_SCARED; // (makes the ghost blue)
}

void Ghost::set_scattering()
{
    curr_mode = Mode::SCATTER;
    direction = GhostDirection::G_DOWN; // (ghost returns to its 'natural' color)
}

void Ghost::set_chasing()
{
    curr_mode = Mode::CHASE;
    direction = GhostDirection::G_RIGHT; // (ghost returns to its 'natural' color)
}

// The main function where ghost movement starts:
void Ghost::move()
{
    #ifdef MOVEMENT_DEBUG
    qDebug() << "'GHOST::move'";
    #endif

    // If ghost is in the house...(the game or new try has just started)
    if((x == 8 && y == 9) ||
       (x == 9 && y == 9) ||
       (x == 10 && y == 9))
    {
        // ... let it wait 'time_to_leave' ms and go outside of the house:
        QTimer::singleShot(time_to_leave, this, SLOT(leave_the_house()));
        return;
    }

    // If ghost is out at the game field:
    delete_trajectory();
    Position source(x, y);

    if(curr_mode == Mode::CHASE)
    {
        Position destination = get_target_chasing();
        RoomManager::get_singleton().get_path(curr_trajectory, source, destination);

        #ifdef TARGET_DEBUG
        qDebug() << get_name() << ": Target in CHASE mode: " << destination.x << destination.y;
        #endif
    }
    else if(curr_mode == Mode::SCATTER)
    {
        // TO DO: make cyclic movement

        Position destination = get_target_scattering();
        RoomManager::get_singleton().get_path(curr_trajectory, source, destination);

        #ifdef TARGET_DEBUG
        qDebug() << get_name() << ": Target in SCATTER mode: " << destination.x << destination.y;
        #endif
    }
    else if(curr_mode == Mode::FRIGHTENED)
    {
        // TO DO: make pseudo-random movement

        #ifdef TARGET_DEBUG
        qDebug() << get_name() << ": Target in FRIGHTENED mode: ";
        #endif
    }

    // Make one move:
    make_step();

    // Check if the ghost collided Pacman:
    bool collided = detect_collision_with_pacman();
    if(collided == true)
        return;    
}

Position Ghost::get_target_chasing()
{
    // This method must be redefined in the derived class
    return {0,0};
}

Position Ghost::get_target_scattering()
{
    // TO DO
    return {0,0};
}

Position Ghost::get_target_frightened()
{
    // TO DO
    return {0,0};
}

void Ghost::make_step()
{
    #ifdef MOVEMENT_DEBUG
    qDebug() << "'GHOST::make_step'";
    #endif

    if(curr_trajectory.size() == 0)
        return;

    // Determine the final destination of a step:
    unsigned int new_x = curr_trajectory.top().x;
    unsigned int new_y = curr_trajectory.top().y;

    // Determine the movement direction:
    determine_direction(new_x, new_y);

    // Determine the intermediate position
    // (for smoother movement):
    unsigned int step = constants::TILE_SIZE/2;
    if(direction == GhostDirection::G_UP)
        redraw(new_x * constants::TILE_SIZE, new_y * constants::TILE_SIZE + step, direction, false);
    else if(direction == GhostDirection::G_DOWN)
        redraw(new_x * constants::TILE_SIZE, new_y * constants::TILE_SIZE - step, direction, false);
    else if(direction == GhostDirection::G_LEFT)
        redraw(new_x * constants::TILE_SIZE + step, new_y * constants::TILE_SIZE, direction, false);
    else if(direction == GhostDirection::G_RIGHT)
        redraw(new_x * constants::TILE_SIZE - step, new_y * constants::TILE_SIZE, direction, false);

    // Re-draw the final position:
    x = new_x;
    y = new_y;
    curr_trajectory.pop();
    QTimer::singleShot(90, this, SLOT(redraw()));
}

void Ghost::redraw()
{
    #ifdef MOVEMENT_DEBUG
    qDebug() << "'GHOST::redraw': according to values of the x,y members";
    #endif

    quickitem->setProperty("x", x * constants::TILE_SIZE);
    quickitem->setProperty("y", y * constants::TILE_SIZE);
    quickitem->setProperty("direction", direction);
}

void Ghost::redraw(unsigned int use_x, unsigned int use_y, unsigned int use_direction, bool with_scaling)
{
    #ifdef MOVEMENT_DEBUG
    qDebug() << "'GHOST::redraw': according to values specified as parameters";
    #endif

    if(with_scaling == true)
    {
        quickitem->setProperty("x", use_x * constants::TILE_SIZE);
        quickitem->setProperty("y", use_y * constants::TILE_SIZE);
        quickitem->setProperty("direction", use_direction);
    }
    if(with_scaling == false)
    {
        quickitem->setProperty("x", use_x);
        quickitem->setProperty("y", use_y);
        quickitem->setProperty("direction", use_direction);
    }
}

void Ghost::determine_direction(unsigned int new_x, unsigned int new_y)
{
    #ifdef MOVEMENT_DEBUG
    qDebug() << "'GHOST::determine_direction'";
    #endif

    if(x == new_x && new_y < y)
        direction = (int)GhostDirection::G_UP;
    else if(x == new_x && new_y > y)
        direction = (int)GhostDirection::G_DOWN;
    else if(new_x < x && new_y == y)
        direction = (int)GhostDirection::G_LEFT;
    else if(new_x > x && new_y == y)
        direction = (int)GhostDirection::G_RIGHT;
}

void Ghost::leave_the_house()
{
    y = 7;
}

bool Ghost::detect_collision_with_pacman()
{
    #ifdef MOVEMENT_DEBUG
    qDebug() << "'GHOST::detect_collision_with_pacman'";
    #endif

    if(x == Pacman::get_singleton().get_x() && y == Pacman::get_singleton().get_y())
    {
        if(curr_mode == Mode::SCATTER || curr_mode == Mode::CHASE)
        {
            emit lost_life();
            return true;
        }
        else if(curr_mode == Mode::FRIGHTENED)
        {
            emit ate_ghost();
            // TO DO: go back to house
            return true;
        }
    }
    else
        return false;
}

























