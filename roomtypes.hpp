#ifndef ROOMTYPES_HPP
#define ROOMTYPES_HPP

#include <memory>
#include <QObject>
#include <QQuickItem>

// Struct to hold properties of wall elements
// (visually represented by 'WallBlock' QML components):
struct WallBlock
{
    unsigned int x_pos;
    unsigned int y_pos;
    std::unique_ptr<QObject> parent; // (C++ owner of the QML item)
    QQuickItem* quickitem; // (ptr to visual representation)

    WallBlock(unsigned int use_x, unsigned int use_y) :
        x_pos(use_x),
        y_pos(use_y),
        parent(std::make_unique<QObject>(nullptr)),
        quickitem(nullptr)
    {}
};

// Struct to hold properties of power pellet elements
// (visually represented by 'Pellet' QML components):
struct Pellet
{
    bool active; // (true -- not eated yet, false -- already eaten)
    unsigned int x_pos;
    unsigned int y_pos;
    std::unique_ptr<QObject> parent; // (C++ owner of the QML item)
    QQuickItem* quickitem; // (ptr to visual representation)

    Pellet(unsigned int use_x, unsigned int use_y) :
        active(true),
        x_pos(use_x),
        y_pos(use_y),
        parent(std::make_unique<QObject>(nullptr)),
        quickitem(nullptr)
    {}
};

// Struct to hold properties of energizer elements
// (visually represented by 'Energizer' QML components):
struct Energizer
{
    bool active; // (true -- not eated yet, false -- already eaten)
    unsigned int x_pos;
    unsigned int y_pos;
    std::unique_ptr<QObject> parent; // (C++ owner of the QML item)
    QQuickItem* quickitem; // (ptr to visual representation)

    Energizer(unsigned int use_x, unsigned int use_y) :
        active(true),
        x_pos(use_x),
        y_pos(use_y),
        parent(std::make_unique<QObject>(nullptr)),
        quickitem(nullptr)
    {}
};

#endif // ROOMTYPES_HPP
