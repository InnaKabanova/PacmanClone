#include <climits>
#include <algorithm>
#include <QDebug>
#include <QQuickWindow>
#include "globalconfig.hpp"
#include "direction.hpp"
#include "roommanager.hpp"

// TO DO 1: set a prize
// TO DO 2: set a prize

//#define ROOMMAN_DEBUG
//#define PATHFINDER_DEBUG

using std::vector;
using std::stack;
using std::unique_ptr;
using std::shared_ptr;
using std::make_unique;
using std::make_shared;
using std::move;

RoomManager& RoomManager::get_singleton(shared_ptr<QQmlApplicationEngine> use_engine)
{
    #ifdef ROOMMAN_DEBUG
    qDebug() << "'RoomManager::get_singleton'";
    #endif

    static RoomManager room_man;
    if(use_engine != nullptr)
    {
        room_man.set_engine(use_engine);
        room_man.create();

        #ifdef ROOMMAN_DEBUG
        qDebug() << "'RoomManager::get_singleton'. Obtained" << room_man.engine.get();
        #endif
    }
    else
    {
        #ifdef ROOMMAN_DEBUG
        qDebug() << "'RoomManager::get_singleton'. No QQmlApplicationEngine passed.";
        #endif
    }

    return room_man;
}

RoomManager::RoomManager() :
    engine(nullptr),
    total_pellets_num(0),
    eaten_pellets_num(0)
{
    // 1 -- wall
    // 0 -- power pellet
    // 2 -- energizer
    // 3 -- nothing
    // 4 -- entry to the ghosts' house

    maze = {{{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
             {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
             {1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
             {1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1},
             {1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1},
             {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
             {1, 1, 1, 1, 0, 1, 1, 1, 3, 1, 3, 1, 1, 1, 0, 1, 1, 1, 1},
             {3, 3, 3, 1, 0, 1, 3, 3, 3, 3, 3, 3, 3, 1, 0, 1, 3, 3, 3},
             {1, 1, 1, 1, 0, 1, 3, 1, 1, 4, 1, 1, 3, 1, 0, 1, 1, 1, 1},
             {3, 3, 3, 3, 0, 3, 3, 1, 3, 3, 3, 1, 3, 3, 0, 3, 3, 3, 3},
             {1, 1, 1, 1, 0, 1, 3, 1, 1, 1, 1, 1, 3, 1, 0, 1, 1, 1, 1},
             {3, 3, 3, 1, 0, 1, 3, 3, 3, 3, 3, 3, 3, 1, 0, 1, 3, 3, 3},
             {1, 1, 1, 1, 0, 1, 3, 1, 1, 1, 1, 1, 3, 1, 0, 1, 1, 1, 1},
             {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
             {1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
             {1, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 1},
             {1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1},
             {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
             {1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
             {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
             {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}}};

    maze_width = maze[0].size();
    maze_height = maze.size();

    #ifdef ROOMMAN_DEBUG
    qDebug() << "'RoomManager' constructor";
    #endif
}

RoomManager::~RoomManager()
{
    #ifdef ROOMMAN_DEBUG
    qDebug() << "'RoomManager' destructor";
    #endif
}

void RoomManager::set_engine(shared_ptr<QQmlApplicationEngine>& use_engine)
{
    engine = use_engine;   
}

void RoomManager::set_state(unsigned int requested_level)
{
    switch(requested_level)
    {
        case 1:
            // TO DO 1
            show_maze();
            break;
        case 2:
            flush_state();
            // TO DO 2
            show_maze();
            break;
    }
}

void RoomManager::flush_state()
{
    for(unique_ptr<Pellet>& pellet: pellets)
        pellet->active = true;
    for(unique_ptr<Energizer>& energizer: energizers)
        energizer->active = true;
    eaten_pellets_num = 0;
    hide_maze();
}

void RoomManager::create()
{
    // Obtaining pointer to the root (window item), then creating context:
    QQuickWindow* window = qobject_cast<QQuickWindow*>(engine->rootObjects().at(0));
    QQuickItem* root = window->contentItem();
    QQmlComponent wall_comp(engine.get(), QUrl("qrc:/QMLs/QMLs/WallBlock.qml"));
    QQmlComponent pellet_comp(engine.get(), QUrl("qrc:/QMLs/QMLs/Pellet.qml"));
    QQmlComponent energizer_comp(engine.get(), QUrl("qrc:/QMLs/QMLs/Energizer.qml"));
    QQmlComponent houseentry_comp(engine.get(), QUrl("qrc:/QMLs/QMLs/HouseEntry.qml"));

    // Parsing maze to create maze objects and their visual representation:
    unsigned int x = 0;
    unsigned int y = 0;
    for(vector<int> row: maze)
    {
        for(int tile: row)
        {
            if(tile == 1 || tile == 4) // (wall or entry to ghosts' house)
            {
                unique_ptr<WallBlock> new_wall = make_unique<WallBlock>(x, y);
                if(tile == 1)
                    new_wall->quickitem = qobject_cast<QQuickItem*>(wall_comp.create());
                else if(tile == 4)
                    new_wall->quickitem = qobject_cast<QQuickItem*>(houseentry_comp.create());
                QQmlApplicationEngine::setObjectOwnership(new_wall->quickitem, QQmlApplicationEngine::CppOwnership);
                new_wall->quickitem->setParentItem(root);
                new_wall->quickitem->setParent(new_wall->parent.get());
                new_wall->quickitem->setProperty("x", new_wall->x_pos * constants::TILE_SIZE);
                new_wall->quickitem->setProperty("y", new_wall->y_pos * constants::TILE_SIZE);
                wallblocks.push_back(move(new_wall));
            }
            else if(tile == 0) // (power pellet)
            {
                unique_ptr<Pellet> new_pellet = make_unique<Pellet>(x, y);
                new_pellet->quickitem = qobject_cast<QQuickItem*>(pellet_comp.create());
                QQmlApplicationEngine::setObjectOwnership(new_pellet->quickitem, QQmlApplicationEngine::CppOwnership);
                new_pellet->quickitem->setParentItem(root);
                new_pellet->quickitem->setParent(new_pellet->parent.get());
                new_pellet->quickitem->setProperty("x", new_pellet->x_pos * constants::TILE_SIZE);
                new_pellet->quickitem->setProperty("y", new_pellet->y_pos * constants::TILE_SIZE);
                pellets.push_back(move(new_pellet));
            }
            else if(tile == 2) // (energizer)
            {
                unique_ptr<Energizer> new_energizer = make_unique<Energizer>(x, y);
                new_energizer->quickitem = qobject_cast<QQuickItem*>(energizer_comp.create());
                QQmlApplicationEngine::setObjectOwnership(new_energizer->quickitem, QQmlApplicationEngine::CppOwnership);
                new_energizer->quickitem->setParentItem(root);
                new_energizer->quickitem->setParent(new_energizer->parent.get());
                new_energizer->quickitem->setProperty("x", new_energizer->x_pos * constants::TILE_SIZE);
                new_energizer->quickitem->setProperty("y", new_energizer->y_pos * constants::TILE_SIZE);
                energizers.push_back(move(new_energizer));
            }
            x++;
        }
        x = 0;
        y++;
    }

    total_pellets_num = pellets.size() + energizers.size();
}

void RoomManager::show_maze()
{
    for(unique_ptr<WallBlock>& wallblock: wallblocks)
        wallblock->quickitem->setProperty("visible", true);
    for(unique_ptr<Pellet>& pellet: pellets)
        pellet->quickitem->setProperty("visible", true);
    for(unique_ptr<Energizer>& energizer: energizers)
        energizer->quickitem->setProperty("visible", true);
}

void RoomManager::hide_maze()
{
    for(unique_ptr<WallBlock>& wallblock: wallblocks)
        wallblock->quickitem->setProperty("visible", false);
    for(unique_ptr<Pellet>& pellet: pellets)
        pellet->quickitem->setProperty("visible", false);
    for(unique_ptr<Energizer>& energizer: energizers)
        energizer->quickitem->setProperty("visible", false);
}

unsigned int RoomManager::check_tile(unsigned int& curr_x, unsigned int& curr_y, int& direction,
                                     unsigned int target_x, unsigned int target_y)
{
    // Return codes correspond to room object located at the target tile:
    // 0 (power pellet) -- movement permitted
    // 1 (wall) -- movement denied or Pacman has eaten all the pellets
    // 2 (energizer) -- movement permitted & caller should enter the 'energized' mode
    // 3 (end of corridor) -- caller's coordinates are changed, caller should do nothings
    // 5 -- if this pellet was the last one on this level and player should proceed to
    // the next one

    // Check for the right end of the corridor:
    if(target_x == 19 && target_y == 9)
    {
        curr_x = 0;
        direction = PacmanDirection::PAC_RIGHT;
        return 3;
    }
    // Check for the left end of the corridor:
    else if(target_x == UINT_MAX && target_y == 9)
    {
        curr_x = 18;
        direction = PacmanDirection::PAC_LEFT;
        return 3;
    }
    // Check for collision with walls:
    else if(maze[target_y][target_x] == 1 || maze[target_y][target_x] == 4)
        return 1;
    // Check for tiles that contain nothing:
    else if(maze[target_y][target_x] == 3)
        return 0;
    // Check for power pellet:
    else if(maze[target_y][target_x] == 0)
    {
        for(unique_ptr<Pellet>& pellet: pellets)
        {
            // (check if the pellet is not eaten yet)
            if(pellet->x_pos == target_x && pellet->y_pos == target_y && pellet->active == true)
            {
                pellet->active = false;
                pellet->quickitem->setProperty("visible", false);
                emit ate(pellet_points); // (update score)

                eaten_pellets_num++;

                if(eaten_pellets_num == total_pellets_num) // (if this pellet was the last one)
                    return 5;

                break;
            }
        }
        return 0;
    }
    // Check for energizer:
    else if(maze[target_y][target_x] == 2)
    {
        for(unique_ptr<Energizer>& energizer: energizers)
        {
            // (check if the energizer is not eaten yet)
            if(energizer->x_pos == target_x && energizer->y_pos == target_y && energizer->active == true)
            {
                energizer->active = false;
                energizer->quickitem->setProperty("visible", false);
                emit ate(energizer_points); // (update score)

                eaten_pellets_num++;

                if(eaten_pellets_num == total_pellets_num) // (if this pellet was the last one)
                    return 5;

                break;
            }
        }
        return 2;
    }
}

void RoomManager::get_path(stack<Position>& trajectory, Position& source, Position& destination)
{
    #ifdef PATHFINDER_DEBUG
    qDebug() << "'RoomManager::get_path':";
    qDebug() << "Source: x " << source.x << " y " << source.y;
    qDebug() << "Destination: x " << destination.x << " y " << destination.y;
    #endif

    if(!is_valid(source) || !is_valid(destination))
    {
        #ifdef PATHFINDER_DEBUG
        qDebug() << "'RoomManager::get_path':";
        qDebug() << "Source or destination is not valid. Returning.";
        #endif
        return;
    }

    if(is_destination(source, destination))
    {
        #ifdef PATHFINDER_DEBUG
        qDebug() << "'RoomManager::get_path':";
        qDebug() << "Source is the destination. Returning.";
        #endif
        return;
    }

    vector<Position> open;
    vector<Position> closed;
    vector<shared_ptr<Position>> potential_parents;

    open.push_back(source);

    // While 'open' list is not empty:
    while(!open.empty())
    {
        // 1. Find the node with the lowest f-value on the 'open' list;
        // (if there are > 1 of such nodes, find the one with the lowest h-value)
        vector<Position>::iterator current = min_element(open.begin(), open.end(),
        [](const Position& fst, const Position& scnd)
        {if(fst.f < scnd.f)
            return true;
        else if(fst.f > scnd.f)
            return false;
        else if(fst.f == scnd.f)
            return fst.h < scnd.h;
        });

        shared_ptr<Position> new_parent = make_shared<Position>(*current);
        potential_parents.push_back(new_parent);

        closed.push_back(*current);
        open.erase(current);

        vector<Position> adjacent = find_valid_adjacent(new_parent.get());

        for(Position& node: adjacent)
        {
            // If we have reached our destination:
            if(is_destination(node, destination))
            {
                node.parent = new_parent.get();
                node.g = node.parent->g + 1;
                node.h = calculate_h(node, destination);
                node.f = node.g + node.h;
                closed.push_back(node);

                // Clear the 'open' list to exit the while-loop:
                open.clear();

                #ifdef PATHFINDER_DEBUG
                qDebug() << "'RoomManager::get_path':";
                qDebug() << "Found the destination!";
                qDebug() << "Path length: " << node.g;
                #endif

                trace_back_path(trajectory, node);
                break;
            }

            // If we haven't reached the destination yet:
            node.parent = new_parent.get();
            node.g = node.parent->g + 1;
            node.h = calculate_h(node, destination);
            node.f = node.g + node.h;

            // Check if node with the same position is already in the 'closed' list
            // and it has lower f-value:
            vector<Position>::iterator closed_it = find_if(closed.begin(), closed.end(),
            [&node](const Position& another_node)
            { return (another_node.x == node.x && another_node.y == node.y && another_node.f < node.f);});
            if(closed_it != closed.end())
                continue;

            // Check if node with the same position is already in the 'open' list
            // and it has lower f-value:
            vector<Position>::iterator open_it = find_if(open.begin(), open.end(),
            [&node](const Position& another_node)
            { return (another_node.x == node.x && another_node.y == node.y);});

            // If found, ...
            if(open_it != open.end())
            {
                if(open_it->f <= node.f)
                   continue;
                if(open_it->f > node.f)
                {
                    (*open_it) = node;
                    continue;
                }
            }

            open.push_back(node);
        }
     }

    #ifdef PATHFINDER_DEBUG
    qDebug() << "'RoomManager::get_path':";
    qDebug() << "Finished";
    #endif
}

bool RoomManager::is_valid(const Position& position)
{
    if(position.x < 0 || position.x > maze_width - 1 ||
       position.y < 0 || position.y > maze_height - 1)
        return false;
    if(maze[position.y][position.x] == 0 || maze[position.y][position.x] == 2 ||
       maze[position.y][position.x] == 3 || maze[position.y][position.x] == 4)
        return true;
    else
        return false;
}

bool RoomManager::is_destination(const Position& position, const Position& destination)
{
    if(position.x == destination.x && position.y == destination.y)
        return true;
    else
        return false;
}

vector<Position> RoomManager::find_valid_adjacent(const Position* position)
{
    int x_shifts[] = {0, -1, 1, 0};
    int y_shifts[] = {-1, 0, 0, 1};

    vector<Position> adjacent;
    for(int i = 0; i < 4; i++)
    {
        int possible_x = position->x + x_shifts[i];
        int possible_y = position->y + y_shifts[i];
        Position new_position(possible_x, possible_y);
        if(is_valid(new_position))
            adjacent.push_back(new_position);
    }
    return adjacent;
}

double RoomManager::calculate_h(const Position& position, const Position& destination)
{
    // Calculating the Manhattan distance:
    double h = std::abs(position.x - destination.x) + std::abs(position.y - destination.y);
    return h;
}

void RoomManager::trace_back_path(std::stack<Position>& trajectory, Position& from_destination)
{
    Position chain_node = from_destination;
    while(chain_node.parent != nullptr)
    {

//        #ifdef PATHFINDER_DEBUG
//        qDebug() << chain_node.x << chain_node.y;
//        #endif

        trajectory.push(chain_node);
        chain_node = *(chain_node.parent);
    }
}





