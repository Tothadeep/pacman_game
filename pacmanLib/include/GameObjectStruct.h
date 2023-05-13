/// \file
/// GameObjectStruct.h

/*
 *  Created on: Jan 30, 2015
 *      Author: frits
 *       Group: TA's
 */

#ifndef SRC_GAMEOBJECTSTRUCT_H
#define SRC_GAMEOBJECTSTRUCT_H
#include <iostream>
#include <vector>
/// These enum values denote the sprite sets available for items on screen.
enum Type {
    PACMAN,
    BLINKY,
    PINKY,
    INKY,
    CLYDE,
    SCARED,
    SCAREDINV,
    CHERRY,
    STRAWBERRY,
    ORANGE,
    LEMON,
    APPLE,
    GRAPES,
    DOT,
    ENERGIZER,
    WALL,
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE
};

/// An enum to denote the direction of an item on screen.
enum Direction : int { UP, DOWN, LEFT, RIGHT };

/// This represents an item that is drawn onto the screen.
struct GameObjectStruct {
    /// x-position of the item.
    int x;
    /// y-position of the item.
    int y;
    /// The appearance of the item (sprite set).
    Type type;
    /// The direction of the item (sprite selection).
    Direction dir;
};

class Energizer
{
  public:
    GameObjectStruct EnergizerObj;
};

class Monster
{
  public:
    GameObjectStruct MonsterObj;

    void moving(std::vector<std::vector<int>> map) //*
    {
        
        switch (MonsterObj.dir) {
        case (UP):
            if (map.at(MonsterObj.y - 1).at(MonsterObj.x) == 1)
                break;
            MonsterObj.y -= 1;
            break;
        case (DOWN):
            if (map.at(MonsterObj.y + 1).at(MonsterObj.x) == 1)
                break;
            MonsterObj.y += 1;
            break;
        case (LEFT):
            if (MonsterObj.x == 0) 
            {
                MonsterObj.x = 27;
                break;
            }   
            else if (map.at(MonsterObj.y).at(MonsterObj.x - 1) == 1) 
            {
                break;
            }
            MonsterObj.x -= 1;
            break;
        case (RIGHT):
            if (MonsterObj.x == 27) 
            {
                MonsterObj.x = 0;
                break;
            }
            else if (map.at(MonsterObj.y).at(MonsterObj.x + 1) == 1) 
            {
                break;
            }
            MonsterObj.x += 1;
            break;
        }
    }
};

class Ghosts : public Monster
{
  private:
    bool scared = false;
    Type old_type = MonsterObj.type;
    int timer{};

  public:
    void random_behav(std::vector<std::vector<int>> map)
    {
        int random_number = std::rand() % 4;
        MonsterObj.dir = static_cast<Direction>(random_number);
        moving(map);
    }

    bool is_scared() { return scared; }

    void be_scared()
    {
        timer = 60;
        scared = true;
        old_type = MonsterObj.type;
        MonsterObj.type = SCARED;
    }

    void be_not_scared()
    {
        scared = false;
        MonsterObj.type = old_type;
    }

    int get_timer() { return timer; }

    virtual void reset(){};
    void decrease_timer_by_1() { timer--; }

    void set_timer_10() { timer = 10; }

    void set_timer_0() { timer = 0; }
};

class PacMan : public Monster
{
  private:
    int lives{3};
    int score{};

  public:
    PacMan()
    {
        this->MonsterObj.x = 13;
        this->MonsterObj.y = 5;
        this->MonsterObj.type = PACMAN;
        this->MonsterObj.dir = UP;
    }

    void energizer_collision(std::vector<GameObjectStruct> &energizers,
                             std::vector<Ghosts *> ghosts)
    {
        for (int i = 0; i < energizers.size(); i++) {
            if ((energizers.at(i).x == MonsterObj.x) &&
                (energizers.at(i).y == MonsterObj.y)) {
                dots_increase_score();
                energizers.erase(energizers.begin() + i);

                for (auto &i : ghosts) {
                    i->be_scared();
                }
            }
        }
    }
    void dot_collision(std::vector<GameObjectStruct> &dots)
    {
        for (int i = 0; i < dots.size(); i++) {
            if ((dots.at(i).x == MonsterObj.x) &&
                (dots.at(i).y == MonsterObj.y)) {
                dots_increase_score();
                dots.erase(dots.begin() + i);
            }
        }
    }
    void fruit_collision(std::vector<GameObjectStruct> &fruits, int fruit_count)
    {
        for (int i = 0; i < fruits.size(); i++) {
            if ((fruits.at(i).x == MonsterObj.x) &&
                (fruits.at(i).y == MonsterObj.y)) {
                fruits_increase_score();
                fruits.erase(fruits.begin() + i);
                fruit_count += 1;
            }
        }
    }

    void fruits_increase_score() { score += 25; }
    void decrease_lives() { lives--; }

    int get_lives() { return lives; }

    void dots_increase_score() { score++; }

    void ghosts_increase_score() { score += 200; }

    int get_score() { return score; }
};

class Blinky : public Ghosts
{

  public:
    Blinky()
    {
        this->MonsterObj.x = 13;
        this->MonsterObj.y = 13;
        this->MonsterObj.type = BLINKY;
        this->MonsterObj.dir = UP;
    }
    void reset()
    {
        MonsterObj.x = 13;
        MonsterObj.y = 13;
        MonsterObj.type = BLINKY;
        MonsterObj.dir = UP;
    }
};

class Pinky : public Ghosts
{
  public:
    Pinky()
    {
        this->MonsterObj.x = 15;
        this->MonsterObj.y = 13;
        this->MonsterObj.type = PINKY;
        this->MonsterObj.dir = UP;
    }
    void reset()
    {
        MonsterObj.x = 15;
        MonsterObj.y = 13;
        MonsterObj.type = PINKY;
        MonsterObj.dir = UP;
    }
};

class Inky : public Ghosts
{
  public:
    Inky()
    {
        this->MonsterObj.x = 6;
        this->MonsterObj.y = 8;
        this->MonsterObj.type = INKY;
        this->MonsterObj.dir = UP;
    }

    void reset()
    {
        MonsterObj.x = 6;
        MonsterObj.y = 8;
        MonsterObj.type = INKY;
        MonsterObj.dir = UP;
    }
};

class Clyde : public Ghosts
{
  public:
    Clyde()
    {
        this->MonsterObj.x = 21;
        this->MonsterObj.y = 12;
        this->MonsterObj.type = CLYDE;
        this->MonsterObj.dir = UP;
    }
    void reset()
    {
        MonsterObj.x = 21;
        MonsterObj.y = 12;
        MonsterObj.type = CLYDE;
        MonsterObj.dir = UP;
    }
};

#endif // SRC_GAMEOBJECTSTRUCT_H