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

Inky::Inky(): Ghost(9, 7, "qrc:/QMLs/QMLs/Blinky.qml", 0)
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

void Inky::move()
{
    #ifdef INKY_DEBUG
    qDebug() << "'Inky::move'";
    #endif
}
