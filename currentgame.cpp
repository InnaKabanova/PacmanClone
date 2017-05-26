#include <QDebug>
#include <QQuickItem>
#include <QQuickWindow>
#include <QQmlContext>

#include "currentgame.hpp"
#include "globalconfig.hpp"
#include "pacman.hpp"
#include "ghostmanager.hpp"
#include "roommanager.hpp"

// #define CURRGAME_DEBUG
// #define SCORE_DEBUG
// #define LEVEL_DEBUG

CurrentGame& CurrentGame::get_singleton()
{
    #ifdef CURRGAME_DEBUG
    qDebug() << "'CurrentGame::get_singleton'";
    #endif

    static CurrentGame curr_game;
    return curr_game;
}

CurrentGame::CurrentGame() :
    tile_size(constants::TILE_SIZE),
    current_level(0),
    score(0),
    health(3),
    engine(std::make_shared<QQmlApplicationEngine>(nullptr))
{
    // Exposing this object to QML
    QQmlContext* context = engine->rootContext();
    context->setContextProperty("current_game", this);
    // Loading the GUI:
    engine->load(QUrl(QStringLiteral("qrc:/QMLs/QMLs/main.qml")));

    // Instantiating static game objects, exposing them to QML:
    context->setContextProperty("pacman_object", &(Pacman::get_singleton()));
    context->setContextProperty("ghostman_object", &(GhostManager::get_singleton()));
    context->setContextProperty("roomman_object", &(RoomManager::get_singleton()));

    // Providing the newly created objects with the currently used
    // QML engine:
    RoomManager::get_singleton(engine);
    Pacman::get_singleton(engine);
    GhostManager::get_singleton(engine);

    // Connecting some signals and slots:

    // 1. Since the 'RoomManager' object keeps track of pellets & energizers eaten,
    // this object updates the score:
    connect(&(RoomManager::get_singleton()), SIGNAL(ate(unsigned int)), this,
            SLOT(add_score(unsigned int)));

    // 2. Also, if all the pellets are eaten (and Pacman is still alive),
    // 'CurrentGame' is informed to start a new level:
    connect(&(Pacman::get_singleton()), SIGNAL(ate_all()), this,
            SLOT(start_level()));

    // 3. 'Pacman' object has to inform all the ghosts when he enters the 'Energized'
    // mode for them to enter the 'Frightened' mode
    // (duration of the stay is specified as a parameter)
    connect(&(Pacman::get_singleton()), SIGNAL(energized(unsigned int)), &(GhostManager::get_singleton()),
            SLOT(set_all_frightened(unsigned int)));

    // 4. If Pacman ate one of the ghosts:
    connect(&(GhostManager::get_singleton()), SIGNAL(ate_ghost(unsigned int)), this, SLOT(add_score(unsigned int)));

    // 5. If one of the ghosts caught Pacman:
    connect(&(GhostManager::get_singleton()), SIGNAL(life_lost()), this, SLOT(try_again()));

    #ifdef CURRGAME_DEBUG
    qDebug() << "'CurrentGame' constructor";
    #endif
}

CurrentGame::~CurrentGame()
{
    #ifdef CURRGAME_DEBUG
    qDebug() << "'CurrentGame' destructor";
    #endif
}

unsigned int CurrentGame::get_tilesize()
{
    return tile_size;
}

unsigned int CurrentGame::get_health()
{
    return health;
}

unsigned int CurrentGame::get_score()
{
    return score;
}

unsigned int CurrentGame::get_level()
{
    return current_level;
}

void CurrentGame::start_level()
{
    current_level++;
    emit updateLevel(current_level);

    // If player has passed all the game levels:
    if(current_level > levels_num)
    {
        #ifdef LEVEL_DEBUG
        qDebug() << "Game ended with victory! ";
        #endif

        // Return all the game objects to their basic (1st level) states:
        flush_state();
        // Signal to QML to make the 'GameOverScreen' item visible:
        emit passed(score, true);
        return;
    }

    #ifdef LEVEL_DEBUG
    qDebug() << "New level: " << current_level;
    #endif

    // Otherwise, set the states of the game objects in accordance with
    // the new level:   
    Pacman::get_singleton().set_state(current_level);
    RoomManager::get_singleton().set_state(current_level);
    GhostManager::get_singleton().set_state(current_level);

}

void CurrentGame::try_again()
{
    health--;
    emit updateHealth(health);

    if(health == 0)
    {
        #ifdef LEVEL_DEBUG
        qDebug() << "Game over! ";
        #endif

        int score_backup = score;
        flush_state();
        emit passed(score_backup, false);
        return;
    }
    else
    {
        #ifdef LEVEL_DEBUG
        qDebug() << "New try! ";
        #endif

        // Return ghosts to their initial positions:
        GhostManager::get_singleton().set_state(current_level);
        // Return Pacman to its initial position:
        Pacman::get_singleton().set_state(current_level);
    }

    #ifdef SCORE_DEBUG
    qDebug() << "Health: " << health;
    #endif
}

void CurrentGame::flush_state()
{
    current_level = 0;
    score = 0;
    health = 3;

    emit updateLevel(current_level);
    emit updateScore(score);
    emit updateHealth(health);

    Pacman::get_singleton().flush_state();
    RoomManager::get_singleton().flush_state();
    GhostManager::get_singleton().flush_state();
}

void CurrentGame::add_score(unsigned int gained)
{
    score += gained;
    emit updateScore(score);

    #ifdef SCORE_DEBUG
    qDebug() << "Score: " << score;
    #endif
}


