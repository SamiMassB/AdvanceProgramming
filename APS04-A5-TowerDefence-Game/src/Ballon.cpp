#include "Ballon.hpp"
#include "config.hpp"
#include "system.hpp"
#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>

using namespace std;
using namespace sf;


Ballon::Ballon(string name, System* sys, float inp_x, float inp_y, int dir) : name(name), system(sys) {
    texture = new Texture();
    sprite = new Sprite();
    
    if(name == NORMAL_BALLON_NAME) {
        health = NORMAL_BALLOON_HEALTH;
        speed = NORMAL_BALLOON_SPEED;
        life_loss_per_ballon = LIFE_LOSS_PER_BALLOON_SIMPLE;
        if(!texture->loadFromFile(NORMAL_BALLOON_ICON)) throw  BALLON_ERROR;
    } 
    else if(name == PREGNANT_BALLOON_NAME) {
        health = PREGNANT_BALLOON_HEALTH;
        speed = PREGNANT_BALLOON_SPEED;
        life_loss_per_ballon = LIFE_LOSS_PER_BALLOON_PREGNANT;
        if(!texture->loadFromFile(PREGNANT_BALLOON_ICON)) throw BALLON_ERROR;
    }
    
    sprite->setTexture(*texture);
    auto cellSize = system->getCellSize();
    sprite->setScale(cellSize.first / (float)texture->getSize().x + BALLON_SIZE_EPSILON, cellSize.second / (float)texture->getSize().y + BALLON_SIZE_EPSILON);

    direction = 0;
    FloatRect bounds = sprite->getLocalBounds();
    sprite->setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    auto [startX, startY] = system->getStartPoint();
    if(inp_x == -1){
        x = startX * system->getCellSize().first + system->getCellSize().first / 2;
        y = startY * system->getCellSize().second + system->getCellSize().second / 2;
    }
    else {
        x = inp_x;
        y = inp_y;

    }
    x += rand() % BALLON_DELTA_X * 2 - BALLON_DELTA_X;
    y += rand() % BALLON_DELTA_Y * 2 - BALLON_DELTA_Y;
    if(dir != -1)
        direction = dir;
    sprite->setPosition(x, y);
    system->addClock(new BallonClock(this, BALLON_MOVE + ":", BALLOON_CLOCK_INTERVAL, system, -1));
    
}


void Ballon::updateHealth() {
    if(health <= 0) {
        system->addMoney(DEAD_BALLON_PRIZE); 
        system->removeBallon(this);
    }
}

void Ballon::doKill(int merge_pregnant, int add_money){
    if(name == PREGNANT_BALLOON_NAME && merge_pregnant){
        system->addBallon(NORMAL_BALLON_NAME, x, y, direction);
        system->addBallon(NORMAL_BALLON_NAME, x, y, direction);

    }
    killed = 1;
    if(add_money)
        system->addMoney(DEAD_BALLON_PRIZE); 
    system->removeBallon(this);
}

void Ballon::move() {
    checkDirection();
    float dx = 0, dy = 0;
    switch(direction) {
        case 0: dx = speed; break; 
        case 1: dy = speed; break;  
        case 2: dx = -speed; break; 
        case 3: dy = -speed; break; 
    }
    x += dx;
    y += dy;

    sprite->setPosition(x, y);

}


void Ballon::checkDirection() {
    if(killed)
        return;
    vector<int> possibleDirs;
    auto cellSize = system->getCellSize();
    int mapX = round((x - cellSize.first / 2) / cellSize.first);
    int mapY = round((y - cellSize.second / 2) / cellSize.second);
    int dx = 0, dy = 0;
    switch(direction) {
        case 0: dx = 1; break; 
        case 1: dy = 1; break;  
        case 2: dx = -1; break; 
        case 3: dy = -1; break; 
    }

    if(system->isEndPoint(mapX + dx, mapY + dy)) {
        system->decreaseLife(life_loss_per_ballon);
 
        doKill(0, 0);
        return;
    }
    
    if(!system->getCell(mapX + dx, mapY + dy)->getGlobalBounds().intersects(sprite->getGlobalBounds()))
        return;
    
    if(system->isPath(mapX + 1, mapY) && direction != 2) possibleDirs.push_back(0);
    if(system->isPath(mapX, mapY + 1) && direction != 3) possibleDirs.push_back(1);
    if(system->isPath(mapX - 1, mapY) && direction != 0) possibleDirs.push_back(2);
    if(system->isPath(mapX, mapY - 1) && direction != 1) possibleDirs.push_back(3);

    if(possibleDirs.size() > 1 && count(possibleDirs.begin(), possibleDirs.end(), direction)) {
        return; 
    }
    else if(!possibleDirs.empty()) {
        direction = possibleDirs[0]; 
    }
}

void Ballon::handleCommand(string cmd){
    istringstream iss(cmd);
    string type, value;
  
    while(getline(iss, type, ':')){
    
        if(type == BALLON_FREEZE){
            
            system->addClock(new BallonClock(this, BALLON_UNFREEZE, BALLON_FREEZE_TIME, system, 1));
            speed = 0;
        }
        if(type == BALLON_KILL){
            doKill();
        }
        if(type == BALLON_MOVE){
            move();
        }
        
        if(type == BALLON_UNFREEZE){
            if(name == NORMAL_BALLON_NAME)
                speed = NORMAL_BALLOON_SPEED;
            if(name == PREGNANT_BALLOON_NAME)
                speed = PREGNANT_BALLOON_SPEED;

        }
    }

}

BallonClock::BallonClock(Ballon* ballon, string com, int interv, System* sys, int count) : interval(interv), rel_system(sys), GameClock(interv), rel_ballon(ballon), command(com), tick_count(count) {}

void BallonClock::tick() {
    auto ballons = rel_system->getBallons();
    if(find(ballons.begin(), ballons.end(), rel_ballon) == ballons.end() && command.substr(0, command.find(":")) != BALLON_CREATE &&  command.substr(0, command.find(":")) != INCREASE_ROUND) {  
        done = 1;
        return;
    }
    istringstream iss(command);
    string type, value = "";
    getline(iss, type, ':');
    getline(iss, value, ' ');
    if(type == BALLON_CREATE){
        rel_system->addBallon(value);
    }
    if (!rel_ballon && type != BALLON_CREATE && type != INCREASE_ROUND) {
        done = 1;
        return;
    }
    if (type == INCREASE_ROUND){
        rel_system->increase_round();
    }
    rel_ballon->handleCommand(type + ":" + value);
    tick_count -= 1;
}

int BallonClock::Kill() {
    if(done)
        return 1;
    
    auto ballons = rel_system->getBallons();
    if(find(ballons.begin(), ballons.end(), rel_ballon) == ballons.end() && command.substr(0, command.find(":")) != BALLON_CREATE && command.substr(0, command.find(":")) != INCREASE_ROUND) {  
        return 1;
    }
    if(tick_count == 0)
        return 1;
    return 0;
}




int Ballon::getX() { return x; }
int Ballon::getY() { return y; }

System* Ballon::getSystem() {
    return system;
}