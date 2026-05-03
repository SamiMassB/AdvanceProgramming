#ifndef BALLON_HPP
#define BALLON_HPP
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "GameClock.hpp"
#include "config.hpp"
using namespace std;
using namespace sf;

class Ballon {
    private:
        System* system;
        Texture* texture;
        Sprite* sprite;
        int speed;
        int health, direction;
        int x, y, x_size, y_size;
        int life_loss_per_ballon;
        string name;
        pair<int, int> startPoint;
        pair<int, int> endPoint;
        bool attacked = 0;
        bool killed = 0;
    public:
        Ballon(string name, System* sys, float inp_x = -1, float inp_y = -1, int dir = -1);  
        System* getSystem();
        void updateHealth();
        void updateSpeed();
        void move();
        void doKill(int merge_pregnant = 1, int add_money = 1);
        void checkDirection();
        void handleCommand(string cmd);
        int getX();
        int getY();
        bool isFreezed(){return speed == 0;};
        bool is_killed(){return killed;};
        bool is_attacked(){return attacked;};
        void make_attacked(bool x){attacked = x;};
        Sprite* getSprite(){return sprite;};
        pair<int, int> getStartPoint() const { return startPoint; }
        pair<int, int> getEndPoint() const { return endPoint; }
    
        void initializeMap(const string grid[MAX_ROW]) {
            for(int y = 0; y < MAX_ROW; y++) {
                for(int x = 0; x < (int)grid[y].size(); x++) {
                    if(grid[y][x] == 'S') startPoint = {x, y};
                    if(grid[y][x] == 'F') endPoint = {x, y};
                }
            }
        }
};
class BallonClock: public GameClock{
    private:
        System* rel_system;
        Ballon* rel_ballon;
        string command;
        bool done = 0;
        int interval;
        int tick_count;
        
    public:
        BallonClock(Ballon* ballon, string com, int interv, System* sys, int count);
        void tick();
        void handleCommand();
        void updateSpeed(); 
        int Kill();
};
#endif 
