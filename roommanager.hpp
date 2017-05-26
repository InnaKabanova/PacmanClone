#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H

#include "roomtypes.hpp"
#include "position.hpp"
#include <vector>
#include <stack>
#include <QQmlApplicationEngine>

class RoomManager: public QObject
{
    Q_OBJECT
public:
    static RoomManager& get_singleton(std::shared_ptr<QQmlApplicationEngine> use_engine = nullptr);

    // Function used to get the room ready for the requested game level:
    void set_state(unsigned int requested_level);
    // Function used to return this object to its basic (1st level) state:
    void flush_state();

    // Function that helps Pacman to proceed through the maze and
    // keeps track of the eating progress
    // (updates the score & sends Pacman to the next level):
    unsigned int check_tile(unsigned int& curr_x, unsigned int& curr_y, int& direction,
                            unsigned int target_x, unsigned int target_y);

    // Function that provides ghosts with movement trajectories,
    // depending on the source tile, destination tile and maze objects:
    void get_path(std::stack<Position>& trajectory, Position& source, Position& destination);
    bool is_valid(const Position& position);
    double calculate_h(const Position& position, const Position& destination);

signals:
    void ate(unsigned int points); // (updates the score if Pacman ate a pellet or energizer)

private:
    std::shared_ptr<QQmlApplicationEngine> engine;
    unsigned int total_pellets_num;
    unsigned int eaten_pellets_num;
    std::vector<std::vector<int>> maze;

    // Gaming data (room elements) to handle:
    std::vector<std::unique_ptr<WallBlock>> wallblocks;
    std::vector<std::unique_ptr<Pellet>> pellets;
    std::vector<std::unique_ptr<Energizer>> energizers;

    // Helper functions:
    void set_engine(std::shared_ptr<QQmlApplicationEngine>& use_engine);
    void create(); // (instantiates the room objects when engine is provided)
    void show_maze(); // (renders the room)
    void hide_maze(); // (makes the room invisible)

    RoomManager();
    ~RoomManager();
    RoomManager(const RoomManager&) = delete;
    RoomManager& operator= (const RoomManager&) = delete;

    // Points for eating:
    const unsigned int pellet_points = 10;
    const unsigned int energizer_points = 50;

    // Data & functions used by 'get_path' function:
    bool is_destination(const Position &position, const Position& destination);
    std::vector<Position> find_valid_adjacent(const Position* position);
    void trace_back_path(std::stack<Position>& trajectory, Position& from_destination);

    unsigned int maze_width;
    unsigned int maze_height;

};

#endif // ROOMMANAGER_H

