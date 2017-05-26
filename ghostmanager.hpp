#ifndef GHOSTMANAGER_H
#define GHOSTMANAGER_H

#include "ghost.hpp"
#include <vector>
#include <QTimer>

class GhostManager: public QObject
{
    Q_OBJECT
public:
    static GhostManager& get_singleton(std::shared_ptr<QQmlApplicationEngine> use_engine = nullptr);
    void set_state(unsigned int requested_level);
    void flush_state();
    void detect_collision_ghosts(unsigned int pacman_x, unsigned int pacman_y);

public slots:
    // Functions that modify ghosts' modes (behavior) during the game:
    void set_all_frightened(unsigned int period);
    void set_all_scattering();
    void set_all_chasing();

    // Function which modifies ghosts' modes in accoradance with the
    // schedule:
    void update_timer();

    // Functions that propagate events from ghosts to 'CurrentGame':
    void update_score(); // (if Pacman ate a ghost)
    void restart(); // (if Pacman lost his life)

signals:
    void ate_ghost(unsigned int points);
    void life_lost();

private:
    std::shared_ptr<QQmlApplicationEngine> engine;
    std::vector<Ghost*> ghosts;

    // Helper function:
    void set_engine(std::shared_ptr<QQmlApplicationEngine>& use_engine);

    // Members used for scheduling:
    std::vector<int> schedule_first_level; // (predefined schedule for the 1st level)
    std::vector<int> schedule_second_level; // (predefined schedule for the 2nd level)
    std::vector<int> curr_schedule; // (schedule used on the current game level)

    bool chase_mode_on;
    bool scatter_mode_on;
    bool frightened_mode_on;

    QTimer wave_timer;

    int remaining_time; // (msec)
    int schedule_index; // (holds the current position in the modes schedule)

    GhostManager();
    ~GhostManager();
    GhostManager(const GhostManager&) = delete;
    GhostManager& operator= (const GhostManager&) = delete;
};

#endif // GHOSTMANAGER_H
