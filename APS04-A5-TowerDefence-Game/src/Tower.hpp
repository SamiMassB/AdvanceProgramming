#ifndef TOWER_HPP
#define TOWER_HPP
#include "Ballon.hpp"
#include "config.hpp"
#include "GameClock.hpp"
#include "Ballon.hpp"
#include "system.hpp"
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <cmath>
using namespace std;
using namespace sf;

class TowerBullet: public GameClock{
    private: 
        System* system;
        vector<Ballon*> ballon_targets;
        Ballon* target;
        Texture* texture;
        Sprite* sprite;
        string name, pow, show_pow, icon = TOWER_BULLET_ICON;
        int bullet_speed, bullet_interval;
        long double x, y;
        string shot_pow; 
        bool is_kill = 0;
    public:
        void tick();
        TowerBullet(string tower_name, string pow, int interval, int speed, vector<Ballon*> target_ballon, int xx, int yy, System* syst);
        int Kill();
        void isCollide();
        Sprite* getSprite(){return sprite;};
        
};
class Tower{
    private:
        Ballon* last_target = NULL;
        System* rel_system;
        Texture* texture;
        Sprite* sprite;
        int cost, interval, x, y, tower_radius; 
        string name;
        string pow;
        string icon;
        int bullet_speed = BULLET_SPEED;
        int bullet_interval = TOWER_BULLET_INTERVAL;
        function<vector<Ballon*>(int inp_x, int inp_y, int rad, System* inp_system)> findTargets;
            
    public:
        Tower(string name, int input_x, int input_y, System* syst);
        void handleCommand(string cmd);
        void addBullets();
        void addTowerClock(string cmd, int interv);
        Sprite* getSprite();
        Vector2f getCenter();
        void rotateToTarget();
        int getRadius() const { return tower_radius; }

    };
class TowerClock : public GameClock{
    private:
        string cmd;
        Tower* rel_tower;
        int interval;
    public:
        void tick();
        int Kill();
        TowerClock(Tower* tow, string command, int interv);
};
float getDistant(float x, float y, float x2, float y2);

vector<Ballon*> IceTowerFindTarget(int x, int y, int rad, System* inp_system);
vector<Ballon*> SimpleTowerFindTarget(int x, int y, int rad, System* inp_system);
vector<Ballon*> BombTowerFindTarget(int x, int y, int rad, System* inp_system);


#endif