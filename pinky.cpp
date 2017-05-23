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

Pinky::Pinky(): Ghost(9, 7, "qrc:/QMLs/QMLs/Blinky.qml", 0)
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

void Pinky::move()
{
    #ifdef PINKY_DEBUG
    qDebug() << "'Pinky::move'";
    #endif
}
