#include <SFML/Audio.hpp>
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "system.hpp"
#include <random>

using namespace std;
using namespace sf;


void System::tick() {    
    if (isGameOver) return;

    for(auto* new_order: new_orders)
        orders.push_back(new_order);
    new_orders.clear();

    for(auto* order: orders) {
        order->checkForTick();
        if(order->Kill() == 0)
            new_orders.push_back(order);
    }
    swap(orders, new_orders);
    new_orders.clear();
}

System::~System() {
    for(auto i: ground) delete i;
    for(auto i: ballons) delete i;
    for(auto i: towers) delete i;
    for(auto i: bullets) delete i;
    for(auto i: orders) delete i;
    for(auto i: new_orders) delete i;
}
void System::addClock(GameClock* clock){
    new_orders.push_back(clock);
}
void System::addBallon(string name, float inp_x, float inp_y, int dir){
    Ballon* ballon = new Ballon(name, this, inp_x, inp_y, dir);
    ballons.push_back(ballon);
}
void System::addTower(string name, int x, int y) {
    if(name == SIMPLE_TOWER) {
        if (money >= SIMPLE_TOWER_COST) {
            money -= SIMPLE_TOWER_COST;
        } else return; 
    }
    else if(name == ICE_TOWER) {
        if (money >= ICE_TOWER_COST) {
            money -= ICE_TOWER_COST;
        } else return;
    }
    else if(name == BOMB_TOWER) {
        if (money >= BOMB_TOWER_COST) {
            money -= BOMB_TOWER_COST;
        } else return;
    }
    
    Tower* tower = new Tower(name, x, y, this);
    towers.push_back(tower);
    updateMenuInfo(); 
}
    
vector<Ballon*> System::getBallons(){
    return ballons;
}
vector<Tower*> System::getTowers(){
    return towers;
}
void System::makeGround(string inp[MAX_ROW]){

    int x_len = 0, y_len = 0;
    float x_size, y_size;
    x_len = inp[0].length();
    while(inp[y_len].length() == x_len)
        y_len += 1;
    x_size = float (WINDOW_WIDTH) / (x_len);
    y_size = float (WINDOW_HEIGHT) / (y_len);
    ground_height = x_len;
    ground_width = y_len;
    
    vector <Sprite*> res;
    
    for (int i = 0; i < MAX_ROW; i++) map[i] = inp[i];  

    for(int i = 0; i < y_len;i++){
        for(int j = 0; j < x_len; j++){
            
            auto texture = new Texture;
            auto sprite = new Sprite;
            if(inp[i][j] == GRASS_SIGN){
                if(!texture->loadFromFile(GRASS_PICTURE_PATH))
                    cout << GRASS_PATH_ERR << endl;
            }
            else{
                if(!texture->loadFromFile(WALK_PICTURE_PATH))
                    cout << WALK_PATH_ERR << endl;
            }
            if(inp[i][j] == START_SYNTAX)
                startPoint = make_pair(i, j);
            if(inp[i][j] == END_SYNTAX)
                endPoint = make_pair(i, j);
       
            sprite->setTexture(*texture);
            sprite->setScale(float (x_size) / texture->getSize().x, float (y_size) / texture->getSize().y);
            
            sprite->setPosition(x_size * j, y_size * i);
        

            ground.push_back(sprite);
         
        }
    }


}
void System::addBullet(TowerBullet* bullet){    
    bullets.push_back(bullet);
}
vector<Sprite*> System::getGround(){
    return ground;
}
void System::removeBullet(TowerBullet* bullet){
    auto it = remove(bullets.begin(), bullets.end(), bullet);
    bullets.erase(it, bullets.end());
    delete bullet;
}
void System::removeBallon(Ballon* ballon) {
    if (ballon == nullptr){
        return;
    }
    auto it = remove(ballons.begin(), ballons.end(), ballon);
    ballons.erase(it, ballons.end());
    delete ballon;
}



bool System::isEndPoint(int x, int y) {
    if(abs(x - endPoint.second) + abs(y - endPoint.first) > 2)
        return 0;

    return (x >= ground_height || x < 0 || y >= ground_width || y < 0);
}

pair<float, float> System::getCellSize() {
    auto size = ground[0]->getGlobalBounds().getSize();
    return make_pair(size.x, size.y);
}
pair<int, int> System::getStartPoint(){
    
    for(int i = 0; i < MAX_ROW; i++)
        for(int j = 0; j < map[i].length(); j++)
            if(map[i][j] == START_SYNTAX)
                return make_pair(j, i);
    return make_pair(5, 0);
}

bool System::isPath(int mapX, int mapY) {
    if (mapY < 0 || mapY >= MAX_ROW || mapX < 0 || mapX >= (int)map[mapY].size())
        return false;
    char c = map[mapY][mapX];
    return (c == START_SYNTAX || c == END_SYNTAX || c == PATH_SYNTAX);
}

void System::startWaveSystem(int waveIndex) {
    waveStartSound.play();
    const AttackWave& wave = ATTACKING_PLAN[waveIndex];
    waveBalloons.clear();

    for(const auto& [type, count] : wave.enemiesCount) {
        for(int i = 0; i < count; i++) {
            waveBalloons.push_back(type);  
        }
    }
    
    auto rng = default_random_engine{};
    shuffle(waveBalloons.begin(), waveBalloons.end(), rng);
    for(int index = 0; index < waveBalloons.size(); index++){
        Ballon* ballon = nullptr;
        addClock(new BallonClock(ballon, BALLON_CREATE + ":" + waveBalloons[index], last_ballon_time, this, 1));
        last_ballon_time += rand() % (wave.enemyLaunchGapMs.second - wave.enemyLaunchGapMs.first) + wave.enemyLaunchGapMs.first;
    }
    last_ballon_time += WAVE_LAUNCH_GAP_SECS * 1000;
    Ballon* ballon = nullptr;
    addClock(new BallonClock(ballon, INCREASE_ROUND + ":", last_ballon_time, this, 1));
}


void System::initializeMenu(){
    currentWave = 0;

    menu = new Menu();
    menu->setPosition(WINDOW_WIDTH, 10);
    menu->setRound(currentWave);
    menu->setMoney(money);
    menu->setLives(life);
    menu->setBlank("");
    
    if (!gameOverBuffer.loadFromFile(GAME_OVER_SOUND_PATH)) {
        cout << ERR_GAME_OVER_SOUND << endl;
    }
    gameOverSound.setBuffer(gameOverBuffer);

    if (!backgroundMusic.openFromFile(BACKGROUND_MUSIC_PATH)) {
        cout << ERR_BACK_GROUND_SOUND << endl;
    }
    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(50); 
    backgroundMusic.play();
    
}
void System::addTowerMenu(string path){
    menu->setImage(path);
}
void System::drawMenu(RenderWindow* window){
    menu->draw(window);
}

void System::addTowerByCoords(float x, float y) {
    if (canPlaceTowerAt(x, y)) {
        Vector2f center = getCellCenter(x, y);
        addTower(SIMPLE_TOWER, center.x, center.y);
    }
}

vector<Sprite*> System::getTowerMenuSprites() {
    return menu->getImages();
}

bool System::canPlaceTowerAt(int x, int y) {
    if (x > WINDOW_WIDTH){
        return false;
    }
    int col = x / getCellSize().first;
    int row = y / getCellSize().second;
    if (!isPath(col, row)) {
        for (auto* t : towers) {
            int tx = t->getSprite()->getPosition().x;
            int ty = t->getSprite()->getPosition().y;
            int tcol = tx / getCellSize().first;
            int trow = ty / getCellSize().second;
            if (tcol == col && trow == row)
                return false;
        }
        return true;
    }
    return false;
}

Vector2f System::getCellCenter(float x, float y) {
    int col = x / getCellSize().first;
    int row = y / getCellSize().second;
    float cx = col * getCellSize().first + getCellSize().first / 2;
    float cy = row * getCellSize().second + getCellSize().second / 2;
    return {cx, cy};
}
void System::updateMenuInfo() {
    menu->setLives(life);
    menu->setMoney(money);
    menu->setRound(currentWave + 1);
    if (currentWave == 3){
        isGameOver = true;
    }
}

void System::decreaseLife(int delta_life) {
    life -= delta_life;
    updateMenuInfo();

    if (life <= 0 && !isGameOver) {
        isGameOver = true;
        gameOverSound.play();
        menu->setBlank(GAMEOVER);
        backgroundMusic.stop();
    }
}

void System::addMoney(int amount) {
    money += amount;
    updateMenuInfo();
}
void System::increase_round(){
    currentWave ++;
}