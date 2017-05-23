#ifndef PACMAN_H
#define PACMAN_H

#include <memory>
#include <QObject>
#include <QQuickItem>
#include <QQmlApplicationEngine>
#include <QTimer>

class Pacman : public QObject
{  
    Q_OBJECT
public:
    static Pacman& get_singleton(std::shared_ptr<QQmlApplicationEngine> use_engine = nullptr);
    void set_state(unsigned int requested_level);
    void flush_state();

public slots:
    // Functions called from QML:
    void move(unsigned int target_direction);
    unsigned int get_x();
    unsigned int get_y();

    // Function which allows Pacman to make a new movement
    // (since the possible number of 'void move(uint)' calls per second is
    // limited by 'QTimer::singleShot()'):
    void make_available_again();

signals:
    void energized(unsigned int period); // (makes all the ghosts 'frightened')
    void ate_all(); // (proceeds Pacman to the next level if all pellets & energizers are eaten)

private:
    // Initial Pacman's position at the beginning of each level:
    const unsigned int init_x = 9;
    const unsigned int init_y = 15;

    std::shared_ptr<QQmlApplicationEngine> engine;
    QQuickItem* quickitem; // (pointer to Pacman's visual representation)
    std::unique_ptr<QObject> parent; // (C++ owner of quickitem)

    // Pacman's properties which change during the game process:
    bool move_allowed;
    unsigned int x;
    unsigned int y;
    int direction;

    // Pacman's properties which are specific for each level:
    unsigned int speed_limit; // (msec) 'void move(uint)' can be called once in this period
    unsigned int energized_time; // (msec)

    // Helper functions:
    void set_engine(std::shared_ptr<QQmlApplicationEngine>& use_engine);
    void redraw(); // (re-renders Pacman with the new properties)
    void show(); // (makes Pacman visible)
    void hide(); // (makes Pacman invisible)

    Pacman();
    ~Pacman();
    Pacman(const Pacman&) = delete;
    Pacman& operator= (const Pacman&) = delete;
};

#endif // PACMAN_H
