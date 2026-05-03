#ifndef SYSTEM_HPP
#define SYSTEM_HPP
#include <SFML/Audio.hpp>
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "Ballon.hpp"
#include "Tower.hpp"
#include "GameClock.hpp"
#include "config.hpp"
#include "Menu.hpp"
using namespace std;
using namespace sf;



class System{    
    private:
        Menu* menu;
        vector<Sprite*> ground;
        vector<Ballon*> ballons;
        vector<Tower*> towers;
        vector<GameClock*> orders, new_orders;
        vector <TowerBullet*> bullets;
        int life = INITIAL_LIFE;
        int last_ballon_time = INITIAL_WAVE_DELAY_SECS * 1000;
        float cellSize = CELL_SIZE;
        string map[MAX_ROW];
        pair<int, int> startPoint;
        pair<int, int> endPoint;
        int currentWave;
        Clock waveClock;
        float nextWaveTime;
        bool waveActive;
        vector<string> waveBalloons;
        int balloonIndex;
        int nextBalloonTime, ground_height, ground_width;
        int money = MENU_INITIAL_MONEY;
        bool isGameOver = false;
        int life_loss_per_ballon;
        SoundBuffer gameOverBuffer;
        Sound gameOverSound;
        Music backgroundMusic;
        SoundBuffer waveStartBuffer;
        Sound waveStartSound;
        
    public:
        Menu* getMenu(){return menu;};
        void initializeMenu();
        ~System();
        void tick();
        pair <int, int> getStartPoint();
        void addTowerMenu(string path);
        void drawMenu(RenderWindow* window);
        void addBullet(TowerBullet* bullet);
        void addBallon(string name, float inp_x = -1, float inp_y = -1, int dir = -1);
        void addTower(string name, int x, int y);
        void addClock(GameClock* clock);
        void makeGround(string inp[MAX_ROW]);
        void removeBullet(TowerBullet* bullet);
        vector<TowerBullet*> getBullets(){return bullets;};
        vector<Sprite*> getGround();
        vector<Ballon*> getBallons();
        vector<Tower*> getTowers();
        void removeBallon(Ballon* ballon);
        void decreaseLife(int delta);
        bool isEndPoint(int x, int y);
        pair<float, float> getCellSize();
        bool isPath(int mapX, int mapY);
        void startWaveSystem(int index);
        void addTowerByCoords(float x, float y);
        vector<Sprite*> getTowerMenuSprites();

        Sprite* getCell(int x, int y){return ground[y * map[0].size() + x];};
        bool canPlaceTowerAt(int x, int y);
        Vector2f getCellCenter(float x, float y);
        void updateMenuInfo();
        void addMoney(int amount);
        void increase_round();

};
#endif
