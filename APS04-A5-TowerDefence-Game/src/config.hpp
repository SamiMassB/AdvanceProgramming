#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <bits/stdc++.h>

using namespace std;

class System;
class Ballon;
class GameClock;
class TowerBullet;
class Tower;

const char START_SYNTAX = 'S';
const char END_SYNTAX = 'F';
const char PATH_SYNTAX = 'O';
const string FONT_PATH = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
const int WINDOW_TICK_INTERVAL = 50;
const int MENU_INITIAL_MONEY = 400;
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;
const int MENU_WIDTH = 200;
const int MENU_HEIGHT = WINDOW_HEIGHT;
const int MAX_ROW = 100;
const string INCREASE_ROUND = "increase_round";
const string GRASS_PICTURE_PATH = "./assets/sprites/Untitled 04-03-2025 11-10-07.png";
const string WALK_PICTURE_PATH = "./assets/sprites/Untitled 04-03-2025 11-21-11.png";
const string GRASS_PATH_ERR = "No such file found path to grass";
const string WALK_PATH_ERR = "No such file found path to ground";
const char WALK_SIGN = 'O';
const char GRASS_SIGN = '-';
const char START_SIGN = 'S';
const char END_SIGN = 'E';
const string WINDOW_BOMB_MENUICON = "./assets/sprites/IMG_20250511_183623_260.jpg";
const string WINDOW_SIMPLE_MENUICON = "./assets/sprites/IMG_20250511_183625_529.jpg";
const string WINDOW_ICE_MENUICON = "./assets/sprites/IMG_20250511_183624_752.jpg";
const string FONT_ERR = "Failed to load font!";
const string INVALID_PATH = "Invalid Path";


const int INITIAL_LIFE = 20;
const float CELL_SIZE = 32;
const int LIFE_LOSS_PER_BALLOON_SIMPLE = 1;
const int LIFE_LOSS_PER_BALLOON_PREGNANT = 2;
const string EXP_CMD = "explode";
const string UNFREEZE = "unfreeze";
const string DAMG_CMD = "damage";
const string GAMEOVER = "GAME OVER!";
const string GAMEWIN = "You WON!";


const float BALLON_SIZE_EPSILON = -0.02;
const string BALLON_UNFREEZE = "unfreeze";
const string BALLON_KILL = "kill";
const string BALLON_MOVE = "move";
const string BALLON_FREEZE = "freeze";
const string NORMAL_BALLON_NAME = "Normal";
const string PREGNANT_BALLOON_NAME = "Pregnant";
const string BALLON_ERROR = "Error loading balloon texture\n";
const int BALLON_FREEZE_TIME = 2000;
const int NORMAL_BALLOON_HEALTH = 1;
const int PREGNANT_BALLOON_HEALTH = 3;
const string BALLON_CREATE = "create";
const int NORMAL_BALLOON_SPEED = 10;
const int PREGNANT_BALLOON_SPEED = 10;
const int NORMAL_BALLOON_SCORE = 10;
const int PREGNANT_BALLOON_SCORE = 30;
const int BALLOON_SIZE = 30;
const int BALLOON_CLOCK_INTERVAL = 10; 
const int BALLOON_GAP_INTERVAL = 1000;
const string NORMAL_BALLOON_ICON = "./assets/sprites/Untitled 04-04-2025 04-56-37.png";
const string PREGNANT_BALLOON_ICON = "./assets/sprites/Untitled 04-04-2025 04-56-38.png";
const int BALLON_DELTA_X = 10;
const int BALLON_DELTA_Y = 10;


const float EPS_DIST = 0.0001;
const int TOWER_BULLET_INTERVAL = 20;
const string TOWER_MAKE_SHOT = "make_shot";
const string TOWER_FIND_TARGET = "rotate_to_target";
const int TOWER_FIND_TARGET_INTERVAL = 20;
const int BULLET_SPEED = 30;
const string ICE_TOWER = "ice";
const string SIMPLE_TOWER = "simple";
const string BOMB_TOWER = "bomb";
const string ICE_TOWER_POWER = BALLON_FREEZE + ":"; 
const string SIMPLE_TOWER_POWER = BALLON_KILL + ":"; 
const string BOMB_TOWER_POWER = BALLON_KILL + ":"; 
const int ICE_TOWER_COST = 150;
const int SIMPLE_TOWER_COST = 100;
const int BOMB_TOWER_COST = 200;
const int ICE_TOWER_INTERVAL = 500;
const int SIMPLE_TOWER_INTERVAL = 500;
const int BOMB_TOWER_INTERVAL = 1000;
const int DEAD_BALLON_PRIZE = 8;
const string ICE_TOWER_ICON = "./assets/sprites/rr.png";
const string SIMPLE_TOWER_ICON = "./assets/sprites/Untitled 04-04-2025 06-26-33.png";
const string BOMB_TOWER_ICON = "./assets/sprites/Untitled 04-04-2025 02-37-21.png";
const int ICE_TOWER_FREEZTIME = 10;
const string TOWER_BULLET_ICON = "./assets/sprites/download.png";
const int BOMB_DAMAGE_RADIUS = 60;
const int BOMB_TOWER_RADIUS = 150;
const int SIMPLE_TOWER_RADIUS = 150;
const int ICE_TOWER_RADIUS = 150;




struct AttackWave {
    map<std::string, int> enemiesCount;
    pair<int, int> enemyLaunchGapMs;
};

const vector<AttackWave> ATTACKING_PLAN = {
    {{{"Normal", 5}}, {500, 700}},
    {{{"Normal", 10}, {"Pregnant", 5}}, {400, 600}},
    {{{"Normal", 15}, {"Pregnant", 10}}, {200, 400}}
};
const int WAVE_LAUNCH_GAP_SECS = 10; 
const int INITIAL_WAVE_DELAY_SECS = 5;


const string ERR_WAVE_SOUND = "Failed to load wave start sound!";
const string ERR_BACK_GROUND_SOUND = "Failed to load background music!";
const string ERR_GAME_OVER_SOUND = "Failed to load Game Over sound!";
const string GAME_OVER_SOUND_PATH = "./assets/audio/gameover.wav";
const string BACKGROUND_MUSIC_PATH = "./assets/audio/background.ogg";
const string WAVE_START_SOUND_PATH = "./assets/audio/wave_start.wav";

const string TOWER_ICON_ERR = "Tower icon not found: ";
const string FILE_ERR = "No such file found";

#endif
