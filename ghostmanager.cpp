#include <QDebug>
#include "ghostmanager.hpp"
#include "globalconfig.hpp"
#include "blinky.hpp"
#include "pinky.hpp"
#include "inky.hpp"
#include "clyde.hpp"

// #define GHOSTMAN_DEBUG
// #define TIMER_DEBUG

using std::shared_ptr;

GhostManager::GhostManager()
{
    #ifdef GHOSTMAN_DEBUG
    qDebug() << "'GhostManager' constructor";
    #endif

    // Setting periods of 'Scatter' and 'Chase' modes // (in ms)
    schedule_first_level = {7000  /*'Scatter'*/,
                            20000 /*'Chase'*/,
                            7000  /*'Scatter'*/,
                            20000 /*'Chase'*/,
                            5000  /*'Scatter'*/,
                            20000 /*'Chase'*/,
                            5000  /*'Scatter'*/
                            /*permanent 'Chase' mode*/};

    schedule_second_level = {7000  /*'Scatter'*/,
                             20000 /*'Chase'*/,
                             7000  /*'Scatter'*/,
                             20000 /*'Chase'*/,
                             5000  /*'Scatter'*/,
                             1033000 /*'Chase'*/,
                             16 /*'Scatter'*/
                             /*permanent 'Chase' mode*/}; // (in ms)

    // Instantiating ghosts:
    Ghost* blinky_ptr = &(Blinky::get_singleton());
    ghosts.push_back(blinky_ptr);
    Ghost* pinky_ptr = &(Pinky::get_singleton());
    ghosts.push_back(pinky_ptr);
    Ghost* inky_ptr = &(Inky::get_singleton());
    ghosts.push_back(inky_ptr);
    Ghost* clyde_ptr = &(Clyde::get_singleton());
    ghosts.push_back(clyde_ptr);

    // Setting the timer ready:
    connect(&wave_timer, SIGNAL(timeout()), this, SLOT(update_timer()));

    // Subscribing for events from all the ghosts:
    for(Ghost* ghost: ghosts)
    {
        connect(ghost, SIGNAL(ate_ghost()), this, SLOT(update_score())); // (if Pacman ate a ghost)
        connect(ghost, SIGNAL(lost_life()), this, SLOT(restart())); // (if ghost caught Pacman)
    }
}

GhostManager::~GhostManager()
{
    #ifdef GHOSTMAN_DEBUG
    qDebug() << "'GhostManager' destructor";
    #endif
}

GhostManager& GhostManager::get_singleton(shared_ptr<QQmlApplicationEngine> use_engine)
{
    #ifdef GHOSTMAN_DEBUG
    qDebug() << "'GhostManager::get_singleton'";
    #endif

    static GhostManager ghost_man;

    if(use_engine != nullptr)
        ghost_man.set_engine(use_engine);

    return ghost_man;
}

void GhostManager::set_engine(shared_ptr<QQmlApplicationEngine>& use_engine)
{
    engine = use_engine;

    #ifdef GHOSTMAN_DEBUG
    qDebug() << "'GhostManager::set_engine'. Obtained" << engine.get();
    #endif

    // Provide the engine to ghosts, so they can render themselves:
    for(Ghost* ghost: ghosts)
        ghost->set_engine(engine);
}

void GhostManager::set_state(unsigned int requested_level)
{
    switch(requested_level)
    {
        case 1:
        {
            // If level is not started from the beginning, i.e.
            // this level is re-started after Pacman lost his life:
            if(wave_timer.isActive())
                wave_timer.stop();

            curr_schedule = schedule_first_level;
            schedule_index = 0;
            wave_timer.setInterval(curr_schedule[schedule_index]);
            scatter_mode_on = true;
            chase_mode_on = false;
            frightened_mode_on = false;

            wave_timer.start();

            for(Ghost* ghost: ghosts)
            {
                ghost->set_state_first_level();
                // Ghosts are rendered and start to move in accordance with the 'wave_timer':
                ghost->show();
            }

            remaining_time = 0;
        }
        break;

        case 2:
        {
            // If level is not started from the beginning, i.e.
            // this level is re-started after Pacman lost his life:
            if(wave_timer.isActive())
                wave_timer.stop();

            curr_schedule = schedule_second_level;
            schedule_index = 0;
            wave_timer.setInterval(curr_schedule[schedule_index]);
            scatter_mode_on = true;
            chase_mode_on = false;
            frightened_mode_on = false;

            wave_timer.start();

            for(Ghost* ghost: ghosts)
            {
                ghost->delete_trajectory();
                ghost->set_state_second_level();
                // Ghosts are rendered and start to move in accordance with the 'wave_timer':
                ghost->show();
            }

            remaining_time = 0;
        }
        break;
    }
}

void GhostManager::flush_state()
{
    wave_timer.stop();
    remaining_time = 0;

    for(Ghost* ghost: ghosts)
        ghost->hide();
}

void GhostManager::set_all_scattering()
{
    for(Ghost* ghost: ghosts)
        ghost->set_scattering();
}

void GhostManager::set_all_chasing()
{
    for(Ghost* ghost: ghosts)
        ghost->set_chasing();
}

void GhostManager::set_all_frightened(unsigned int period)
{
    frightened_mode_on = true;
    remaining_time = wave_timer.remainingTime();
    wave_timer.stop();

    QTimer::singleShot(period, this, SLOT(update_timer()));

    #ifdef TIMER_DEBUG
    qDebug() << "____WAVE_TIMER__________________________";
    qDebug() << "Frighthtened for " << period / 1000 << " secs";
    qDebug() << "Schedule index to come back to: " << schedule_index;
    qDebug() << "________________________________________";
    #endif

    for(Ghost* ghost: ghosts)
        ghost->set_frightened();
}

void GhostManager::update_timer()
{
    // If the 'Frightened' mode is over, and the main timer must be resumed:
    if(frightened_mode_on == true)
    {
        frightened_mode_on = false;
        wave_timer.setInterval(remaining_time);

        if(chase_mode_on == true)
            set_all_chasing();
        else if(scatter_mode_on == true)
            set_all_scattering();

        wave_timer.start();

        #ifdef TIMER_DEBUG
        qDebug() << "____WAVE_TIMER__________________________";
        qDebug() << "Resumed after 'Frightened' mode. Remaining time: " << remaining_time / 1000;
        qDebug() << "Schedule index: " << schedule_index;
        qDebug() << "________________________________________";
        #endif
        return;
    }

    // If remaining time (that left after the timer was resumed) is out,
    // set the next interval and mode:
    if(remaining_time == wave_timer.interval())
    {
        if(schedule_index != 6) // (if there are modes left in the schedule)
        {
            schedule_index++;
            wave_timer.start(curr_schedule[schedule_index]);

            if(scatter_mode_on == true)
            {
                scatter_mode_on = false;
                chase_mode_on = true;
                set_all_chasing();

                #ifdef TIMER_DEBUG
                qDebug() << "____WAVE_TIMER__________________________";
                qDebug() << "New interval: " << wave_timer.interval();
                qDebug() << "Schedule index: " << schedule_index;
                qDebug() << "Entered the 'Chase' mode";
                qDebug() << "________________________________________";
                #endif
            }
            else if(chase_mode_on == true)
            {
                chase_mode_on = false;
                scatter_mode_on = true;
                set_all_scattering();

                #ifdef TIMER_DEBUG
                qDebug() << "____WAVE_TIMER__________________________";
                qDebug() << "New interval: " << wave_timer.interval();
                qDebug() << "Schedule index: " << schedule_index;
                qDebug() << "Entered the 'Scatter' mode";
                qDebug() << "________________________________________";
                #endif
            }
            return;
        }

        if(schedule_index == 6) // (if in permanent 'Chase' mode)
        {
            wave_timer.start(20000);
            chase_mode_on = true;

            #ifdef TIMER_DEBUG
            qDebug() << "____WAVE_TIMER__________________________";
            qDebug() << "Returning to permanent 'Chase' mode after 'Frightened' mode";
            qDebug() << "________________________________________";
            #endif
        }
    }

    // If ghosts are already in the permanent 'Chase' mode:
    if(schedule_index == 6 && chase_mode_on == true)
    {
        #ifdef TIMER_DEBUG
        qDebug() << "____WAVE_TIMER__________________________";
        qDebug() << "Already in the permanent 'Chase' mode";
        qDebug() << "Schedule index: " << schedule_index;
        qDebug() << "________________________________________";
        #endif
        return;
    }

    // If all the ghosts are currently in the last 'Scatter' mode
    // before the permanent 'Chase' mode:
    if(schedule_index == 6)
    {
        scatter_mode_on = false;
        chase_mode_on = true;
        set_all_chasing();

        #ifdef TIMER_DEBUG
        qDebug() << "____WAVE_TIMER__________________________";
        qDebug() << "Entered the permanent 'Chase' mode";
        qDebug() << "Schedule index: " << schedule_index;
        qDebug() << "________________________________________";
        #endif
        return;
    }

    // If no timer interruptions were made and there are still modes
    // to go through in a schedule:
    if(scatter_mode_on == true)
    {
        schedule_index++;
        wave_timer.start(curr_schedule[schedule_index]);
        scatter_mode_on = false;
        chase_mode_on = true;

        set_all_chasing();

        #ifdef TIMER_DEBUG
        qDebug() << "____WAVE_TIMER__________________________";
        qDebug() << "New interval: " << wave_timer.interval();
        qDebug() << "Schedule index: " << schedule_index;
        qDebug() << "Entered the 'Chase' mode";
        qDebug() << "________________________________________";
        #endif

        return;
    }
    else if(chase_mode_on == true)
    {
        schedule_index++;
        wave_timer.start(curr_schedule[schedule_index]);
        chase_mode_on = false;
        scatter_mode_on = true;

        set_all_scattering();

        #ifdef TIMER_DEBUG
        qDebug() << "____WAVE_TIMER__________________________";
        qDebug() << "New interval: " << wave_timer.interval();
        qDebug() << "Schedule index: " << schedule_index;
        qDebug() << "Entered the 'Scatter' mode";
        qDebug() << "________________________________________";
        #endif

        return;
    }
}

void GhostManager::update_score()
{
    // TO DO (improvement): add 200 for every new ghost eaten
    // within 1 'Frightened' mode

    emit ate_ghost(200);
}

void GhostManager::restart()
{
    emit life_lost();
}

void GhostManager::detect_collision_ghosts(unsigned int pacman_x, unsigned int pacman_y)
{
    for(Ghost* ghost: ghosts)
    {
        if(ghost->get_x() == pacman_x && ghost->get_y() == pacman_y)
        {
            update_score();
            ghost->go_back_to_house();
        }
    }
}



















