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

Clyde::Clyde(): Ghost(9, 7, "qrc:/QMLs/QMLs/Blinky.qml", 0)
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

void Clyde::move()
{
    #ifdef CLYDE_DEBUG
    qDebug() << "'Clyde::move'";
    #endif
}
