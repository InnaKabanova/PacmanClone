#ifndef CURRENTGAME_H
#define CURRENTGAME_H

#include <memory>
#include <QObject>
#include <QQmlApplicationEngine>

class CurrentGame : public QObject
{
    Q_OBJECT
public:
    static CurrentGame& get_singleton();

public slots:
    // Functions called from QML:
    // 1. (for 'Tile.qml' singleton to be initialized)
    unsigned int get_tilesize();
    // 2. (when 'play game' button is clicked)
    void start_level();
    // 3. (for 'InfoBar.qml' texts update)
    unsigned int get_health();
    unsigned int get_score();
    unsigned int get_level();

    // Function that increments the score during the game:
    void add_score(unsigned int gained);
    // Function which decides whether to continue the game
    // when Pacman loses 1 life:
    void try_again();

signals:
    // Signal to QML that user has passed all the levels or
    // the game is over:
    void passed(int score, bool victory = false);

    // Signals to QML for UI info update:
    void updateScore(unsigned int new_score);
    void updateHealth(unsigned int new_health);
    void updateLevel(unsigned int new_level);

private:
    // Function that returns all the game objects into
    // their basic (1st level) states:
    void flush_state();

    const int levels_num = 2;
    const int max_health = 3;

    unsigned int tile_size;
    unsigned int current_level;
    unsigned int score;
    int health;
    std::shared_ptr<QQmlApplicationEngine> engine;

    CurrentGame();
    ~CurrentGame();
    CurrentGame(const CurrentGame&) = delete;
    CurrentGame& operator= (const CurrentGame&) = delete;
};

#endif // CURRENTGAME_H
