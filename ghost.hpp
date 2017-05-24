#ifndef GHOST_H
#define GHOST_H

#include <QObject>
#include <QQuickItem>
#include <QTimer>
#include <QQmlApplicationEngine>
#include <memory>
#include "direction.hpp"
#include "roommanager.hpp"
#include "pacman.hpp"

enum Mode{SCATTER, CHASE, FRIGHTENED};

class Ghost: public QObject
{
    Q_OBJECT
public:
    Ghost(unsigned int use_init_x,
          unsigned int use_init_y,
          const char* use_qml_path,
          unsigned int use_time_to_leave,
          unsigned int use_s_x_1,
          unsigned int use_s_y_1,
          unsigned int use_s_x_2,
          unsigned int use_s_y_2);
    virtual ~Ghost();

    // Functions used by 'GhostManager':
    void set_engine(std::shared_ptr<QQmlApplicationEngine>& use_engine);
    void set_frightened();
    void set_scattering();
    void set_chasing();

    void delete_trajectory();
    void show();
    void hide();

    virtual void set_state_first_level(); // (!! defined in the derived class)
    virtual void set_state_second_level(); // (!! defined in the derived class)

public slots:
    // 'Main' function of ghost's movement:
    void move();
    void leave_the_house();

    // Functions that render Ghost with new position:
    void redraw();
    void redraw(unsigned int use_x, unsigned int use_y, unsigned int use_direction, bool with_scaling);

signals:
    void lost_life(); // (if ghost caught Pacman)
    void ate_ghost(); // (if Pacman ate ghost)

protected:
    std::shared_ptr<QQmlApplicationEngine> engine;
    QTimer move_timer;

    unsigned int init_x;
    unsigned int init_y;
    const char* qml_path;
    unsigned int time_to_leave; // (determines when the ghost leaves his house at the beginning)
    unsigned int speed; // (number of position updates per second)

    QQuickItem* quickitem; // (ptr to visual representation)
    std::unique_ptr<QObject> parent; // (C++ owner of the QML item)

    unsigned int x;
    unsigned int y;
    unsigned int direction;

    std::stack<Position> curr_trajectory; // (current movement trajectory)
    unsigned int curr_mode;

    // Helper functions used by 'move()' to compose ghost's movement:
    void make_step();
    bool detect_collision_with_pacman();
    void determine_direction(unsigned int new_x, unsigned int new_y);

    // Functions that provide with target tiles for every mode
    // (can be redefined in the derived classes in accordance with
    // every concrete ghost's personality)
    virtual Position get_target_chasing(); // (redefined)
    virtual Position get_target_scattering();
    virtual Position get_target_frightened();

    // Debug function:
    virtual const char* get_name(); // (redefined)

    unsigned int scatter_x_1;
    unsigned int scatter_y_1;
    unsigned int scatter_x_2;
    unsigned int scatter_y_2;




};

#endif // GHOST_H
