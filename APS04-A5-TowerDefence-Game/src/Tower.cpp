#include "Tower.hpp"

Tower::Tower(string inp_name, int input_x, int input_y, System* sys){
    x = input_x;
    y = input_y;
    if(inp_name == ICE_TOWER){
        cost = ICE_TOWER_COST;
        name = ICE_TOWER;
        pow = ICE_TOWER_POWER;
        interval = ICE_TOWER_INTERVAL;
        findTargets = IceTowerFindTarget;
        icon = ICE_TOWER_ICON;
        tower_radius = ICE_TOWER_RADIUS;
    }
    else if(inp_name == SIMPLE_TOWER){
        cost = SIMPLE_TOWER_COST;
        name = SIMPLE_TOWER;
        pow = SIMPLE_TOWER_POWER;
        interval = SIMPLE_TOWER_INTERVAL;
        findTargets = SimpleTowerFindTarget;
        icon = SIMPLE_TOWER_ICON;
        tower_radius = ICE_TOWER_RADIUS;
    }
    else if(inp_name == BOMB_TOWER){
        cost = BOMB_TOWER_COST;
        name = BOMB_TOWER;
        pow = BOMB_TOWER_POWER;
        interval = BOMB_TOWER_INTERVAL;
        findTargets = BombTowerFindTarget;
        icon = BOMB_TOWER_ICON;
        tower_radius = BOMB_TOWER_RADIUS;
    }
    rel_system = sys;
    texture = new Texture;
    sprite = new Sprite;
    if (!texture->loadFromFile(icon))
        throw runtime_error(TOWER_ICON_ERR + icon);
    sprite->setTexture(*texture);
    auto bounds = sprite->getLocalBounds();
    sprite->setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    auto sprite_size = sprite->getGlobalBounds().getSize();
    auto target_size = rel_system->getCellSize();
    sprite->setScale(target_size.first / sprite_size.x, target_size.second / sprite_size.y);
    sprite->setPosition(x, y);
    addTowerClock(TOWER_FIND_TARGET + ":", TOWER_FIND_TARGET_INTERVAL);
    addTowerClock(TOWER_MAKE_SHOT + ":", interval);
    
}
float getDistant(float x, float y, float x2, float y2){
    return sqrt((x - x2)*(x - x2) + (y - y2)*(y - y2));
}

Vector2f Tower::getCenter(){
    auto bounds = sprite->getGlobalBounds();
    Vector2f center(
        bounds.left + bounds.width / 2.f,
        bounds.top + bounds.height / 2.f
    );
    return center;
}
void Tower::addBullets(){
    vector<Ballon*> targets = findTargets(x, y, tower_radius, rel_system);
    if (targets.empty()) return;

    auto center = this->getCenter();
    TowerBullet* order = new TowerBullet(name, pow, bullet_interval, bullet_speed, targets, center.x, center.y, rel_system);
    rel_system->addClock(order);
    rel_system->addBullet(order);
   
    for(auto target: targets){
        
        target->make_attacked(1);
    
    }
    if(targets.size())
        last_target = targets[0];
}
Sprite* Tower::getSprite(){
    return sprite;
}
void Tower::rotateToTarget(){
    Vector2f position = sprite->getPosition();
    if(last_target == NULL || last_target->is_killed() || (last_target->isFreezed() && name == ICE_TOWER)){
        auto targets = findTargets(x, y, tower_radius, rel_system);
        if(targets.size())
            last_target = findTargets(x, y, tower_radius, rel_system)[0];
        
        last_target = nullptr;

        return;
    }
    Ballon* target = last_target;
    if (!target || target->is_killed()) return;
    
    
    
    float dx =  target->getX() - position.x;
    float dy = target->getY() - position.y;
    float angleRad = atan2(dy, dx);
    float angleDeg = angleRad * 180 / static_cast<float>(M_PI);
    angleDeg += 87;
    sprite->setRotation(angleDeg);


}
void Tower::handleCommand(string cmd_line){
    string cmd, command_type, command_value;
    vector <string> orders;
    istringstream iss(cmd_line);
    while(getline(iss, cmd, ' ')){
        istringstream order(cmd);
        getline(order, command_type, ':');
        orders.push_back(command_type);
        while(getline(order, command_value, ','))
            orders.push_back(command_value);
        if(orders[0] == TOWER_MAKE_SHOT)  addBullets();
        if(orders[0] == TOWER_FIND_TARGET) rotateToTarget();

    }


}
void Tower::addTowerClock(string cmd, int interv){
    TowerClock* clock = new TowerClock(this, cmd, interv);
    rel_system->addClock(clock);
}
TowerClock::TowerClock(Tower* tow, string command, int interv) : GameClock(interv){
    rel_tower = tow;
    interval = interv;
    cmd = command;
}
void TowerClock::tick(){
    rel_tower->handleCommand(cmd);
}
int TowerClock::Kill(){
    return 0;
}
TowerBullet::TowerBullet(string tower_name, string power, int interval, int speed, vector<Ballon*> target_ballons, int xx, int yy, System* sys) : GameClock(interval){
    if (target_ballons.empty()) {
        is_kill = true;
        return;
    }
    target = target_ballons[0];
    if (!target || target->is_killed()) {
        is_kill = true;
        return;
    }
    
    
    name = tower_name;
    pow = power;
    bullet_interval = interval;
    bullet_speed = speed;
    ballon_targets = target_ballons;
    target = ballon_targets[0];
    system = sys;
    x = xx;
    y = yy;
    texture = new Texture;
    if(!texture->loadFromFile(icon))
        cerr << FILE_ERR << endl;
    sprite = new Sprite;
    auto bounds = sprite->getLocalBounds();
    sprite->setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    sprite->setTexture(*texture);
    sprite->setScale(0.1, 0.1);
    sprite->setPosition(x, y);
}
void TowerBullet::tick(){
    if(target->is_killed()){
        is_kill = 1;
        return;
    }
    float dx, dy;
    dx = target->getX() - x;
    dy = target->getY() - y;
    x += bullet_speed * (dx / sqrt(dx * dx + dy * dy));
    y += bullet_speed * (dy / sqrt(dx * dx + dy * dy));
    sprite->setScale(0.1, 0.1);
    sprite->setPosition(x, y);
    isCollide();
}
void TowerBullet::isCollide(){
    if(sprite->getGlobalBounds().intersects(target->getSprite()->getGlobalBounds())){
        is_kill = 1;
        for(auto *tar: ballon_targets){
            tar->make_attacked(0);
            tar->handleCommand(pow);
        }
    }
}
int TowerBullet::Kill(){
    if(is_kill == 0)
        return 0;
    system->removeBullet(this);
    
    return 1;
}
vector<Ballon*> SimpleTowerFindTarget(int x, int y, int tower_radius, System* system){
    pair<int, Ballon*> min_dis_target = make_pair(-1, nullptr);
    vector< Ballon*> res;
    for(auto *i: system->getBallons()){
        if(i->is_attacked())
            continue;
        if(getDistant(x, y, i->getX(), i->getY()) > tower_radius)
            continue;
        if(min_dis_target.first == -1 || getDistant(x, y, i->getX(), i->getY()) < min_dis_target.first){

            min_dis_target = make_pair(getDistant(x, y, i->getX(), i->getY()), i);
        }
    }
    if(min_dis_target.second != nullptr)
        res.push_back(min_dis_target.second);
    return res;
}
vector<Ballon*> IceTowerFindTarget(int x, int y, int tower_radius, System* inp_system){
    pair<int, Ballon*> min_dis_target = make_pair(-1, nullptr);
    vector< Ballon*> res;
    for(auto *i: inp_system->getBallons()){
        if(i->is_attacked())
            continue;
        if(i->isFreezed())
            continue;
        if(getDistant(x, y, i->getX(), i->getY()) > tower_radius)
            continue;
        if(min_dis_target.first == -1 || getDistant(x, y, i->getX(), i->getY()) < min_dis_target.first){
            min_dis_target = make_pair(getDistant(x, y, i->getX(), i->getY()), i);
        }
    }
    if(min_dis_target.second != nullptr)
        res.push_back(min_dis_target.second);
    return res;
}
vector<Ballon*> BombTowerFindTarget(int x, int y, int tower_radius, System* inp_system){
    Ballon* best_target = nullptr;
    vector< Ballon*> res;
    int mx_cnt = -1, cnt = 0;
    for(auto *i: inp_system->getBallons()){
        if(i->is_attacked())
            continue;
        if(i->isFreezed())
            continue;
        if(getDistant(x, y, i->getX(), i->getY()) > tower_radius)
            continue;
        cnt = 0;
        for(auto *j: inp_system->getBallons()){
            float dx = abs(i->getX() - j->getX());
            float dy = abs(i->getY() - j->getY());

            if(sqrt(dx * dx + dy * dy) < BOMB_DAMAGE_RADIUS)
                cnt += 1;

        }
        if(cnt > mx_cnt){
            mx_cnt = cnt;
            best_target = i; 
        }
    }
    if(best_target == nullptr)
        return res;
    res.push_back(best_target);
    
    for(auto *j: inp_system->getBallons()){
        float dx = abs(best_target->getX() - j->getX());
        float dy = abs(best_target->getY() - j->getY());

        if(sqrt(dx * dx + dy * dy) < BOMB_DAMAGE_RADIUS && (dx != 0 || dy != 0))
            res.push_back(j);

    }
    return res;
}
