#ifndef BALLON_HPP
#define BALLON_HPP
#include "GameClock.hpp"


GameClock::GameClock(int interv){
    interval = interv;
    clock = new Clock;
    
};
void GameClock::checkForTick(){
    Time elapsed = clock->getElapsedTime();
    
    if(elapsed.asMilliseconds() > interval){
    
        tick();
        clock->restart();
    }

};
int GameClock::checkForKill(){
    if(Kill())
        return 1;
    return 0;

};
#endif