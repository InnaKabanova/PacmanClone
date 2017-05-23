#include "pacman.hpp"
#include "direction.hpp"
#include "globalconfig.hpp"
#include "roommanager.hpp"
#include <QDebug>
#include <QQuickWindow>
#include <QQmlComponent>

// #define PACMAN_DEBUG
// #define PACMAN_STATES

using std::shared_ptr;
using std::make_unique;

Pacman::Pacman() :
    engine(nullptr),
    quickitem(nullptr),
    parent(make_unique<QObject>(nullptr)),
    move_allowed(true)
{
    #ifdef PACMAN_DEBUG
    qDebug() << "'Pacman' constructor";
    #endif
}

Pacman& Pacman::get_singleton(shared_ptr<QQmlApplicationEngine> use_engine)
{
    #ifdef PACMAN_DEBUG
    qDebug() << "'Pacman::get_singleton'";
    #endif

    static Pacman pacman;

    if(use_engine != nullptr)
    {
        pacman.set_engine(use_engine);

        // Dynamically creating a visual (QML) representation of this object.
        // It is not visible still. It will become visible when 'Pacman::set_state' is called, and
        // invisible when 'Pacman::flush_state' is called.
        QQuickWindow* window = qobject_cast<QQuickWindow*>(pacman.engine->rootObjects().at(0));
        QQuickItem* root = window->contentItem();
        QQmlComponent pacman_comp(pacman.engine.get(), QUrl("qrc:/QMLs/QMLs/Pacman.qml"));
        pacman.quickitem = qobject_cast<QQuickItem*>(pacman_comp.create());
        QQmlApplicationEngine::setObjectOwnership(pacman.quickitem, QQmlApplicationEngine::CppOwnership);
        pacman.quickitem->setParentItem(root);
        pacman.quickitem->setParent(pacman.parent.get());

        #ifdef PACMAN_DEBUG
        qDebug() << "'Pacman::get_singleton'. Obtained" << pacman.engine.get();
        #endif
    }
    else
    {
        #ifdef PACMAN_DEBUG
        qDebug() << "'Pacman::get_singleton'. No QQmlApplicationEngine passed.";
        #endif
    }

    return pacman;
}

Pacman::~Pacman()
{
    #ifdef PACMAN_DEBUG
    qDebug() << "'Pacman' destructor";
    #endif
}

void Pacman::set_engine(shared_ptr<QQmlApplicationEngine>& use_engine)
{
    engine = use_engine;
}

void Pacman::set_state(unsigned int requested_level)
{
    switch(requested_level)
    {
        #ifdef PACMAN_STATES
        qDebug() << "'Pacman::set_state':" << requested_level << "level";
        #endif

        case 1:
        {
            x = init_x;
            y = init_y;
            direction = PacmanDirection::PAC_UP;

            // Max. speed: 11 tiles per second
            speed_limit = 91; // msec (1/11 sec)
            energized_time = 10000;

            redraw();
            show();
        }

        break;
        case 2:
        {
            x = init_x;
            y = init_y;
            direction = PacmanDirection::PAC_UP;

            speed_limit = 97; // msec (1/11 sec)
            energized_time = 8000;

            redraw();
        }
        break;
    }
}

void Pacman::flush_state()
{
    hide();
}

void Pacman::redraw()
{
    #ifdef PACMAN_STATES
    qDebug() << "'Pacman::redraw' x:" << x << "y:" << y;
    #endif

    quickitem->setProperty("x", x * constants::TILE_SIZE);
    quickitem->setProperty("y", y * constants::TILE_SIZE);
    quickitem->setProperty("direction", direction);
}

void Pacman::show()
{
    quickitem->setProperty("visible", true);
}

void Pacman::hide()
{
    quickitem->setProperty("visible", false);
}

void Pacman::move(unsigned int target_direction)
{
    if(move_allowed == false)
        return;

    // Calculating target x & y:
    unsigned int target_x;
    unsigned int target_y;
    if(target_direction == PacmanDirection::PAC_UP)
    {
        target_x = x;
        target_y = y - 1;
    }
    else if(target_direction == PacmanDirection::PAC_LEFT)
    {
        target_x = x - 1;
        target_y = y;
    }
    else if(target_direction == PacmanDirection::PAC_RIGHT)
    {
        target_x = x + 1;
        target_y = y;
    }
    else if(target_direction == PacmanDirection::PAC_DOWN)
    {
        target_x = x;
        target_y = y + 1;
    }

    unsigned int result = RoomManager::get_singleton().check_tile(x, y, direction, target_x, target_y);
    // If movement was denied:
    if(result == 1)
        return;
    // If Pacman's coordinates were changed by 'RoomManager::check_tile':
    else if(result == 3)
        return;
    // If movement was permitted:
    else if(result == 0 || result == 2 || result == 5)
    {
        x = target_x;
        y = target_y;
        direction = target_direction;
        redraw();
        move_allowed = false;
        QTimer::singleShot(speed_limit, this, SLOT(make_available_again()));
        if(result == 2)
            emit energized(energized_time);
        if(result == 5)
            emit ate_all();
    }
}

void Pacman::make_available_again()
{
    move_allowed = true;
}

unsigned int Pacman::get_x()
{
    return x;
}

unsigned int Pacman::get_y()
{
    return y;
}














































