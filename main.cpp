#include <QGuiApplication>
#include "currentgame.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    CurrentGame::get_singleton();
    return app.exec();
}
