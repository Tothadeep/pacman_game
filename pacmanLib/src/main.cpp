/// \file
/// main.cpp

/*
 *  Created on: Jan 29, 2015
 *      Author:
 *       Group:
 */

#include "Classes.h"
#include "GameObjectStruct.h"
#include "UI.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream>
#include <vector>

/// Callback function to update the game state.
///
/// This function is called by an SDL timer at regular intervals.
/// Note that this callback may happen on a different thread than the main
/// thread. You therefore have to be careful in avoiding data races. For
/// example, you should use mutexes to access shared data. Read the
/// documentation of SDL_AddTimer for more information and for tips regarding
/// multithreading issues.

int highest_score{};

Uint32 gameUpdate(Uint32 interval, void * /*param*/) { return interval; }

void monster_collision(PacMan &pacman, std::vector<Ghosts *> ghost)
{
    for (auto &i : ghost) {
        if ((i->MonsterObj.x == pacman.MonsterObj.x) &&
            (i->MonsterObj.y == pacman.MonsterObj.y)) {
            if ((!i->is_scared())) // if the ghost is not scared
            {
                pacman.decrease_lives();
                pacman.MonsterObj.x = 13;
                pacman.MonsterObj.y = 5;
            }

            else // if the ghost is scared
            {
                pacman.ghosts_increase_score();
                i->reset();

                i->be_not_scared();
            }
        }
    }
};

/// Program entry point.
int main(int /*argc*/, char ** /*argv*/)
{
start:

    unsigned int time_ui = unsigned int(time(NULL)); // set the seed for random
    srand(time_ui);

    std::vector<GameObjectStruct> fruits{};
    std::vector<GameObjectStruct> fruits_fake{};

    std::vector<std::vector<int>> map = {{
#include "board.def"
    }};

    std::vector<GameObjectStruct> way{};
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map.at(i).size(); j++) {
            switch (map.at(i).at(j)) {
            case (1):
                break;
            case (0):
                GameObjectStruct dot;
                dot.x = j;
                dot.y = i;
                dot.type = DOT;
                dot.dir = UP;
                way.push_back(dot);
                break;
            }
        }
    }

    std::vector<std::vector<int>> map_backup; // places the dots on the map
    map_backup = map;

    std::vector<std::vector<int>> not_dots{
        {11, 9},  {11, 10}, {11, 11}, {11, 12}, {11, 13}, {11, 14},
        {11, 15}, {11, 16}, {11, 17}, {11, 18}, {15, 9},  {15, 10},
        {15, 11}, {15, 12}, {15, 13}, {15, 14}, {15, 15}, {15, 16},
        {15, 17}, {15, 18}, {13, 7},  {13, 8},  {13, 9},  {13, 12},
        {13, 13}, {13, 14}, {13, 15}, {13, 18}, {13, 19}, {13, 20},
        {12, 9},  {12, 13}, {12, 14}, {12, 18}, {14, 9},  {14, 18}};

    for (int i = 0; i < not_dots.size(); i++) {
        map_backup.at(not_dots[i][0]).at(not_dots[i][1]) = 1;
    }

    std::vector<GameObjectStruct> dots;

    for (int i = 0; i < map_backup.size(); i++) {
        for (int j = 0; j < map_backup.at(i).size(); j++) {
            switch (map_backup.at(i).at(j)) {
            case (1):
                break;
            case (0):
                GameObjectStruct dot;
                dot.x = j;
                dot.y = i;
                dot.type = DOT;
                dot.dir = UP;
                dots.push_back(dot);
                break;
            }
        }
    }

    // Create a new ui object
    UI ui(map); // <-- use map from your game objects.

    // Start timer for game update, call this function every 100 ms.
    SDL_TimerID timer_id =
        SDL_AddTimer(100, gameUpdate, static_cast<void *>(nullptr));

    Energizer energizer1;
    energizer1.EnergizerObj.x = 1;
    energizer1.EnergizerObj.y = 1;
    energizer1.EnergizerObj.type = ENERGIZER;
    energizer1.EnergizerObj.dir = UP;

    Energizer energizer2;
    energizer2.EnergizerObj.x = 26;
    energizer2.EnergizerObj.y = 1;
    energizer2.EnergizerObj.type = ENERGIZER;
    energizer2.EnergizerObj.dir = UP;

    Energizer energizer3;
    energizer3.EnergizerObj.x = 1;
    energizer3.EnergizerObj.y = 25;
    energizer3.EnergizerObj.type = ENERGIZER;
    energizer3.EnergizerObj.dir = UP;

    Energizer energizer4;
    energizer4.EnergizerObj.x = 26;
    energizer4.EnergizerObj.y = 25;
    energizer4.EnergizerObj.type = ENERGIZER;
    energizer4.EnergizerObj.dir = UP;

    std::vector<GameObjectStruct> energizers{
        energizer1.EnergizerObj, energizer2.EnergizerObj,
        energizer3.EnergizerObj, energizer4.EnergizerObj};

    PacMan pacman;

    Blinky blinky;
    Inky inky;
    Pinky pinky;
    Clyde clyde;

    std::vector<Ghosts *> monster_list{&blinky, &pinky, &inky, &clyde};

    // Call game init code here

    bool quit = false;
    while (!quit) {
        pacman.moving(map);
        monster_collision(pacman, monster_list);

        // set timeout to limit frame rate
        Uint32 timeout = SDL_GetTicks() + 80;

        // Handle the input
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            // Quit button.
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            // All keydown events
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_LEFT:
                    pacman.MonsterObj.dir = LEFT;
                    break;
                case SDLK_RIGHT:
                    pacman.MonsterObj.dir = RIGHT; // YOUR CODE HERE
                    break;
                case SDLK_UP:
                    pacman.MonsterObj.dir = UP; // YOUR CODE HERE
                    break;
                case SDLK_DOWN:
                    pacman.MonsterObj.dir = DOWN; // YOUR CODE HERE
                    break;
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                }
            }
        }

        pacman.dot_collision(dots);
        // pacman.energizer_collision(energizers,monster_list);
        pacman.energizer_collision(energizers, monster_list);

        for (auto &i : monster_list) {
            i->random_behav(map);
        }

        pacman.fruit_collision(fruits);

        for (auto i :
             monster_list) // calculates the collisions with the energizer
        {
            if (i->is_scared()) // if scared
            {
                if (i->get_timer() ==
                    0) // if the timer is equal to zero, set it to be not scared
                {
                    i->be_not_scared();
                } else { // if the timer is not zero, decrease it by one
                    i->decrease_timer_by_1();
                }
            }
        }

        // Set the score
        ui.setScore(pacman.get_score()); // <-- Pass correct value to the setter
        // Set the amount of lives
        ui.setLives(pacman.get_lives()); // <-- Pass correct value to the setter
        if (pacman.get_lives() == 0)
            quit = true;

        if (pacman.get_score() - 100 * fruits_fake.size() > 100) {
            int random_number = std::rand() % 6;
            int random_num_for_pos = std::rand() % 274;
            GameObjectStruct fruit;
            fruit.type = static_cast<Type>(random_number + 8);
            fruit.x = way.at(random_num_for_pos).x;
            fruit.y = way.at(random_num_for_pos).y;
            fruit.dir = UP;
            fruits.push_back(fruit);
            fruits_fake.push_back(fruit);
        }

        // Render the scene
        std::vector<GameObjectStruct> objects = {};
        objects.insert(objects.end(), dots.begin(), dots.end());

        for (auto &i : monster_list) {
            objects.push_back(i->MonsterObj);
        }

        objects.insert(objects.end(), energizers.begin(), energizers.end());
        objects.push_back(pacman.MonsterObj);
        objects.insert(objects.end(), fruits.begin(), fruits.end());
        // ^-- Your code should provide this vector somehow (e.g.
        // game->getStructs())

        ui.update(objects);

        if (pacman.get_lives() == 0) {
            std::cout << "Game Over :(" << std::endl;
            std::cout << "Your Score: " << pacman.get_score() << std::endl;
            if (highest_score < pacman.get_score()) {
                highest_score = pacman.get_score();
                std::cout << "New High Score!" << std::endl;
            }
            std::cout << "High Score: " << highest_score << std::endl;
            std::cout << "Press Enter to Play Again!" << std::endl;
            if (std::cin.get() == '\n') {
                goto start;
            }
        }
        if (dots.empty()) {
            std::cout << "Congratulations:)" << std::endl;
            std::cout << "Your Score: " << pacman.get_score() << std::endl;
            if (highest_score < pacman.get_score()) {
                highest_score = pacman.get_score();
                std::cout << "New High Score!" << std::endl;
            }
            std::cout << "High Score: " << highest_score << std::endl;
            std::cout << "Press Enter to Play Again!" << std::endl;
            if (std::cin.get() == '\n') {
                goto start;
            }
        }

        while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout)) {
            // ... do work until timeout has elapsed
        }
    }

    SDL_RemoveTimer(timer_id);

    return 0;
}